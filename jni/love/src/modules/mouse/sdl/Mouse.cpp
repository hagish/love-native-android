/**
* Copyright (c) 2006-2011 LOVE Development Team
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

#include "Mouse.h"
#include "AndroidEvent.h"
extern CurrentMouseState gMouseState;

namespace love
{
namespace mouse
{
namespace sdl
{
	const char * Mouse::getName() const
	{
		return "love.mouse.sdl";
	}

	int Mouse::getX() const
	{
		return gMouseState.x;
	}

	int Mouse::getY() const
	{
		return gMouseState.y;
	}

	void Mouse::getPosition(int & x, int & y) const
	{
		x = gMouseState.x;
		y = gMouseState.y;
	}

	void Mouse::setPosition(int x, int y)
	{
		gMouseState.x = x;
		gMouseState.y = y;
	}

	void Mouse::setVisible(bool visible)
	{
	}

	bool Mouse::isDown(Button * buttonlist) const
	{
		for (Button button = *buttonlist; button != BUTTON_MAX_ENUM; button = *(++buttonlist))
		{
			if (button == gMouseState.keyCode)
				return true;
		}
		return false;
	}

	bool Mouse::isVisible() const
	{
		return false;
	}

	void Mouse::setGrab(bool grab)
	{
	}

	bool Mouse::isGrabbed() const
	{
		return true;
	}

	EnumMap<Mouse::Button, unsigned, Mouse::BUTTON_MAX_ENUM>::Entry Mouse::buttonEntries[] = 
	{
		{ Mouse::BUTTON_LEFT, ANDROID_MOUSE_LEFT},
//		{ Mouse::BUTTON_MIDDLE, SDL_BUTTON_MIDDLE},
		{ Mouse::BUTTON_RIGHT, ANDROID_MOUSE_RIGHT},
//		{ Mouse::BUTTON_WHEELUP, SDL_BUTTON_WHEELUP},
//		{ Mouse::BUTTON_WHEELDOWN, SDL_BUTTON_WHEELDOWN},
//		{ Mouse::BUTTON_X1, SDL_BUTTON_X1},
//		{ Mouse::BUTTON_X2, SDL_BUTTON_X2},
	};

	EnumMap<Mouse::Button, unsigned, Mouse::BUTTON_MAX_ENUM> Mouse::buttons(Mouse::buttonEntries, sizeof(Mouse::buttonEntries));

} // sdl
} // mouse
} // love
