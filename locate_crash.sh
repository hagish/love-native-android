#!/bin/bash

ADDR2LINE=/opt/android-ndk-r6/toolchains/arm-linux-androideabi-4.4.3/prebuilt/linux-x86/bin/arm-linux-androideabi-addr2line

$ADDR2LINE -C -f -e obj/local/armeabi/liblove.so $1
