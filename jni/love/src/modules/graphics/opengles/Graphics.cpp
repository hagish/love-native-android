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

#include <common/config.h>
#include <common/math.h>
#include <common/Vector.h>

#include "Graphics.h"
#include <window/android/Window.h>

#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>

extern int gScreenWidth;
extern int gScreenHeight;

namespace love
{
namespace graphics
{
namespace opengles
{

	Graphics::Graphics()
		: currentFont(0), currentImageFilter(), lineStyle(LINE_SMOOTH), lineWidth(1), matrixLimit(0), userMatrices(0)
	{
		currentWindow = love::window::android::Window::getSingleton();

		modelViewMatrix.push(new love::Matrix);
		projectionMatrix.push(new love::Matrix);
		resetBoundTexture();
		
		const char *effect = 
			"precision mediump float;\n"
			"#define number float\n"
			"#define Image sampler2D\n"
			"#define extern uniform\n"
			"#define Texel texture2D\n"
			"varying vec4 vColour;\n"
			"varying vec2 vTexCoord;\n"
			"varying vec4 vPosition;\n"
			"uniform sampler2D _tex0_;\n"
			"vec4 effect(vec4 color, Image texture, vec2 texture_coords, vec2 pixel_coords)\n"
			"{\n"
			"	return color;\n"
			"}\n"
			"void main() {\n"
			"	gl_FragColor = effect(vColour, _tex0_, vTexCoord, vPosition.xy);\n"
			"}\n";
		PixelEffect *primitivesEffect = newPixelEffect(effect);
	}

	Graphics::~Graphics()
	{
		delete primitivesEffect;
		
		if (currentFont != 0)
			currentFont->release();

		while(!modelViewMatrix.empty())
		{
			delete modelViewMatrix.top();
			modelViewMatrix.pop();
		}

		while(!projectionMatrix.empty())
		{
			delete projectionMatrix.top();
			projectionMatrix.pop();
		}

		currentWindow->release();
	}

	const char * Graphics::getName() const
	{
		return "love.graphics.opengles";
	}

	bool Graphics::checkMode(int width, int height, bool fullscreen)
	{
		return currentWindow->checkWindowSize(width, height, fullscreen);
	}

	DisplayState Graphics::saveState()
	{
		DisplayState s;

		s.color = getColor();
		s.backgroundColor = getBackgroundColor();
		
		s.blendMode = getBlendMode();
		s.colorMode = getColorMode();
		//get line style
		s.lineStyle = lineStyle;
		//get the point size
		s.pointSize = 1;
		//get point style
		s.pointStyle = Graphics::POINT_ROUGH;
		//get scissor status
		s.scissor = (glIsEnabled(GL_SCISSOR_TEST) == GL_TRUE);
		//do we have scissor, if so, store the box
		if (s.scissor)
			glGetIntegerv(GL_SCISSOR_BOX, s.scissorBox);

		return s;
	}

	void Graphics::restoreState(const DisplayState & s)
	{
		setColor(s.color);
		setBackgroundColor(s.backgroundColor);
		setBlendMode(s.blendMode);
		setColorMode(s.colorMode);
		setLine(lineWidth, s.lineStyle);
		setPoint(s.pointSize, s.pointStyle);
		if (s.scissor)
			setScissor(s.scissorBox[0], s.scissorBox[1], s.scissorBox[2], s.scissorBox[3]);
		else
			setScissor();
	}

	bool Graphics::setMode(int width, int height, bool fullscreen, bool vsync, int fsaa)
	{
		// This operation destroys the OpenGL context, so
		// we must save the state.
		DisplayState tempState;
		if (isCreated())
			tempState = saveState();

		// Unlad all volatile objects. These must be reloaded after
		// the display mode change.
		Volatile::unloadAll();

		currentWindow->setWindow(width, height, fullscreen, vsync, fsaa);

		// Okay, setup OpenGL.

		// Enable blending
		glEnable(GL_BLEND);

		// "Normal" blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		currentBlendMode = Graphics::BLEND_ALPHA;

		// Enable line/point smoothing.
		// not supported, but we want to be compatible to normal GL
		setLineStyle(LINE_SMOOTH);

		// Enable textures
		glEnable(GL_TEXTURE_2D);

		// Set the viewport to top-left corner
		glViewport(0,0, width, height);

		// Reset the projection matrix
		projectionMatrix.top()->setIdentity();

		// Set up orthographic view (no depth)
		projectionMatrix.top()->ortho(0.0, width, height,0.0, -1.0, 1.0);

		// Reset modelview matrix
		modelViewMatrix.top()->setIdentity();

		// Set pixel row alignment
		glPixelStorei(GL_UNPACK_ALIGNMENT, 2);

		// Reload all volatile objects.
		if (!Volatile::loadAll())
			std::cerr << "Could not reload all volatile objects." << std::endl;

		// Restore the display state.
		restoreState(tempState);

		// Get the maximum number of matrices
		// actually, we are limited to memory size only since
		// we have all matrix handling on CPU side
		matrixLimit = 100;

		return true;
	}

