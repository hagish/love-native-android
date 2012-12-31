#!/bin/bash

NDK=/home/moe/android/android-ndk-r7

adb logcat | $NDK/ndk-stack -sym obj/local/armeabi
