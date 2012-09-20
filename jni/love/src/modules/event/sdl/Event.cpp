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
namespace sdl
{
	const char * Event::getName() const
	{
		return "love.event.sdl";
	}

	Event::Event()
	{
	}

	void Event::pump()
	{
	}

	bool Event::poll(Message & message)
	{
		AndroidEvent *ev;
		pthread_mutex_lock(&gEventMutex);


		while(gAndroidEvents.size() > 0)
		{
			ev = gAndroidEvents.front();
			gAndroidEvents.pop();
			if(convert(*ev, message))
			{
				delete ev;
				pthread_mutex_unlock(&gEventMutex);
				return true;
			}
			delete ev;
		}

		pthread_mutex_unlock(&gEventMutex);
		return false;
	}

	bool Event::wait(Message & message)
	{
		pthread_mutex_lock(&gEventMutex);
		if(gAndroidEvents.size() == 0)
			pthread_cond_wait(&gEventCond, &gEventMutex);
		AndroidEvent *ev = gAndroidEvents.front();
		gAndroidEvents.pop();
		pthread_mutex_unlock(&gEventMutex);

		bool status = convert(*ev, message);
		delete ev;
		return status;
	}

	bool Event::push(Message & message)
	{
		AndroidEvent *ev = new AndroidEvent;
		bool ok = convert(message, *ev);
		pthread_mutex_lock(&gEventMutex);
		gAndroidEvents.push(ev);
		pthread_mutex_unlock(&gEventMutex);

		return ok;
	}

	void Event::clear()
	{
		pthread_mutex_lock(&gEventMutex);
		while(!gAndroidEvents.empty())
			gAndroidEvents.pop();
		pthread_mutex_unlock(&gEventMutex);
	}

	bool Event::convert(AndroidEvent & e, Message & m)
	{
		switch(e.event)
		{
		case ANDROID_KEY_DOWN:
			m.type = Event::TYPE_KEY_PRESSED;
			m.keyboard.u = e.keyCode;
			return keys.find(e.keyCode, m.keyboard.k);
		case ANDROID_KEY_UP:
			m.type = Event::TYPE_KEY_RELEASED;
			return keys.find(e.keyCode, m.keyboard.k);
		case ANDROID_MOUSE_DOWN:
		case ANDROID_MOUSE_UP:
			m.type = (e.event == ANDROID_MOUSE_DOWN) ? Event::TYPE_MOUSE_PRESSED : Event::TYPE_MOUSE_RELEASED;
			m.mouse.x = e.x;
			m.mouse.y = e.y;
			return buttons.find(e.keyCode, m.mouse.b);
		case ANDROID_TOUCH_DOWN:
			m.type = Event::TYPE_TOUCH_PRESSED;
			m.touch.x = e.xArr;
			m.touch.y = e.yArr;
			m.touch.actionIndex = e.keyCode;
			m.touch.size = e.arraySize;
			return true;
		case ANDROID_TOUCH_MOVE:
			m.type = Event::TYPE_TOUCH_MOVED;
			m.touch.x = e.xArr;
			m.touch.y = e.yArr;
			m.touch.actionIndex = e.keyCode;
			m.touch.size = e.arraySize;
			return true;
		case ANDROID_TOUCH_UP:
			m.type = Event::TYPE_TOUCH_RELEASED;
			m.touch.x = e.xArr;
			m.touch.y = e.yArr;
			m.touch.actionIndex = e.keyCode;
			m.touch.size = e.arraySize;
			return true;
		case ANDROID_SENSOR:
			m.type = Event::TYPE_SENSOR;
			m.sensor.values = e.values;
			m.sensor.size = e.arraySize;
			m.sensor.name = e.sensorName;
			m.sensor.senorType = e.sensorType;
			return true;
//		case SDL_JOYBUTTONDOWN:
//		case SDL_JOYBUTTONUP:
//			m.type = (e.type == SDL_JOYBUTTONDOWN) ? Event::TYPE_JOYSTICK_PRESSED : Event::TYPE_JOYSTICK_RELEASED;
//			m.joystick.button = e.jbutton.button;
//			m.joystick.index = e.jbutton.which;
//			return true;
//		case SDL_ACTIVEEVENT:
//			if (e.active.state & SDL_APPINPUTFOCUS) {
//				m.type = Event::TYPE_FOCUS;
//				m.focus.f = (e.active.gain != 0);
//				return true;
//			} else break;
		case ANDROID_QUIT:
			m.type = Event::TYPE_QUIT;
			return true;
		}

		return false;
	}

