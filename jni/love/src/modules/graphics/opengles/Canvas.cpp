/**
* Copyright (c) 2006-2012 LOVE Development Team
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would be
*    appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
**/

#include "Canvas.h"
#include "Graphics.h"
#include <common/Matrix.h>

#include <cstring> // For memcpy

namespace love
{
namespace graphics
{
namespace opengles
{

	// strategy for fbo creation, interchangable at runtime:
	// none, opengles 2.0
	struct FramebufferStrategy {
		/// create a new framebuffer, depth_stencil and texture
		/**
		 * @param[out] framebuffer   Framebuffer name
		 * @param[out] depth         Name for packed depth buffer
		 * @param[out] stencil       Name for packed stencil buffer
		 * @param[out] img           Texture name
		 * @param[in]  width         Width of framebuffer
		 * @param[in]  height        Height of framebuffer
		 * @return Creation status
		 */
		virtual GLenum createFBO(GLuint&, GLuint&, GLuint&, GLuint&, int, int)
		{ return GL_FRAMEBUFFER_UNSUPPORTED; }
		/// remove objects
		/**
		 * @param[in] framebuffer   Framebuffer name
		 * @param[in] depth         Name for packed depth buffer
		 * @param[in] stencil       Name for packed stencil buffer
		 * @param[in] img           Texture name
		 */
		virtual void deleteFBO(GLuint, GLuint, GLuint, GLuint) {}
		virtual void bindFBO(GLuint) {}
	};

	struct FramebufferStrategyGLES2 : public FramebufferStrategy {
		virtual GLenum createFBO(GLuint& framebuffer, GLuint& depth, GLuint& stencil,  GLuint& img, int width, int height)
		{
			// get currently bound fbo to reset to it later
			GLint current_fbo;
			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &current_fbo);

			// create framebuffer
			glGenFramebuffers(1, &framebuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

			// create stencil buffer
			glGenRenderbuffers(1, &depth);
			glBindRenderbuffer(GL_RENDERBUFFER, depth);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
					GL_RENDERBUFFER, depth);
			
			glGenRenderbuffers(1, &stencil);
			glBindRenderbuffer(GL_RENDERBUFFER, stencil);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
					GL_RENDERBUFFER, stencil);

			// generate texture save target
			glGenTextures(1, &img);
			bindTexture(img);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8_OES, width, height,
					0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			bindTexture(0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
					GL_TEXTURE_2D, img, 0);

			// check status
			GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

			// unbind framebuffer
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, (GLuint)current_fbo);
			return status;
		}
		virtual void deleteFBO(GLuint framebuffer, GLuint depth, GLuint stencil,  GLuint img)
		{
			deleteTexture(img);
			glDeleteRenderbuffers(1, &depth);
			glDeleteRenderbuffers(1, &stencil);
			glDeleteFramebuffers(1, &framebuffer);
		}