	void Graphics::saveSettings(void)
	{
		// This operation destroys the OpenGL context, so
		// we must save the state.
		if (isCreated())
			storedDisplayState = saveState();

		// Unlad all volatile objects. These must be reloaded after
		// the display mode change.
		Volatile::unloadAll();
	}

	void Graphics::restoreSettings(void)
	{
		// Enable blending
		glEnable(GL_BLEND);

		// "Normal" blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		currentBlendMode = Graphics::BLEND_ALPHA;

		// Enable line/point smoothing.
		// not supported, but we want to be compatible to normal GL
		setLineStyle(LINE_SMOOTH);

		// Enable textures
		glEnable(GL_TEXTURE_2D);

		// Set the viewport to top-left corner
		glViewport(0,0, gScreenWidth, gScreenHeight);

		// Reset the projection matrix
		projectionMatrix.top()->setIdentity();

		// Set up orthographic view (no depth)
		projectionMatrix.top()->ortho(0.0, gScreenWidth, gScreenHeight,0.0, -1.0, 1.0);

		// Reset modelview matrix
		modelViewMatrix.top()->setIdentity();

		// Set pixel row alignment
		glPixelStorei(GL_UNPACK_ALIGNMENT, 2);

		// Reload all volatile objects.
		if (!Volatile::loadAll())
			std::cerr << "Could not reload all volatile objects." << std::endl;

		// Restore the display state.
		restoreState(storedDisplayState);
	}
	
	void Graphics::getMode(int &width, int &height, bool &fullscreen, bool &vsync, int &fsaa)
	{
		currentWindow->getWindow(width, height, fullscreen, vsync, fsaa);
	}

	bool Graphics::toggleFullscreen()
	{
		int width, height, fsaa;
		bool fullscreen, vsync;
		currentWindow->getWindow(width, height, fullscreen, vsync, fsaa);
		return setMode(width, height, !fullscreen, vsync, fsaa);
	}


	void Graphics::reset()
	{
		DisplayState s;
		discardStencil();
		Canvas::bindDefaultCanvas();
		PixelEffect::detach();
		restoreState(s);
	}

	void Graphics::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		modelViewMatrix.top()->setIdentity();
	}

	void Graphics::present()
	{
		currentWindow->swapBuffers();
	}

	void Graphics::setIcon(Image * image)
	{
		currentWindow->setIcon(image->getData());
	}

	void Graphics::setCaption(const char * caption)
	{
		std::string title(caption);
		currentWindow->setWindowTitle(title);
	}

	int Graphics::getCaption(lua_State * L)
	{
		std::string title = currentWindow->getWindowTitle();
		lua_pushstring(L, title.c_str());
		return 1;
	}

	int Graphics::getWidth()
	{
		return currentWindow->getWidth();
	}

	int Graphics::getHeight()
	{
		return currentWindow->getHeight();
	}

	int Graphics::getRenderHeight()
	{
		if (Canvas::current)
			return Canvas::current->getHeight();
		return getHeight();
	}

	bool Graphics::isCreated()
	{
		return currentWindow->isCreated();
	}

	int Graphics::getModes(lua_State * L)
	{
		int n;
		love::window::Window::WindowSize ** modes = currentWindow->getFullscreenSizes(n);

		if (modes == 0)
			return 0;

		lua_newtable(L);

		for (int i = 0; i < n ; i++)
		{
			lua_pushinteger(L, i+1);
			lua_newtable(L);

			// Inner table attribs.

			lua_pushstring(L, "width");
			lua_pushinteger(L, modes[i]->width);
			lua_settable(L, -3);

			lua_pushstring(L, "height");
			lua_pushinteger(L, modes[i]->height);
			lua_settable(L, -3);

			// Inner table attribs end.

			lua_settable(L, -3);

			delete modes[i];
		}

		delete[] modes;
		return 1;
	}