	bool Event::convert(Message & m, AndroidEvent & e)
	{
		switch(m.type)
		{
		case Event::TYPE_KEY_PRESSED:
			e.event = ANDROID_KEY_DOWN;
			e.keyCode = m.keyboard.u;
			return keys.find(m.keyboard.k, e.keyCode);
		case Event::TYPE_KEY_RELEASED:
			e.event = ANDROID_KEY_UP;
			return keys.find(m.keyboard.k, e.keyCode);
		case Event::TYPE_MOUSE_PRESSED:
		case Event::TYPE_MOUSE_RELEASED:
			e.event = (m.type == Event::TYPE_MOUSE_PRESSED) ? ANDROID_MOUSE_DOWN : ANDROID_MOUSE_UP;
			e.x = m.mouse.x;
			e.y = m.mouse.y;
			return buttons.find(m.mouse.b, e.keyCode);
		case Event::TYPE_TOUCH_PRESSED:
			e.event = ANDROID_TOUCH_DOWN;
			e.xArr = m.touch.x;
			e.yArr = m.touch.y;
			e.keyCode = m.touch.actionIndex;
			e.arraySize = m.touch.size;
			return true;
		case Event::TYPE_TOUCH_MOVED:
			e.event = ANDROID_TOUCH_MOVE;
			e.xArr = m.touch.x;
			e.yArr = m.touch.y;
			e.keyCode = m.touch.actionIndex;
			e.arraySize = m.touch.size;
			return true;
		case Event::TYPE_TOUCH_RELEASED:
			e.event = ANDROID_TOUCH_UP;
			e.xArr = m.touch.x;
			e.yArr = m.touch.y;
			e.keyCode = m.touch.actionIndex;
			e.arraySize = m.touch.size;
			return true;
		case Event::TYPE_SENSOR:
			e.event = ANDROID_SENSOR;
			e.values = m.sensor.values;
			e.arraySize = m.sensor.size;
			e.sensorName = m.sensor.name;
			e.sensorType = m.sensor.senorType;
			return true;
//		case Event::TYPE_JOYSTICK_PRESSED:
//		case Event::TYPE_JOYSTICK_RELEASED:
//			e.type = (m.type == Event::TYPE_JOYSTICK_PRESSED) ? SDL_JOYBUTTONDOWN : SDL_JOYBUTTONUP;
//			e.jbutton.which = m.joystick.index;
//			e.jbutton.button = m.joystick.button;
//			return true;
//		case Event::TYPE_FOCUS:
//			e.type = SDL_ACTIVEEVENT;
//			e.active.state = SDL_APPINPUTFOCUS;
//			e.active.gain = m.focus.f;
//			return true;
		case Event::TYPE_QUIT:
			e.event = ANDROID_QUIT;
			return true;
		default:
			return true;
		}

		return true;
	}