		virtual void bindFBO(GLuint framebuffer)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		}
	};

	FramebufferStrategy* strategy = NULL;

	FramebufferStrategy strategyNone;

	FramebufferStrategyGLES2 strategyGLES2;

	Canvas* Canvas::current = NULL;

	static void loadStrategy()
	{
		if (!strategy)
		{
			if (GL_ES_VERSION_2_0)
				strategy = &strategyGLES2;
			else
				strategy = &strategyNone;
		}
	}

	Canvas::Canvas(int width, int height, std::stack<love::Matrix*> &projMatrix, std::stack<love::Matrix*> &modelViewMatrix, float *curColor, PixelEffect *primitivesEffect)
		: width(width), height(height), projMatrix(projMatrix)
		, modelViewMatrix(modelViewMatrix)
		, curColor(curColor)
		, primitivesEffect(primitivesEffect)
	{
		float w = static_cast<float>(width);
		float h = static_cast<float>(height);

		// world coordinates
		vertices[0].x = 0;     vertices[0].y = 0;
		vertices[1].x = 0;     vertices[1].y = h;
		vertices[2].x = w; vertices[2].y = h;
		vertices[3].x = w; vertices[3].y = 0;

		// texture coordinates
		vertices[0].s = 0;     vertices[0].t = 1;
		vertices[1].s = 0;     vertices[1].t = 0;
		vertices[2].s = 1;     vertices[2].t = 0;
		vertices[3].s = 1;     vertices[3].t = 1;

		loadStrategy();

		loadVolatile();
	}

	Canvas::~Canvas()
	{
		// reset framebuffer if still using this one
		if (current == this)
			stopGrab();

		unloadVolatile();
	}

	bool Canvas::isSupported()
	{
		loadStrategy();
		return (strategy != &strategyNone);
	}

	void Canvas::bindDefaultCanvas()
	{
		if (current != NULL)
			current->stopGrab();
	}

	void Canvas::startGrab()
	{
		// already grabbing
		if (current == this)
			return;

		// cleanup after previous fbo
		if (current != NULL)
			current->stopGrab();

		// bind buffer and clear screen
		strategy->bindFBO(fbo);
		glViewport(0, 0, width, height);

		// Reset the projection matrix
		projMatrix.push(new love::Matrix());

		// Set up orthographic view (no depth)
		projMatrix.top()->ortho(0.0, width, height, 0.0, -1.0, 1.0);

		// indicate we are using this fbo
		current = this;
	}

	void Canvas::stopGrab()
	{
		// i am not grabbing. leave me alone
		if (current != this)
			return;

		// bind default
		strategy->bindFBO( 0 );
		delete projMatrix.top();
		projMatrix.pop();
		current = NULL;
	}


	void Canvas::clear(const Color& c)
	{
		GLuint previous = 0;
		if (current != NULL)
			previous = current->fbo;

		strategy->bindFBO(fbo);
		float oldColor[4];
		glGetFloatv(GL_COLOR_CLEAR_VALUE, oldColor);
		
		glClearColor((float)c.r/255.0f, (float)c.g/255.0f, (float)c.b/255.0f, (float)c.a/255.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearColor(oldColor[0], oldColor[1], oldColor[2], oldColor[3]);

		strategy->bindFBO(previous);
	}

	void Canvas::draw(float x, float y, float angle, float sx, float sy, float ox, float oy, float kx, float ky) const
	{
		static Matrix t;
		t.setTransformation(x, y, angle, sx, sy, ox, oy, kx, ky);

		drawv(t, vertices);
	}

	void Canvas::drawq(love::graphics::Quad * quad, float x, float y, float angle, float sx, float sy, float ox, float oy, float kx, float ky) const
	{
		static Matrix t;
		quad->mirror(false, true);
		const vertex * v = quad->getVertices();

		t.setTransformation(x, y, angle, sx, sy, ox, oy, kx, ky);
		drawv(t, v);
		quad->mirror(false, true);
	}

	love::image::ImageData * Canvas::getImageData(love::image::Image * image)
	{
		int row = 4 * width;
		int size = row * height;
		GLubyte* pixels = new GLubyte[size];
		GLubyte* screenshot = new GLubyte[size];

		strategy->bindFBO( fbo );
		glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		if (current)
			strategy->bindFBO( current->fbo );
		else
			strategy->bindFBO( 0 );

		GLubyte * src = pixels - row; // second line of source image
		GLubyte * dst = screenshot + size; // last row of destination image

		for (int i = 0; i < height; ++i)
			memcpy(dst -= row, src += row, row);

		love::image::ImageData* img = image->newImageData(width, height, (void*)screenshot);

		delete[] screenshot;
		delete[] pixels;

		return img;
	}

	void Canvas::setFilter(const Image::Filter &f)
	{
		GLint gmin = (f.min == Image::FILTER_NEAREST) ? GL_NEAREST : GL_LINEAR;
		GLint gmag = (f.mag == Image::FILTER_NEAREST) ? GL_NEAREST : GL_LINEAR;

		bindTexture(img);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gmin);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gmag);
	}

	Image::Filter Canvas::getFilter() const
	{
		GLint gmin, gmag;

		bindTexture(img);
		glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, &gmin);
		glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, &gmag);

		Image::Filter f;
		f.min = (gmin == GL_NEAREST) ? Image::FILTER_NEAREST : Image::FILTER_LINEAR;
		f.mag = (gmag == GL_NEAREST) ? Image::FILTER_NEAREST : Image::FILTER_LINEAR;
		return f;
	}

	void Canvas::setWrap(const Image::Wrap &w)
	{
		GLint wrap_s = (w.s == Image::WRAP_CLAMP) ? GL_CLAMP_TO_EDGE : GL_REPEAT;
		GLint wrap_t = (w.t == Image::WRAP_CLAMP) ? GL_CLAMP_TO_EDGE : GL_REPEAT;

		bindTexture(img);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
	}

	Image::Wrap Canvas::getWrap() const
	{
		GLint wrap_s, wrap_t;
		bindTexture(img);
		glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, &wrap_s);
		glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, &wrap_t);

		Image::Wrap w;
		w.s = (wrap_s == GL_CLAMP_TO_EDGE) ? Image::WRAP_CLAMP : Image::WRAP_REPEAT;
		w.t = (wrap_t == GL_CLAMP_TO_EDGE) ? Image::WRAP_CLAMP : Image::WRAP_REPEAT;

		return w;
	}

	bool Canvas::loadVolatile()
	{
		status = strategy->createFBO(fbo, depth, stencil, img, width, height);
		if (status != GL_FRAMEBUFFER_COMPLETE)
			return false;

		setFilter(settings.filter);
		setWrap(settings.wrap);
		Color c;
		c.r = c.g = c.b = c.a = 0;
		clear(c);
		return true;
	}

	void Canvas::unloadVolatile()
	{
		settings.filter = getFilter();
		settings.wrap   = getWrap();
		strategy->deleteFBO(fbo, depth, stencil, img);
	}

	int Canvas::getWidth()
	{
		return width;
	}

	int Canvas::getHeight()
	{
		return height;
	}

	void Canvas::drawv(const Matrix & t, const vertex * v) const
	{
		modelViewMatrix.push(new love::Matrix(*modelViewMatrix.top()));
		*modelViewMatrix.top() *= t;

		bindTexture(img);

		bool useStdShader = false;
		if(PixelEffect::current == NULL)
		{
			useStdShader = true;
			primitivesEffect->attach();
		}

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)&v[0].x);
		glVertexAttrib4fv(1, curColor);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)&v[0].s);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(2);

		PixelEffect::current->bindAttribLocation("position", 0);
		PixelEffect::current->bindAttribLocation("colour", 1);
		PixelEffect::current->bindAttribLocation("texCoord", 2);
		
		Matrix mvp = *modelViewMatrix.top() * *projMatrix.top();
		PixelEffect::current->sendMatrix("mvp", 4, mvp.getElements(), 1);

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(2);

		if(useStdShader == true)
		  primitivesEffect->detach();

		delete modelViewMatrix.top();
		modelViewMatrix.pop();
	}

} // opengl
} // graphics
} // love