	void Graphics::setScissor(int x, int y, int width, int height)
	{
		glEnable(GL_SCISSOR_TEST);
		glScissor(x, getRenderHeight() - (y + height), width, height); // Compensates for the fact that our y-coordinate is reverse of OpenGLs.
	}

	void Graphics::setScissor()
	{
		glDisable(GL_SCISSOR_TEST);
	}

	int Graphics::getScissor(lua_State * L)
	{
		if (glIsEnabled(GL_SCISSOR_TEST) == GL_FALSE)
			return 0;

		GLint scissor[4];
		glGetIntegerv(GL_SCISSOR_BOX, scissor);

		lua_pushnumber(L, scissor[0]);
		lua_pushnumber(L, getRenderHeight() - (scissor[1] + scissor[3])); // Compensates for the fact that our y-coordinate is reverse of OpenGLs.
		lua_pushnumber(L, scissor[2]);
		lua_pushnumber(L, scissor[3]);

		return 4;
	}

	void Graphics::defineStencil()
	{
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glEnable(GL_STENCIL_TEST);
		glClear(GL_STENCIL_BUFFER_BIT);
		glStencilFunc(GL_ALWAYS, 1, 1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	}

	void Graphics::useStencil(bool invert)
	{
		glStencilFunc(GL_EQUAL, (int)(!invert), 1); // invert ? 0 : 1
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	}

	void Graphics::discardStencil()
	{
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glDisable(GL_STENCIL_TEST);
	}

	Image * Graphics::newImage(love::image::ImageData * data)
	{
		// Create the image.
		Image * image = new Image(data, projectionMatrix, modelViewMatrix, currentColour, primitivesEffect);
		bool success;
		try
		{
			success = image->load();
		}
		catch (love::Exception & e)
		{
			image->release();
			throw love::Exception(e.what());
		}
		if (!success)
		{
			image->release();
			return 0;
		}

		image->setFilter(currentImageFilter);

		return image;
	}

	Quad * Graphics::newQuad(float x, float y, float w, float h, float sw, float sh)
	{
		Quad::Viewport v;
		v.x = x;
		v.y = y;
		v.w = w;
		v.h = h;
		return new Quad(v, sw, sh);
	}

	Font * Graphics::newFont(love::font::Rasterizer * r, const Image::Filter& filter)
	{
		Font * font = new Font(r, projectionMatrix, modelViewMatrix, currentColour, primitivesEffect, filter);

		// Load it and check for errors.
		if (!font)
		{
			delete font;
			return 0;
		}

		return font;
	}

	SpriteBatch * Graphics::newSpriteBatch(Image * image, int size, int usage)
	{
		SpriteBatch * t = NULL;
		try
		{
			t = new SpriteBatch(image, size, usage, projectionMatrix, modelViewMatrix, currentColour, primitivesEffect);
		}
		catch (love::Exception& e)
		{
			if (t) delete t;
			throw e;
		}
		return t;
	}

	ParticleSystem * Graphics::newParticleSystem(Image * image, int size)
	{
		return new ParticleSystem(image, size, projectionMatrix, modelViewMatrix, currentColour, primitivesEffect);
	}

	Canvas * Graphics::newCanvas(int width, int height)
	{
		Canvas * canvas = new Canvas(width, height, projectionMatrix, modelViewMatrix, currentColour, primitivesEffect);
		GLenum err = canvas->getStatus();

		// everything ok, reaturn canvas (early out)
		if (err == GL_FRAMEBUFFER_COMPLETE)
			return canvas;

		// create error message
		std::stringstream error_string;
		error_string << "Cannot create canvas: ";
		switch (err) {

			case GL_FRAMEBUFFER_UNSUPPORTED:
				error_string << "Not supported by your OpenGL implementation.";
				break;

			// remaining error codes are highly unlikely:
			case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
				error_string << "Error in implementation. Possible fix: Make canvas width and height powers of two.";
				break;

			default:
				// my intel hda card wrongly returns 0 to glCheckFramebufferStatus() but sets
				// no error flag. I think it meant to return GL_FRAMEBUFFER_UNSUPPORTED, but who
				// knows.
				if (glGetError() == GL_NO_ERROR)
					error_string << "May not be supported by your OpenGL implementation.";
				// the remaining error is an indication of a serious fuckup since it should
				// only be returned if glCheckFramebufferStatus() was called with the wrong
				// arguments.
				else
					error_string << "Cannot create canvas: Aliens did it (OpenGL error code: " << glGetError() << ")";
		}

		canvas->release();
		throw Exception(error_string.str().c_str());
		return NULL; // never reached
	}

	PixelEffect * Graphics::newPixelEffect(const std::string& code)
	{
		PixelEffect * effect = NULL;
		try
		{
			const char *vertex =
				"attribute vec2 position;\n"
				"attribute vec4 colour;\n"
				"attribute vec2 texCoord;\n"
				"varying vec4 vColour;\n"
				"varying vec2 vTexCoord;\n"
				"varying vec4 vPosition;\n"
				"uniform mat4 mvp;\n"
				"void main()\n"
				"{\n"
				"	vColour = colour;\n"
				"	vTexCoord = texCoord;\n"
				"	vPosition = mvp * vec4(position, 0.0, 1.0);\n"
				"	gl_Position = vPosition;\n"
				"}\n";
			effect = new PixelEffect(vertex, code);
		}
		catch (love::Exception& e)
		{
			if (effect)
				delete effect;
			throw(e);
		}
		return effect;
	}

	void Graphics::setColor(const Color& c)
	{
		currentColour[0] = c.r / 255.f;
		currentColour[1] = c.g / 255.f;
		currentColour[2] = c.b / 255.f;
		currentColour[3] = c.a / 255.f;
	}

	Color Graphics::getColor()
	{
		Color t;
		t.r = (unsigned char)(255.0f*currentColour[0]);
		t.g = (unsigned char)(255.0f*currentColour[1]);
		t.b = (unsigned char)(255.0f*currentColour[2]);
		t.a = (unsigned char)(255.0f*currentColour[3]);

		return t;
	}

	void Graphics::setBackgroundColor(const Color& c)
	{
		glClearColor((float)c.r/255.0f, (float)c.g/255.0f, (float)c.b/255.0f, (float)c.a/255.0f);
	}

	Color Graphics::getBackgroundColor()
	{
		float c[4];
		glGetFloatv(GL_COLOR_CLEAR_VALUE, c);

		Color t;
		t.r = (unsigned char)(255.0f*c[0]);
		t.g = (unsigned char)(255.0f*c[1]);
		t.b = (unsigned char)(255.0f*c[2]);
		t.a = (unsigned char)(255.0f*c[3]);

		return t;
	}

	void Graphics::setFont( Font * font )
	{
		if (currentFont != 0)
			currentFont->release();

		currentFont = font;

		if (font != 0)
			currentFont->retain();
	}

	Font * Graphics::getFont()
	{
		return currentFont;
	}

	void Graphics::setBlendMode( Graphics::BlendMode mode )
	{
		if (mode == BLEND_SUBTRACTIVE)
			glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
		else
			glBlendEquation(GL_FUNC_ADD);


		if (mode == BLEND_ALPHA)
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		else if (mode == BLEND_MULTIPLICATIVE)
			glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
		else if (mode == BLEND_PREMULTIPLIED)
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		else // mode == BLEND_ADDITIVE || mode == BLEND_SUBTRACTIVE
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		currentBlendMode = mode;
	}

	void Graphics::setColorMode ( Graphics::ColorMode mode )
	{
		// not supported in ES
		// consider some fency shaders - but this will have performance hits...
		if (mode == COLOR_MODULATE)
		{
		}
		else if (mode == COLOR_COMBINE) 
		{
		}
		else // mode = COLOR_REPLACE
		{
		}
		currentColourMode = mode;
	}

	void Graphics::setDefaultImageFilter(const Image::Filter& f)
	{
		currentImageFilter = f;
	}

	Graphics::BlendMode Graphics::getBlendMode ()
	{
		return currentBlendMode;
	}

	Graphics::ColorMode Graphics::getColorMode()
	{
		return currentColourMode;
	}

	const Image::Filter& Graphics::getDefaultImageFilter() const
	{
		return currentImageFilter;
	}

	void Graphics::setLineWidth( float width )
	{
		lineWidth = width;
	}

	void Graphics::setLineStyle(Graphics::LineStyle style)
	{
		lineStyle = style;
	}

	void Graphics::setLine( float width, Graphics::LineStyle style )
	{
		setLineWidth(width);

		if (style == 0)
			return;
		setLineStyle(style);
	}

	float Graphics::getLineWidth()
	{
		float w;
		glGetFloatv(GL_LINE_WIDTH, &w);
		return w;
	}

	Graphics::LineStyle Graphics::getLineStyle()
	{
		return lineStyle;
	}

	void Graphics::setPointSize( float size )
	{
		// not suppoted
	}

	void Graphics::setPointStyle( Graphics::PointStyle style )
	{
		// not supported
	}

	void Graphics::setPoint( float size, Graphics::PointStyle style )
	{
		setPointSize(size);
		setPointStyle(style);
	}

	float Graphics::getPointSize()
	{
		// not supported => set a default value
		return 1.f;
	}

	Graphics::PointStyle Graphics::getPointStyle()
	{
		// not supported => set a default value
		return POINT_SMOOTH;
	}

	int Graphics::getMaxPointSize()
	{
		// not supported => set a default value
		return 1;
	}

	void Graphics::print( const char * str, float x, float y , float angle, float sx, float sy, float ox, float oy, float kx, float ky)
	{
		if (currentFont != 0)
		{
			std::string text(str);
			currentFont->print(text, x, y, angle, sx, sy, ox, oy, kx, ky);
		}
	}

	void Graphics::printf( const char * str, float x, float y, float wrap, AlignMode align)
	{
		if (currentFont == 0)
			return;

		using namespace std;
		string text(str);
		vector<string> lines_to_draw = currentFont->getWrap(text, wrap);

		// now for the actual printing
		vector<string>::const_iterator line_iter, line_end = lines_to_draw.end();
		for (line_iter = lines_to_draw.begin(); line_iter != line_end; ++line_iter)
		{
			float width = static_cast<float>(currentFont->getWidth( *line_iter ));
			switch (align) {
				case ALIGN_RIGHT:
					currentFont->print(*line_iter, ceil(x + wrap - width), ceil(y));
					break;
				case ALIGN_CENTER:
					currentFont->print(*line_iter, ceil(x + (wrap - width) / 2), ceil(y));
					break;
				case ALIGN_LEFT:
				default:
					currentFont->print(*line_iter, ceil(x), ceil(y));
					break;
			}
			y += currentFont->getHeight() * currentFont->getLineHeight();
		}
	}

	/**
	* Primitives
	**/

	void Graphics::point( float x, float y )
	{
		bool useStdShader = false;
		if(PixelEffect::current == NULL)
		{
			useStdShader = true;
			primitivesEffect->attach();
		}
		float data[8] = {
			x, y, // position
			currentColour[0], currentColour[1], currentColour[2], currentColour[3], // colour
			0, 0, // tex coord
		};
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), data);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), data + 2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), data + 6);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		PixelEffect::current->bindAttribLocation("position", 0);
		PixelEffect::current->bindAttribLocation("colour", 1);
		PixelEffect::current->bindAttribLocation("texCoord", 2);
		
		Matrix mvp = *modelViewMatrix.top() * *projectionMatrix.top();
		PixelEffect::current->sendMatrix("mvp", 4, mvp.getElements(), 1);

		glDrawArrays(GL_POINTS, 0, 1);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		
		if(useStdShader == true)
			primitivesEffect->detach();
	}

	// Calculate line boundary points u1 and u2. Sketch:
	//              u1
	// -------------+---...___
	//              |         ```'''--  ---
	// p- - - - - - q- - . _ _           | w/2
	//              |          ` ' ' r   +
	// -------------+---...___           | w/2
	//              u2         ```'''-- ---
	//
	// u1 and u2 depend on four things:
	//   - the half line width w/2
	//   - the previous line vertex p
	//   - the current line vertex q
	//   - the next line vertex r
	//
	// u1/u2 are the intersection points of the parallel lines to p-q and q-r,
	// i.e. the point where
	//
	//    (p + w/2 * n1) + mu * (q - p) = (q + w/2 * n2) + lambda * (r - q)   (u1)
	//    (p - w/2 * n1) + mu * (q - p) = (q - w/2 * n2) + lambda * (r - q)   (u2)
	//
	// with n1,n2 being the normals on the segments p-q and q-r:
	//
	//    n1 = perp(q - p) / |q - p|
	//    n2 = perp(r - q) / |r - q|
	//
	// The intersection points can be calculated using cramers rule.
	static void pushIntersectionPoints(Vector *vertices, Vector* overdraw,
			int pos, int count, float hw, float inv_hw,
			const Vector& p, const Vector& q, const Vector& r)
	{
		// calculate line directions
		Vector s = (q - p);
		Vector t = (r - q);

		// calculate vertex displacement vectors
		Vector n1 = s.getNormal();
		Vector n2 = t.getNormal();
		n1.normalize();
		n2.normalize();
		float det_norm = n1 ^ n2; // will be close to zero if the angle between the normals is sharp
		n1 *= hw;
		n2 *= hw;

		// lines parallel -> assume intersection at displacement points
		if (fabs(det_norm) <= .03)
		{
			vertices[pos]   = q - n2;
			vertices[pos+1] = q + n2;
		}
		// real intersection -> calculate boundary intersection points with cramers rule
		else
		{
			float det = s ^ t;
			Vector d = n1 - n2;
			Vector b = s - d; // s = q - p
			Vector c = s + d;
			float lambda = (b ^ t) / det;
			float mu     = (c ^ t) / det;

			// ordering for GL_TRIANGLE_STRIP
			vertices[pos]   = p + s*mu - n1;     // u1
			vertices[pos+1] = p + s*lambda + n1; // u2
		}

		if (overdraw)
		{
			// displacement of the overdraw vertices (works by magic).
			Vector x = (vertices[pos] - q) * inv_hw;

			overdraw[pos]   = vertices[pos];
			overdraw[pos+1] = vertices[pos] + x;

			overdraw[2*count-pos-2] = vertices[pos+1];
			overdraw[2*count-pos-1] = vertices[pos+1] - x;
		}
	}

	// precondition:
	// glEnableClientState(GL_VERTEX_ARRAY);
	static void draw_overdraw(Vector* overdraw, size_t count, bool looping, float *c)
	{
		// if not looping, the outer overdraw vertices need to be displaced
		// to cover the line endings, i.e.:
		// +- - - - //- - +         +- - - - - //- - - +
		// +-------//-----+         : +-------//-----+ :
		// | core // line |   -->   : | core // line | :
		// +-----//-------+         : +-----//-------+ :
		// +- - //- - - - +         +- - - //- - - - - +
		if (!looping)
		{
			Vector s = overdraw[1] - overdraw[3];
			s.normalize();
			overdraw[1] += s;
			overdraw[2*count-1] += s;

			Vector t = overdraw[count-1] - overdraw[count-3];
			t.normalize();
			overdraw[count-1] += t;
			overdraw[count+1] += t;

			// we need to draw two more triangles to close the
			// overdraw at the line start.
			overdraw[2*count]   = overdraw[0];
			overdraw[2*count+1] = overdraw[1];
		}

		// prepare colors:
		// even indices in overdraw* point to inner vertices => alpha = current-alpha,
		// odd indices point to outer vertices => alpha = 0.
		Color *colors = new Color[2*count+2];
		for (size_t i = 0; i < 2*count+2; ++i)
		{
			colors[i] = Color(GLubyte(c[0] * 255.f),
					GLubyte(c[1] * 255.f),
					GLubyte(c[2] * 255.f),
					// avoids branching. equiv to if (i%2 == 1) colors[i].a = 0;
					GLubyte(c[3] * 255.f) * GLubyte(i%2 == 0));
		}

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, overdraw);
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, colors);
		glVertexAttrib2f(2, 0.f, 0.f);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		PixelEffect::current->bindAttribLocation("position", 0);
		PixelEffect::current->bindAttribLocation("colour", 1);
		PixelEffect::current->bindAttribLocation("texCoord", 2);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 2*count + 2 * int(!looping));

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		delete[] colors;
	}

	void Graphics::polyline(const float* coords, size_t count)
	{
		Vector *vertices = new Vector[count]; // two vertices for every line end-point
		Vector *overdraw = NULL;

		Vector p,q,r;
		bool looping = (coords[0] == coords[count-2]) && (coords[1] == coords[count-1]);

		float halfwidth = lineWidth/2.f;
		float inv_hw    = 1.f / halfwidth;

		if (lineStyle == LINE_SMOOTH) {
			overdraw = new Vector[2*count+2];
			// Overdraw changes visible line width. account for that.
			// Value of 0.2 chosen empirically.
			halfwidth -= .2f;
		}

		// get line vertex boundaries
		// if not looping, extend the line at the beginning, else use last point as `p'
		r = Vector(coords[0], coords[1]);
		if (!looping)
			q = r * 2 - Vector(coords[2], coords[3]);
		else
			q = Vector(coords[count-4], coords[count-3]);

		for (size_t i = 0; i+3 < count; i += 2)
		{
			p = q; q = r;
			r = Vector(coords[i+2], coords[i+3]);
			pushIntersectionPoints(vertices, overdraw, i, count, halfwidth, inv_hw, p,q,r);
		}

		// if not looping, extend the line at the end, else use first point as `r'
		p = q; q = r;
		if (!looping)
			r += q - p;
		else
			r = Vector(coords[2], coords[3]);
		pushIntersectionPoints(vertices, overdraw, count-2, count, halfwidth, inv_hw, p,q,r);
		// end get line vertex boundaries

		// draw the core line
		bool useStdShader = false;
		if(PixelEffect::current == NULL)
		{
			useStdShader = true;
			primitivesEffect->attach();
		}
		
		Matrix mvp = *modelViewMatrix.top() * *projectionMatrix.top();
		PixelEffect::current->sendMatrix("mvp", 4, mvp.getElements(), 1);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vertices);
		glVertexAttrib4fv(1, currentColour);
		glVertexAttrib2f(2, 0.f, 0.f);

		glEnableVertexAttribArray(0);

		PixelEffect::current->bindAttribLocation("position", 0);
		PixelEffect::current->bindAttribLocation("colour", 1);
		PixelEffect::current->bindAttribLocation("texCoord", 2);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, count);

		glDisableVertexAttribArray(0);

		// draw the line halo (antialiasing)
		if (lineStyle == LINE_SMOOTH)
			draw_overdraw(overdraw, count, looping, currentColour);

		if(useStdShader == true)
			primitivesEffect->detach();

		// cleanup
		delete[] vertices;
		if (lineStyle == LINE_SMOOTH)
			delete[] overdraw;
	}

	void Graphics::triangle(DrawMode mode, float x1, float y1, float x2, float y2, float x3, float y3 )
	{
		float coords[] = { x1,y1, x2,y2, x3,y3, x1,y1 };
		polygon(mode, coords, 4 * 2);
	}

	void Graphics::rectangle(DrawMode mode, float x, float y, float w, float h)
	{
		quad(mode, x,y, x,y+h, x+w,y+h, x+w,y);
	}

	void Graphics::quad(DrawMode mode, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4 )
	{
		float coords[] = { x1,y1, x2,y2, x3,y3, x4,y4, x1,y1 };
		polygon(mode, coords, 5 * 2);
	}

	void Graphics::circle(DrawMode mode, float x, float y, float radius, int points)
	{
		float two_pi = static_cast<float>(LOVE_M_PI * 2);
		if (points <= 0) points = 1;
		float angle_shift = (two_pi / points);
		float phi = .0f;

		float *coords = new float[2 * (points + 1)];
		for (int i = 0; i < points; ++i, phi += angle_shift)
		{
			coords[2*i]   = x + radius * cos(phi);
			coords[2*i+1] = y + radius * sin(phi);
		}

		coords[2*points]   = coords[0];
		coords[2*points+1] = coords[1];

		polygon(mode, coords, (points + 1) * 2);

		delete[] coords;
	}

	void Graphics::arc(DrawMode mode, float x, float y, float radius, float angle1, float angle2, int points)
	{
		// Nothing to display with no points or equal angles. (Or is there with line mode?)
		if (points <= 0 || angle1 == angle2)
			return;

		// Oh, you want to draw a circle?
		if (fabs(angle1 - angle2) >= 2.0f * (float) LOVE_M_PI)
		{
			circle(mode, x, y, radius, points);
			return;
		}

		float angle_shift = (angle2 - angle1) / points;
		// Bail on precision issues.
		if (angle_shift == 0.0)
			return;

		float phi = angle1;
		int num_coords = (points + 3) * 2;
		float * coords = new float[num_coords];
		coords[0] = coords[num_coords - 2] = x;
		coords[1] = coords[num_coords - 1] = y;

		for (int i = 0; i <= points; ++i, phi += angle_shift)
		{
			coords[2 * (i+1)]     = x + radius * cos(phi);
			coords[2 * (i+1) + 1] = y + radius * sin(phi);
		}

		// GL_POLYGON can only fill-draw convex polygons, so we need to do stuff manually here
		if (mode == DRAW_LINE)
		{
			polyline(coords, num_coords); // Artifacts at sharp angles if set to looping.
		}
		else
		{
			bool useStdShader = false;
			if(PixelEffect::current == NULL)
			{
				useStdShader = true;
				primitivesEffect->attach();
			}

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, coords);
			glVertexAttrib4fv(1, currentColour);
			glVertexAttrib2f(2, 0.f, 0.f);

			glEnableVertexAttribArray(0);

			PixelEffect::current->bindAttribLocation("position", 0);
			PixelEffect::current->bindAttribLocation("colour", 1);
			PixelEffect::current->bindAttribLocation("texCoord", 2);
			
			Matrix mvp = *modelViewMatrix.top() * *projectionMatrix.top();
			PixelEffect::current->sendMatrix("mvp", 4, mvp.getElements(), 1);

			glDrawArrays(GL_TRIANGLE_FAN, 0, points + 2);

			glDisableVertexAttribArray(0);

			if(useStdShader == true)
			  primitivesEffect->detach();
		}

		delete[] coords;
	}

	/// @param mode    the draw mode
	/// @param coords  the coordinate array
	/// @param count   the number of coordinates/size of the array
	void Graphics::polygon(DrawMode mode, const float* coords, size_t count)
	{
		// coords is an array of a closed loop of vertices, i.e.
		// coords[count-2] = coords[0], coords[count-1] = coords[1]
		if (mode == DRAW_LINE)
		{
			polyline(coords, count);
		}
		else
		{
			bool useStdShader = false;
			if(PixelEffect::current == NULL)
			{
				useStdShader = true;
				primitivesEffect->attach();
			}

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, coords);
			glVertexAttrib4fv(1, currentColour);
			glVertexAttrib2f(2, 0.f, 0.f);

			glEnableVertexAttribArray(0);

			PixelEffect::current->bindAttribLocation("position", 0);
			PixelEffect::current->bindAttribLocation("colour", 1);
			PixelEffect::current->bindAttribLocation("texCoord", 2);
			
			Matrix mvp = *modelViewMatrix.top() * *projectionMatrix.top();
			PixelEffect::current->sendMatrix("mvp", 4, mvp.getElements(), 1);

			glDrawArrays(GL_TRIANGLE_FAN, 0, count/2);

			glDisableVertexAttribArray(0);

			if(useStdShader == true)
			  primitivesEffect->detach();
		}
	}

	love::image::ImageData * Graphics::newScreenshot(love::image::Image * image)
	{
		int w = getWidth();
		int h = getHeight();

		int row = 4*w;

		int size = row*h;

		GLubyte * pixels = new GLubyte[size];
		GLubyte * screenshot = new GLubyte[size];

		glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		// OpenGL sucks and reads pixels from the lower-left. Let's fix that.

		GLubyte *src = pixels - row, *dst = screenshot + size;

		for (int i = 0; i < h; ++i)
		{
			memcpy(dst-=row, src+=row, row);
		}

		love::image::ImageData * img = image->newImageData(w, h, (void*)screenshot);

		delete [] pixels;
		delete [] screenshot;

		return img;
	}

	void Graphics::push()
	{
		if (userMatrices == matrixLimit)
			throw Exception("Maximum stack depth reached.");
		// currently, only model view matrix stack is manipulated
		modelViewMatrix.push(new love::Matrix(*modelViewMatrix.top()));
		++userMatrices;
	}

	void Graphics::pop()
	{
		if (userMatrices < 1)
			throw Exception("Minimum stack depth reached. (More pops than pushes?)");
		// currently, only model view matrix stack is manipulated
		delete modelViewMatrix.top();
		modelViewMatrix.pop();
		--userMatrices;
	}

	void Graphics::rotate(float r)
	{
		modelViewMatrix.top()->rotate(r);
	}

	void Graphics::scale(float x, float y)
	{
		modelViewMatrix.top()->scale(x, y);
	}

	void Graphics::translate(float x, float y)
	{
		modelViewMatrix.top()->translate(x, y);
	}

	void Graphics::shear(float kx, float ky)
	{
		modelViewMatrix.top()->shear(kx, ky);
	}

	void Graphics::drawTest(Image * image, float x, float y, float a, float sx, float sy, float ox, float oy)
	{
		image->bind();

		// Buffer for transforming the image.
		vertex buf[4];

		Matrix t;
		t.translate(x, y);
		t.rotate(a);
		t.scale(sx, sy);
		t.translate(ox, oy);
		t.transform(buf, image->getVertices(), 4);

		const vertex * vertices = image->getVertices();

		bool useStdShader = false;
		if(PixelEffect::current == NULL)
		{
			useStdShader = true;
			primitivesEffect->attach();
		}

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, &buf[0].x);
		glVertexAttrib4fv(1, currentColour);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, &vertices[0].s);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(2);

		PixelEffect::current->bindAttribLocation("position", 0);
		PixelEffect::current->bindAttribLocation("colour", 1);
		PixelEffect::current->bindAttribLocation("texCoord", 2);
		
		Matrix mvp = *modelViewMatrix.top() * *projectionMatrix.top();
		PixelEffect::current->sendMatrix("mvp", 4, mvp.getElements(), 1);

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(2);

		if(useStdShader == true)
		  primitivesEffect->detach();
	}

	bool Graphics::hasFocus()
	{
		return currentWindow->hasFocus();
	}
} // opengl
} // graphics
} // love