	EnumMap<love::keyboard::Keyboard::Key, int, love::keyboard::Keyboard::KEY_MAX_ENUM>::Entry Event::keyEntries[] =
	{
//		{ love::keyboard::Keyboard::KEY_BACKSPACE, SDLK_BACKSPACE},
//		{ love::keyboard::Keyboard::KEY_TAB, SDLK_TAB},
//		{ love::keyboard::Keyboard::KEY_CLEAR, SDLK_CLEAR},
//		{ love::keyboard::Keyboard::KEY_RETURN, SDLK_RETURN},
//		{ love::keyboard::Keyboard::KEY_PAUSE, SDLK_PAUSE},
//		{ love::keyboard::Keyboard::KEY_ESCAPE, SDLK_ESCAPE },
//		{ love::keyboard::Keyboard::KEY_SPACE, SDLK_SPACE },
//		{ love::keyboard::Keyboard::KEY_EXCLAIM, SDLK_EXCLAIM },
//		{ love::keyboard::Keyboard::KEY_QUOTEDBL, SDLK_QUOTEDBL },
//		{ love::keyboard::Keyboard::KEY_HASH, SDLK_HASH },
//		{ love::keyboard::Keyboard::KEY_DOLLAR, SDLK_DOLLAR },
//		{ love::keyboard::Keyboard::KEY_AMPERSAND, SDLK_AMPERSAND },
//		{ love::keyboard::Keyboard::KEY_QUOTE, SDLK_QUOTE },
//		{ love::keyboard::Keyboard::KEY_LEFTPAREN, SDLK_LEFTPAREN },
//		{ love::keyboard::Keyboard::KEY_RIGHTPAREN, SDLK_RIGHTPAREN },
//		{ love::keyboard::Keyboard::KEY_ASTERISK, SDLK_ASTERISK },
//		{ love::keyboard::Keyboard::KEY_PLUS, SDLK_PLUS },
//		{ love::keyboard::Keyboard::KEY_COMMA, SDLK_COMMA },
//		{ love::keyboard::Keyboard::KEY_MINUS, SDLK_MINUS },
//		{ love::keyboard::Keyboard::KEY_PERIOD, SDLK_PERIOD },
//		{ love::keyboard::Keyboard::KEY_SLASH, SDLK_SLASH },
//		{ love::keyboard::Keyboard::KEY_0, SDLK_0 },
//		{ love::keyboard::Keyboard::KEY_1, SDLK_1 },
//		{ love::keyboard::Keyboard::KEY_2, SDLK_2 },
//		{ love::keyboard::Keyboard::KEY_3, SDLK_3 },
//		{ love::keyboard::Keyboard::KEY_4, SDLK_4 },
//		{ love::keyboard::Keyboard::KEY_5, SDLK_5 },
//		{ love::keyboard::Keyboard::KEY_6, SDLK_6 },
//		{ love::keyboard::Keyboard::KEY_7, SDLK_7 },
//		{ love::keyboard::Keyboard::KEY_8, SDLK_8 },
//		{ love::keyboard::Keyboard::KEY_9, SDLK_9 },
//		{ love::keyboard::Keyboard::KEY_COLON, SDLK_COLON },
//		{ love::keyboard::Keyboard::KEY_SEMICOLON, SDLK_SEMICOLON },
//		{ love::keyboard::Keyboard::KEY_LESS, SDLK_LESS },
//		{ love::keyboard::Keyboard::KEY_EQUALS, SDLK_EQUALS },
//		{ love::keyboard::Keyboard::KEY_GREATER, SDLK_GREATER },
//		{ love::keyboard::Keyboard::KEY_QUESTION, SDLK_QUESTION },
//		{ love::keyboard::Keyboard::KEY_AT, SDLK_AT },
//
//		{ love::keyboard::Keyboard::KEY_LEFTBRACKET, SDLK_LEFTBRACKET },
//		{ love::keyboard::Keyboard::KEY_BACKSLASH, SDLK_BACKSLASH },
//		{ love::keyboard::Keyboard::KEY_RIGHTBRACKET, SDLK_RIGHTBRACKET },
//		{ love::keyboard::Keyboard::KEY_CARET, SDLK_CARET },
//		{ love::keyboard::Keyboard::KEY_UNDERSCORE, SDLK_UNDERSCORE },
//		{ love::keyboard::Keyboard::KEY_BACKQUOTE, SDLK_BACKQUOTE },
//		{ love::keyboard::Keyboard::KEY_A, SDLK_a },
//		{ love::keyboard::Keyboard::KEY_B, SDLK_b },
//		{ love::keyboard::Keyboard::KEY_C, SDLK_c },
//		{ love::keyboard::Keyboard::KEY_D, SDLK_d },
//		{ love::keyboard::Keyboard::KEY_E, SDLK_e },
//		{ love::keyboard::Keyboard::KEY_F, SDLK_f },
//		{ love::keyboard::Keyboard::KEY_G, SDLK_g },
//		{ love::keyboard::Keyboard::KEY_H, SDLK_h },
//		{ love::keyboard::Keyboard::KEY_I, SDLK_i },
//		{ love::keyboard::Keyboard::KEY_J, SDLK_j },
//		{ love::keyboard::Keyboard::KEY_K, SDLK_k },
//		{ love::keyboard::Keyboard::KEY_L, SDLK_l },
//		{ love::keyboard::Keyboard::KEY_M, SDLK_m },
//		{ love::keyboard::Keyboard::KEY_N, SDLK_n },
//		{ love::keyboard::Keyboard::KEY_O, SDLK_o },
//		{ love::keyboard::Keyboard::KEY_P, SDLK_p },
//		{ love::keyboard::Keyboard::KEY_Q, SDLK_q },
//		{ love::keyboard::Keyboard::KEY_R, SDLK_r },
//		{ love::keyboard::Keyboard::KEY_S, SDLK_s },
//		{ love::keyboard::Keyboard::KEY_T, SDLK_t },
//		{ love::keyboard::Keyboard::KEY_U, SDLK_u },
//		{ love::keyboard::Keyboard::KEY_V, SDLK_v },
//		{ love::keyboard::Keyboard::KEY_W, SDLK_w },
//		{ love::keyboard::Keyboard::KEY_X, SDLK_x },
//		{ love::keyboard::Keyboard::KEY_Y, SDLK_y },
//		{ love::keyboard::Keyboard::KEY_Z, SDLK_z },
//		{ love::keyboard::Keyboard::KEY_DELETE, SDLK_DELETE },
//
//		{ love::keyboard::Keyboard::KEY_KP0, SDLK_KP0 },
//		{ love::keyboard::Keyboard::KEY_KP1, SDLK_KP1 },
//		{ love::keyboard::Keyboard::KEY_KP2, SDLK_KP2 },
//		{ love::keyboard::Keyboard::KEY_KP3, SDLK_KP3 },
//		{ love::keyboard::Keyboard::KEY_KP4, SDLK_KP4 },
//		{ love::keyboard::Keyboard::KEY_KP5, SDLK_KP5 },
//		{ love::keyboard::Keyboard::KEY_KP6, SDLK_KP6 },
//		{ love::keyboard::Keyboard::KEY_KP7, SDLK_KP7 },
//		{ love::keyboard::Keyboard::KEY_KP8, SDLK_KP8 },
//		{ love::keyboard::Keyboard::KEY_KP9, SDLK_KP9 },
//		{ love::keyboard::Keyboard::KEY_KP_PERIOD, SDLK_KP_PERIOD },
//		{ love::keyboard::Keyboard::KEY_KP_DIVIDE, SDLK_KP_DIVIDE },
//		{ love::keyboard::Keyboard::KEY_KP_MULTIPLY, SDLK_KP_MULTIPLY},
//		{ love::keyboard::Keyboard::KEY_KP_MINUS, SDLK_KP_MINUS },
//		{ love::keyboard::Keyboard::KEY_KP_PLUS, SDLK_KP_PLUS },
//		{ love::keyboard::Keyboard::KEY_KP_ENTER, SDLK_KP_ENTER },
//		{ love::keyboard::Keyboard::KEY_KP_EQUALS, SDLK_KP_EQUALS },

		{ love::keyboard::Keyboard::KEY_UP, ANDROID_KEY_DPAD_UP },
		{ love::keyboard::Keyboard::KEY_DOWN, ANDROID_KEY_DPAD_DOWN },
		{ love::keyboard::Keyboard::KEY_RIGHT, ANDROID_KEY_DPAD_RIGHT },
		{ love::keyboard::Keyboard::KEY_LEFT, ANDROID_KEY_DPAD_LEFT },
		{ love::keyboard::Keyboard::KEY_RETURN, ANDROID_KEY_DPAD_CENTER },
//		{ love::keyboard::Keyboard::KEY_INSERT, SDLK_INSERT },
//		{ love::keyboard::Keyboard::KEY_HOME, SDLK_HOME },
//		{ love::keyboard::Keyboard::KEY_END, SDLK_END },
//		{ love::keyboard::Keyboard::KEY_PAGEUP, SDLK_PAGEUP },
//		{ love::keyboard::Keyboard::KEY_PAGEDOWN, SDLK_PAGEDOWN },
//
//		{ love::keyboard::Keyboard::KEY_F1, SDLK_F1 },
//		{ love::keyboard::Keyboard::KEY_F2, SDLK_F2 },
//		{ love::keyboard::Keyboard::KEY_F3, SDLK_F3 },
//		{ love::keyboard::Keyboard::KEY_F4, SDLK_F4 },
//		{ love::keyboard::Keyboard::KEY_F5, SDLK_F5 },
//		{ love::keyboard::Keyboard::KEY_F6, SDLK_F6 },
//		{ love::keyboard::Keyboard::KEY_F7, SDLK_F7 },
//		{ love::keyboard::Keyboard::KEY_F8, SDLK_F8 },
//		{ love::keyboard::Keyboard::KEY_F9, SDLK_F9 },
//		{ love::keyboard::Keyboard::KEY_F10, SDLK_F10 },
//		{ love::keyboard::Keyboard::KEY_F11, SDLK_F11 },
//		{ love::keyboard::Keyboard::KEY_F12, SDLK_F12 },
//		{ love::keyboard::Keyboard::KEY_F13, SDLK_F13 },
//		{ love::keyboard::Keyboard::KEY_F14, SDLK_F14 },
//		{ love::keyboard::Keyboard::KEY_F15, SDLK_F15 },
//
//		{ love::keyboard::Keyboard::KEY_NUMLOCK, SDLK_NUMLOCK },
//		{ love::keyboard::Keyboard::KEY_CAPSLOCK, SDLK_CAPSLOCK },
//		{ love::keyboard::Keyboard::KEY_SCROLLOCK, SDLK_SCROLLOCK },
//		{ love::keyboard::Keyboard::KEY_RSHIFT, SDLK_RSHIFT },
//		{ love::keyboard::Keyboard::KEY_LSHIFT, SDLK_LSHIFT },
//		{ love::keyboard::Keyboard::KEY_RCTRL, SDLK_RCTRL },
//		{ love::keyboard::Keyboard::KEY_LCTRL, SDLK_LCTRL },
//		{ love::keyboard::Keyboard::KEY_RALT, SDLK_RALT },
//		{ love::keyboard::Keyboard::KEY_LALT, SDLK_LALT },
//		{ love::keyboard::Keyboard::KEY_RMETA, SDLK_RMETA },
//		{ love::keyboard::Keyboard::KEY_LMETA, SDLK_LMETA },
//		{ love::keyboard::Keyboard::KEY_LSUPER, SDLK_LSUPER },
//		{ love::keyboard::Keyboard::KEY_RSUPER, SDLK_RSUPER },
//		{ love::keyboard::Keyboard::KEY_MODE, SDLK_MODE },
//		{ love::keyboard::Keyboard::KEY_COMPOSE, SDLK_COMPOSE },
//
//		{ love::keyboard::Keyboard::KEY_HELP, SDLK_HELP },
//		{ love::keyboard::Keyboard::KEY_PRINT, SDLK_PRINT },
//		{ love::keyboard::Keyboard::KEY_SYSREQ, SDLK_SYSREQ },
//		{ love::keyboard::Keyboard::KEY_BREAK, SDLK_BREAK },
//		{ love::keyboard::Keyboard::KEY_MENU, SDLK_MENU },
//		{ love::keyboard::Keyboard::KEY_POWER, SDLK_POWER },
//		{ love::keyboard::Keyboard::KEY_EURO, SDLK_EURO },
//		{ love::keyboard::Keyboard::KEY_UNDO, SDLK_UNDO },
//
//		{ love::keyboard::Keyboard::KEY_UNKNOWN, SDLK_UNKNOWN },
	};

