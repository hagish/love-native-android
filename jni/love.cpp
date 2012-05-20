#include <jni.h>

// Lua
extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

#include <loveLog.h>
#include <loveCore.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <queue>
#include <map>

#include "AndroidEvent.h"

std::queue<AndroidEvent*> gAndroidEvents;
pthread_mutex_t gEventMutex;
pthread_cond_t gEventCond;
CurrentMouseState gMouseState;
std::map<int, bool> gKeyState;
int gScreenWidth = 0;
int gScreenHeight = 0;
bool gDeinitDone = false;
bool gInitDone = false;

namespace love {
namespace audio {
namespace openal {
extern float requestedDeviceAudioVolume;
}
}
}

extern "C" {
    JNIEXPORT void JNICALL Java_net_schattenkind_nativelove_LoveJNI_init(JNIEnv * env, jobject obj,  jint width, jint height, jstring file);
    JNIEXPORT void JNICALL Java_net_schattenkind_nativelove_LoveJNI_step(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_net_schattenkind_nativelove_LoveJNI_deinit(JNIEnv * env, jobject obj);
    JNIEXPORT bool JNICALL Java_net_schattenkind_nativelove_LoveJNI_onKeyDown(JNIEnv * env, jobject obj, int keyCode);
    JNIEXPORT bool JNICALL Java_net_schattenkind_nativelove_LoveJNI_onKeyUp(JNIEnv * env, jobject obj, int keyCode);
    JNIEXPORT bool JNICALL Java_net_schattenkind_nativelove_LoveJNI_onMouseDown(JNIEnv * env, jobject obj, int x, int y);
    JNIEXPORT bool JNICALL Java_net_schattenkind_nativelove_LoveJNI_onMouseUp(JNIEnv * env, jobject obj, int x, int y);
    JNIEXPORT void JNICALL Java_net_schattenkind_nativelove_LoveJNI_onMouseMove(JNIEnv * env, jobject obj, int x, int y);
    JNIEXPORT void JNICALL Java_net_schattenkind_nativelove_LoveJNI_setDeviceAudioVolume(JNIEnv * env, jobject obj, float volume);
    JNIEXPORT float JNICALL Java_net_schattenkind_nativelove_LoveJNI_getDeviceAudioVolume(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_net_schattenkind_nativelove_LoveJNI_onScreenSizeChanged(JNIEnv * env, jobject obj, int width, int height);
};

JNIEnv *gEnv = NULL;
jobject *gObj = NULL;

void sendAndroidExitSignal(void)
{
  if(gEnv && gObj)
  {
	//jclass cls = gEnv->GetObjectClass(*gObj);
	
	jclass cls = gEnv->FindClass("net/schattenkind/nativelove/LoveJNI");

	jmethodID mid = gEnv->GetStaticMethodID(cls, "exitLove", "()V");
	if (mid == 0)
		return;
	gEnv->CallStaticVoidMethod(cls, mid);
  }
}


JNIEXPORT void JNICALL Java_net_schattenkind_nativelove_LoveJNI_init(JNIEnv * env, jobject obj,  jint width, jint height, jstring file)
{
	if(gInitDone)
		Java_net_schattenkind_nativelove_LoveJNI_deinit(env, obj);
	
	if(!gInitDone)
	{
		LOGI("init");

		/*
		int argc = 1;
		char **argv = new char *[1];
		argv[0] = new char[5];
		strcpy(argv[0], "love");
		*/

		int argc = 2;
		char **argv = new char *[2];
		argv[0] = new char[5];
		strcpy(argv[0], "love");
		const char * loveFile = env->GetStringUTFChars(file, NULL);
		argv[1] = new char[strlen(loveFile)+1];
		strcpy(argv[1], loveFile);
		env->ReleaseStringUTFChars(file, loveFile);

		pthread_mutex_init(&gEventMutex, NULL);
		pthread_cond_init(&gEventCond, NULL);

		gScreenWidth = width;
		gScreenHeight = height;

		main_prepare(argc, argv);

		delete argv;
		gInitDone = true;
		gDeinitDone = false;
	}
}

JNIEXPORT void JNICALL Java_net_schattenkind_nativelove_LoveJNI_step(JNIEnv * env, jobject obj)
{
	gEnv = env;
	gObj = &obj;
	main_step();
	gEnv = NULL;
	gObj = NULL;
}

JNIEXPORT void JNICALL Java_net_schattenkind_nativelove_LoveJNI_deinit(JNIEnv * env, jobject obj)
{
	if(!gDeinitDone)
	{
		LOGI("deinit");
		main_shutdown();
		gDeinitDone = true;
		gInitDone = false;
	}
}

JNIEXPORT bool JNICALL Java_net_schattenkind_nativelove_LoveJNI_onKeyDown(JNIEnv * env, jobject obj, int keyCode)
{
	LOGI("onKeyDown");
	AndroidEvent *ev = new AndroidEvent;
	ev->event = ANDROID_KEY_DOWN;
	ev->keyCode = keyCode;
	gKeyState[keyCode] = true;
	pthread_mutex_lock(&gEventMutex);
	gAndroidEvents.push(ev);
	pthread_mutex_unlock(&gEventMutex);
	pthread_cond_signal(&gEventCond);

	return true;
}

JNIEXPORT bool JNICALL Java_net_schattenkind_nativelove_LoveJNI_onKeyUp(JNIEnv * env, jobject obj, int keyCode)
{
	LOGI("onKeyUp");
	AndroidEvent *ev = new AndroidEvent;
	ev->event = ANDROID_KEY_UP;
	ev->keyCode = keyCode;
	gKeyState.erase(keyCode);
	pthread_mutex_lock(&gEventMutex);
	gAndroidEvents.push(ev);
	pthread_mutex_unlock(&gEventMutex);
	pthread_cond_signal(&gEventCond);

	return true;
}

JNIEXPORT bool JNICALL Java_net_schattenkind_nativelove_LoveJNI_onMouseDown(JNIEnv * env, jobject obj, int x, int y)
{
	LOGI("onMouseDown");
	AndroidEvent *ev = new AndroidEvent;
	ev->event = ANDROID_MOUSE_DOWN;
	ev->keyCode = ANDROID_MOUSE_LEFT;
	ev->x = x;
	ev->y = y;
	gMouseState.x = x;
	gMouseState.y = y;
	gMouseState.keyCode = ANDROID_MOUSE_LEFT;
	pthread_mutex_lock(&gEventMutex);
	gAndroidEvents.push(ev);
	pthread_mutex_unlock(&gEventMutex);
	pthread_cond_signal(&gEventCond);

	return true;
}

JNIEXPORT void JNICALL Java_net_schattenkind_nativelove_LoveJNI_onScreenSizeChanged(JNIEnv * env, jobject obj, int width, int height)
{
	LOGI("onScreenSizeChanged w=%i h=%i", width, height);

	gScreenWidth = width;
	gScreenHeight = height;

	lua_State *L = main_getLuaState();

	/* push functions and arguments */
	lua_getglobal(L, "android_onScreenSizeChanged");  /* function to be called */
	lua_pushnumber(L, width);   /* push 1st argument */
	lua_pushnumber(L, height);   /* push 2nd argument */

	/* do the call (2 arguments, 0 result) */
	if (lua_pcall(L, 2, 0, 0) != 0) {
		LOGE("error running function `f': %s", lua_tostring(L, -1));
	}
}

JNIEXPORT bool JNICALL Java_net_schattenkind_nativelove_LoveJNI_onMouseUp(JNIEnv * env, jobject obj, int x, int y)
{
	LOGI("onMouseUp");
	AndroidEvent *ev = new AndroidEvent;
	ev->event = ANDROID_MOUSE_UP;
	ev->keyCode = ANDROID_MOUSE_LEFT;
	ev->x = x;
	ev->y = y;
	gMouseState.x = x;
	gMouseState.y = y;
	gMouseState.keyCode = ANDROID_MOUSE_NONE;
	pthread_mutex_lock(&gEventMutex);
	gAndroidEvents.push(ev);
	pthread_mutex_unlock(&gEventMutex);
	pthread_cond_signal(&gEventCond);

	return true;
}

JNIEXPORT void JNICALL Java_net_schattenkind_nativelove_LoveJNI_onMouseMove(JNIEnv * env, jobject obj, int x, int y)
{
	//LOGI("onMouseMove");
	gMouseState.x = x;
	gMouseState.y = y;
}

JNIEXPORT float JNICALL Java_net_schattenkind_nativelove_LoveJNI_getDeviceAudioVolume(JNIEnv * env, jobject obj)
{
	return love::audio::openal::requestedDeviceAudioVolume;
}

JNIEXPORT void JNICALL Java_net_schattenkind_nativelove_LoveJNI_setDeviceAudioVolume(JNIEnv * env, jobject obj, float volume)
{
	LOGI("adjust volume");

	// clamp
	if (volume < 0)volume = 0;
	if (volume > 1)volume = 1;

	love::audio::openal::requestedDeviceAudioVolume = volume;
}
