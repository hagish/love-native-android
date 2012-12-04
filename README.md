Löve2d native port to android
=============================

### About

forum - https://love2d.org/forums/viewforum.php?f=11
original source - http://love2d.org

### State

0.7 - beta
0.8 - ?

### Pay attention to ...

* There is no joystick support (do not enable the module in the conf.lua).
* You can not change the screen resolution. It is enforced by the device.
* The forum: https://love2d.org/forums/viewforum.php?f=11

### Usage

The port contains a launcher activity (LoveLauncher) that is capable of starting any .love files on the android phone.

It is possible to bundle a game. All content from the assets/ folder is available (like executing love assets/). Therefore placing extracted game files (main.lua, ...) into assets/ and starting the LoveNative activity will directly launch the game.

### License

ATTENTION ATTENTION ATTENTION ATTENTION ATTENTION ATTENTION ATTENTION

Currently this project is a license mess. See jni/LICENSE for details.