	EnumMap<love::keyboard::Keyboard::Key, int, love::keyboard::Keyboard::KEY_MAX_ENUM> Event::keys(Event::keyEntries, sizeof(Event::keyEntries));

	EnumMap<love::mouse::Mouse::Button, int, love::mouse::Mouse::BUTTON_MAX_ENUM>::Entry Event::buttonEntries[] =
	{
		{ love::mouse::Mouse::BUTTON_LEFT, ANDROID_MOUSE_LEFT},
//		{ love::mouse::Mouse::BUTTON_MIDDLE, SDL_BUTTON_MIDDLE},
		{ love::mouse::Mouse::BUTTON_RIGHT, ANDROID_MOUSE_RIGHT},
//		{ love::mouse::Mouse::BUTTON_WHEELUP, SDL_BUTTON_WHEELUP},
//		{ love::mouse::Mouse::BUTTON_WHEELDOWN, SDL_BUTTON_WHEELDOWN},
//		{ love::mouse::Mouse::BUTTON_X1, SDL_BUTTON_X1},
//		{ love::mouse::Mouse::BUTTON_X2, SDL_BUTTON_X2},
	};

	EnumMap<love::mouse::Mouse::Button, int, love::mouse::Mouse::BUTTON_MAX_ENUM> Event::buttons(Event::buttonEntries, sizeof(Event::buttonEntries));

} // sdl
} // event
} // love
