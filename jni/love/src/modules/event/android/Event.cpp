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

#include "Event.h"
#include "../../../../../AndroidEvent.h"

#include <keyboard/Keyboard.h>
#include <mouse/Mouse.h>
#include <pthread.h>
#include <queue>

extern std::queue<AndroidEvent*> gAndroidEvents;
extern pthread_mutex_t gEventMutex;
extern pthread_cond_t gEventCond;

namespace love
{
namespace event
{
namespace android
{
	const char * Event::getName() const
	{
		return "love.event.android";
	}

	Event::Event()
	{
	}

	void Event::pump()
	{
		AndroidEvent *ev;
		pthread_mutex_lock(&gEventMutex);

		Message *msg;

		while (gAndroidEvents.size() > 0)
		{
			ev = gAndroidEvents.front();
			gAndroidEvents.pop();
			msg = convert(*ev);
			delete ev;
			if (msg)
			{
				push(msg);
				msg->release();
			}
		}
		pthread_mutex_unlock(&gEventMutex);
	}

	Message *Event::wait()
	{
		pthread_mutex_lock(&gEventMutex);
		if(gAndroidEvents.size() == 0)
			pthread_cond_wait(&gEventCond, &gEventMutex);
		AndroidEvent *ev = gAndroidEvents.front();
		gAndroidEvents.pop();
		pthread_mutex_unlock(&gEventMutex);
		
		Message *msg = convert(*ev);
		delete ev;
		return msg;
	}

	void Event::clear()
	{
		pthread_mutex_lock(&gEventMutex);
		while(!gAndroidEvents.empty())
		{
			delete gAndroidEvents.front();
			gAndroidEvents.pop();
		}
		pthread_mutex_unlock(&gEventMutex);

		love::event::Event::clear();
	}

	Message *Event::convert(AndroidEvent & e)
	{
		Message *msg = NULL;
		love::keyboard::Keyboard::Key key;
		love::mouse::Mouse::Button button;
		Variant *arg1, *arg2, *arg3;
		const char *txt;
		switch(e.event)
		{
		case ANDROID_KEY_DOWN:
			if (keys.find(e.keyCode, key) && love::event::Event::keys.find(key, txt))
			{
				arg1 = new Variant(txt, strlen(txt));
				arg2 = new Variant((double) e.keyCode);
				msg = new Message("keypressed", arg1, arg2);
				arg1->release();
				arg2->release();
			}
			break;
		case ANDROID_KEY_UP:
			if (keys.find(e.keyCode, key) && love::event::Event::keys.find(key, txt))
			{
				arg1 = new Variant(txt, strlen(txt));
				msg = new Message("keyreleased", arg1);
				arg1->release();
			}
			break;
		case ANDROID_MOUSE_DOWN:
		case ANDROID_MOUSE_UP:
			if (buttons.find(e.keyCode, button) && love::event::Event::buttons.find(button, txt))
			{
				arg1 = new Variant((double) e.x);
				arg2 = new Variant((double) e.y);
				arg3 = new Variant(txt, strlen(txt));
				msg = new Message((e.event == ANDROID_MOUSE_DOWN) ?
						"mousepressed" : "mousereleased",
						arg1, arg2, arg3);
				arg1->release();
				arg2->release();
				arg3->release();
			}
			break;
		case ANDROID_JOYSTICK_DOWN:
		case ANDROID_JOYSTICK_UP:
			arg1 = new Variant((double) (e.devId+1));
			arg2 = new Variant((double) (e.keyCode+1));
			msg = new Message((e.event == ANDROID_JOYSTICK_DOWN) ?
					"joystickpressed" : "joystickreleased",
					arg1, arg2);
			arg1->release();
			arg2->release();
			break;
		case ANDROID_ACTIVE_EVENT:
			arg1 = new Variant(e.keyCode == 1);
			msg = new Message("focus", arg1);
			arg1->release();
			break;
		case ANDROID_QUIT:
			msg = new Message("quit");
			break;
		}

		return msg;
	}

	EnumMap<love::keyboard::Keyboard::Key, int, love::keyboard::Keyboard::KEY_MAX_ENUM>::Entry Event::keyEntries[] =
	{
		{ love::keyboard::Keyboard::KEY_UP, ANDROID_KEY_DPAD_UP },
		{ love::keyboard::Keyboard::KEY_DOWN, ANDROID_KEY_DPAD_DOWN },
		{ love::keyboard::Keyboard::KEY_RIGHT, ANDROID_KEY_DPAD_RIGHT },
		{ love::keyboard::Keyboard::KEY_LEFT, ANDROID_KEY_DPAD_LEFT },
		{ love::keyboard::Keyboard::KEY_RETURN, ANDROID_KEY_DPAD_CENTER },
	};

	EnumMap<love::keyboard::Keyboard::Key, int, love::keyboard::Keyboard::KEY_MAX_ENUM> Event::keys(Event::keyEntries, sizeof(Event::keyEntries));

	EnumMap<love::mouse::Mouse::Button, uint8_t, love::mouse::Mouse::BUTTON_MAX_ENUM>::Entry Event::buttonEntries[] =
	{
		{ love::mouse::Mouse::BUTTON_LEFT, ANDROID_MOUSE_LEFT},
		{ love::mouse::Mouse::BUTTON_RIGHT, ANDROID_MOUSE_RIGHT},
	};

	EnumMap<love::mouse::Mouse::Button, uint8_t, love::mouse::Mouse::BUTTON_MAX_ENUM> Event::buttons(Event::buttonEntries, sizeof(Event::buttonEntries));

} // android
} // event
} // love
