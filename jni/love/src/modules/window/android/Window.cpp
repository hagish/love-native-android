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

// STL
#include <iostream>

// OpenGL
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>

// LOVE
#include <common/config.h>
#include "Window.h"

extern int gScreenWidth;
extern int gScreenHeight;

namespace love
{
namespace window
{
namespace android
{
	Window::Window()
		: windowTitle(""), created(false), mouseVisible(false)
	{
	}

	Window::~Window()
	{
	}

	Window::_currentMode::_currentMode()
		: width(gScreenWidth), height(gScreenHeight), fullscreen(true), vsync(true), fsaa(0)
	{
	}

	bool Window::setWindow(int width, int height, bool fullscreen, bool vsync, int fsaa)
	{
		bool mouseVisible = getMouseVisible();

		// Set caption.
		setWindowTitle(windowTitle);
		setMouseVisible(mouseVisible);

		created = true;



		GLint buffers;
		GLint samples;

		glGetIntegerv(GL_SAMPLE_BUFFERS, &buffers);
		glGetIntegerv(GL_SAMPLES, &samples);

		// Set the new display mode as the current display mode.
		currentMode.width = gScreenWidth;
		currentMode.height = gScreenHeight;
		currentMode.fsaa = fsaa;
		currentMode.fullscreen = fullscreen;
		currentMode.vsync = true;

		return true;
	}

	void Window::getWindow(int &width, int &height, bool &fullscreen, bool &vsync, int &fsaa)
	{
		width = gScreenWidth;
		height = gScreenHeight;
		fullscreen = true;
		vsync = true;
		fsaa = currentMode.fsaa;
	}

	bool Window::checkWindowSize(int width, int height, bool fullscreen)
	{
		if(fullscreen && width == gScreenWidth && height == gScreenHeight)
		  return true;
		return false;
	}

	typedef Window::WindowSize WindowSize;

	WindowSize **Window::getFullscreenSizes(int &n)
	{
		WindowSize **sizes = new WindowSize*[1];

		sizes[0] = new WindowSize;
		sizes[0]->width = gScreenWidth;
		sizes[0]->height = gScreenHeight;
		return sizes;
	}

	int Window::getWidth()
	{
		return gScreenWidth;
	}

	int Window::getHeight()
	{
		return gScreenHeight;
	}

	bool Window::isCreated()
	{
		return created;
	}

	void Window::setWindowTitle(std::string &title)
	{
		windowTitle = title;
	}

	std::string Window::getWindowTitle()
	{
		// not a reference
		// because we want this untouched
		// const std::string& might be an option
		return windowTitle;
	}

	bool Window::setIcon(love::image::ImageData *imgd)
	{
		uint32_t rmask, gmask, bmask, amask;
#ifdef LOVE_BIG_ENDIAN
		rmask = 0xFF000000;
		gmask = 0x00FF0000;
		bmask = 0x0000FF00;
		amask = 0x000000FF;
#else
		rmask = 0x000000FF;
		gmask = 0x0000FF00;
		bmask = 0x00FF0000;
		amask = 0xFF000000;
#endif

		int w = static_cast<int>(imgd->getWidth());
		int h = static_cast<int>(imgd->getHeight());
		int pitch = static_cast<int>(imgd->getWidth() * 4);

		return true;
	}

	void Window::swapBuffers()
	{
	}

	bool Window::hasFocus()
	{
		return true;
	}

	void Window::setMouseVisible(bool visible)
	{
	}

	bool Window::getMouseVisible()
	{
		return false;
	}

	love::window::Window *Window::getSingleton()
	{
		if (!singleton)
			singleton = new Window();
		else
			singleton->retain();

		return singleton;
	}

	const char *Window::getName() const
	{
		return "love.window.android";
	}
} // android
} // window
} // love
