#include <jni.h>

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

extern "C" {
    JNIEXPORT void JNICALL Java_net_schattenkind_nativelove_LoveJNI_init(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_net_schattenkind_nativelove_LoveJNI_step(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_net_schattenkind_nativelove_LoveJNI_deinit(JNIEnv * env, jobject obj);
    JNIEXPORT bool JNICALL Java_net_schattenkind_nativelove_LoveJNI_onKeyDown(JNIEnv * env, jobject obj, int keyCode);
    JNIEXPORT bool JNICALL Java_net_schattenkind_nativelove_LoveJNI_onKeyUp(JNIEnv * env, jobject obj, int keyCode);
    JNIEXPORT bool JNICALL Java_net_schattenkind_nativelove_LoveJNI_onMouseDown(JNIEnv * env, jobject obj, int x, int y);
    JNIEXPORT bool JNICALL Java_net_schattenkind_nativelove_LoveJNI_onMouseUp(JNIEnv * env, jobject obj, int x, int y);
};

JNIEXPORT void JNICALL Java_net_schattenkind_nativelove_LoveJNI_init(JNIEnv * env, jobject obj,  jint width, jint height)
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
	argv[1] = new char[strlen("/mnt/sdcard/love/iyfct")+1];
	strcpy(argv[1], "/mnt/sdcard/love/iyfct");

	pthread_mutex_init(&gEventMutex, NULL);
	pthread_cond_init(&gEventCond, NULL);

	gScreenWidth = width;
	gScreenHeight = height;

	main_prepare(argc, argv);

	delete argv;
}

JNIEXPORT void JNICALL Java_net_schattenkind_nativelove_LoveJNI_step(JNIEnv * env, jobject obj)
{
	main_step();
}

JNIEXPORT void JNICALL Java_net_schattenkind_nativelove_LoveJNI_deinit(JNIEnv * env, jobject obj)
{
	LOGI("deinit");
	main_shutdown();
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
