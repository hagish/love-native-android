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

#include "Joystick.h"

// STD
#include <cmath>

namespace love
{
namespace joystick
{
namespace android
{
	Joystick::Joystick()
	{
		
	}

	Joystick::~Joystick()
	{
		
	}

	const char * Joystick::getName() const
	{
		return "love.joystick.android";
	}

	bool Joystick::checkIndex(int index)
	{
		if(index == 1)
		  return true;
		return false;
	}

	int Joystick::getNumJoysticks()
	{
		return 1;
	}

	const char * Joystick::getName(int index)
	{
		static const char* name = "android joystick";
		return name;
	}

	bool Joystick::open(int index)
	{
		return true;
	}

	bool Joystick::isOpen(int index)
	{
		return true;
	}

	bool Joystick::verifyJoystick(int index)
	{
		if (!checkIndex(index))
			return false;

		if (!isOpen(index))
			return false;

		return true;
	}

	int Joystick::getNumAxes(int index)
	{
		return  0;
	}

	int Joystick::getNumBalls(int index)
	{
		return 0;
	}

	int Joystick::getNumButtons(int index)
	{
		return 6;
	}

	int Joystick::getNumHats(int index)
	{
		return 0;
	}

	float Joystick::clampval(float x)
	{
		if (fabs((double)x) < 0.01) return 0.0f;
		if (x < -0.99f) return -1.0f;
		if (x > 0.99f) return 1.0f;
		return x;
	}

	float Joystick::getAxis(int index, int axis)
	{
		return 0;
	}

	int Joystick::getAxes(lua_State * L)
	{
		return 0;
	}

	int Joystick::getBall(lua_State * L)
	{
		return 0;
	}

	bool Joystick::isDown(int index, int * buttonlist)
	{
		return false;
	}

	Joystick::Hat Joystick::getHat(int index, int hat)
	{
		Hat h = HAT_INVALID;
		return h;
	}

	void Joystick::close(int index)
	{

	}

	EnumMap<Joystick::Hat, uint8_t, Joystick::HAT_MAX_ENUM>::Entry Joystick::hatEntries[] =
	{

	};

	EnumMap<Joystick::Hat, uint8_t, Joystick::HAT_MAX_ENUM> Joystick::hats(Joystick::hatEntries, sizeof(Joystick::hatEntries));

} // sdl
} // joystick
} // love
