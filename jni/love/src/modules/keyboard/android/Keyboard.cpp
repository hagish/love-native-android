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

#include "Keyboard.h"
#include "AndroidEvent.h"

#include <map>

extern std::map<int, bool> gKeyState;

namespace love
{
namespace keyboard
{
namespace android
{
	const char * Keyboard::getName() const
	{
		return "love.keyboard.android";
	}

	bool Keyboard::isDown(Key * keylist) const
	{
		int k;
		for (Key key = *keylist; key != KEY_MAX_ENUM; key = *(++keylist))
		{
			if (keys.find(key, k) && gKeyState.find(k) == gKeyState.end())
				return true;
		}

		return false;
	}

	void Keyboard::setKeyRepeat(int delay, int interval) const
	{
	}

	int Keyboard::getKeyRepeatDelay() const
	{
		int delay = 0, interval = 0;
		return delay;
	}

	int Keyboard::getKeyRepeatInterval() const
	{
		int delay = 0, interval = 0;
		return interval;
	}

	EnumMap<Keyboard::Key, int, Keyboard::KEY_MAX_ENUM>::Entry Keyboard::keyEntries[] =
	{
		{ Keyboard::KEY_UP, ANDROID_KEY_DPAD_UP },
		{ Keyboard::KEY_DOWN, ANDROID_KEY_DPAD_DOWN },
		{ Keyboard::KEY_RIGHT, ANDROID_KEY_DPAD_RIGHT },
		{ Keyboard::KEY_LEFT, ANDROID_KEY_DPAD_LEFT },
		{ Keyboard::KEY_RETURN, ANDROID_KEY_DPAD_CENTER },
	};

	EnumMap<Keyboard::Key, int, Keyboard::KEY_MAX_ENUM> Keyboard::keys(Keyboard::keyEntries, sizeof(Keyboard::keyEntries));


} // sdl
} // keyboard
} // love
