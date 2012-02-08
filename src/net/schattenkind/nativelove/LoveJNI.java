package net.schattenkind.nativelove;

public class LoveJNI {

     static {
    	 System.loadLibrary("ogg");
    	 System.loadLibrary("vorbis");
    	 System.loadLibrary("mpg123");
         System.loadLibrary("love");
     }

    /**
     * @param width the current view width
     * @param height the current view height
     */
     public static native void init(int width, int height);
     public static native void step();
     public static native void deinit();     
     public static native boolean onKeyDown(int keyCode);
     public static native boolean onKeyUp(int keyCode);
     public static native boolean onMouseDown(int x, int y);
     public static native boolean onMouseUp(int x, int y);
}
