#!/bin/bash

OBJDUMP=/home/moe/android/android-ndk-r7/toolchains/arm-linux-androideabi-4.4.3/prebuilt/linux-x86/bin/arm-linux-androideabi-objdump
PARSE_STACK=~/workspace/adt/android-ndk-stacktrace-analyzer-read-only/parse_stack.py
ADDR2LINE=/home/moe/android/android-ndk-r7/toolchains/arm-linux-androideabi-4.4.3/prebuilt/linux-x86/bin/arm-linux-androideabi-addr2line

#~ $OBJDUMP -S libs/armeabi/liblove.so > liblove.asm
#~ python $PARSE_STACK liblove.asm logcat.txt


while read line
do
	ADDR=`echo $line | awk '{print $5}'`
	LEVEL=`echo $line | awk '{print $3}'`
	LOC=`$ADDR2LINE -C -f -e obj/local/armeabi/liblove.so $ADDR | sed -e "s#$PWD##g"`
	echo -n "$LEVEL "
	echo -n $LOC
	echo -en "\t"
	echo $line
done < `./adb logcat`
#|grep liblove|grep " pc ")
