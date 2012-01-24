#!/bin/bash

./generate_lua.sh

/opt/android-ndk-r6b/ndk-build -j `cat /proc/cpuinfo |grep processor|wc -l`
