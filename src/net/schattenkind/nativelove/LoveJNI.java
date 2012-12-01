package net.schattenkind.nativelove;

import android.app.Activity;

public class LoveJNI {

     static {
    	 // whatever you do, do not change the order!
    	 System.loadLibrary("ogg");
    	 System.loadLibrary("vorbis");
    	 System.loadLibrary("mpg123");
    	 System.loadLibrary("modplug");
    	 System.loadLibrary("openal");
    	 System.loadLibrary("physfs");
    	 System.loadLibrary("jpeg");
    	 System.loadLibrary("tiff");
    	 System.loadLibrary("mng");
    	 System.loadLibrary("png");
    	 System.loadLibrary("jasper");
    	 System.loadLibrary("lcms");
    	 System.loadLibrary("devil");
    	 System.loadLibrary("freetype");
    	 System.loadLibrary("lua");
         System.loadLibrary("love");
     }

    /**
     * @param width the current view width
     * @param height the current view height
     * @param file the love file to open
     */
     public static native void init(int width, int height, String file);
     public static native void step();
     public static native void deinit();
     public static native boolean onKeyDown(int keyCode);
     public static native boolean onKeyUp(int keyCode);
     public static native boolean onMouseDown(int x, int y);
     public static native boolean onMouseUp(int x, int y);
     public static native void onMouseMove(int x, int y);
     public static native void onScreenSizeChanged(int width, int height);
     public static native void saveOpenGLState();
     public static native void restoreOpenGLState();
     public static native boolean onTouchDown(int count, int eventId, int x[], int y[]);
     public static native boolean onTouchUp(int count, int eventId, int x[], int y[]);
     public static native boolean onTouchMove(int count, int eventId, int x[], int y[]);
     public static native void onSensorChanged(String name, String type, float values[]);
     public static native void setPackageFile(String apkFile);

     /**
      * @param volume 0 (mute) - 1f (max)
      */
     public static native void setDeviceAudioVolume(float volume);
     public static native float getDeviceAudioVolume();

     public static boolean mExitQueued = false;
     public static void exitLove() {
    	 mExitQueued = true;
     }

     public static void doExit() {
    	 if(mAndroidActivity != null)
    		 mAndroidActivity.finish();
     }

     private static LoveNative mAndroidActivity = null;
     public static void setActivity(LoveNative activity) {
    	 mAndroidActivity = activity;
     }

     public static void disableSensor(String name) {
    	 if(mAndroidActivity != null)
    		 mAndroidActivity.disableSensor(name);
     }

     public static void enableSensor(String name) {
    	 if(mAndroidActivity != null)
    		 mAndroidActivity.enableSensor(name);
     }
}
