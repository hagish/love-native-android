#ifndef ANDROID_EVENT_H
#define ANDROID_EVENT_H

#include <string>

enum AndroidEventType
{
	ANDROID_MOUSE_DOWN,
	ANDROID_MOUSE_UP,
	ANDROID_KEY_DOWN,
	ANDROID_KEY_UP,
	ANDROID_TOUCH,
	ANDROID_TOUCH_DOWN,
	ANDROID_TOUCH_UP,
	ANDROID_TOUCH_MOVE,
	ANDROID_SENSOR,
	ANDROID_QUIT
};

enum MouseButton
{
	ANDROID_MOUSE_NONE,
	ANDROID_MOUSE_LEFT,
	ANDROID_MOUSE_RIGHT,
};

enum KeyboardKey
{
	ANDROID_KEY_NONE,
	ANDROID_KEY_DPAD_UP = 19,
	ANDROID_KEY_DPAD_DOWN = 20,
	ANDROID_KEY_DPAD_LEFT = 21,
	ANDROID_KEY_DPAD_RIGHT = 22,
	ANDROID_KEY_DPAD_CENTER = 23,
};

struct CurrentMouseState
{
	float x;
	float y;
	int keyCode;
};

struct AndroidEvent
{
	AndroidEventType event;
	int keyCode;
	unsigned int x;
	unsigned int y;
	unsigned int *xArr;
	unsigned int *yArr;
	unsigned int arraySize;
	std::string *sensorName;
	std::string *sensorType;
	float *values;
};

#endif
