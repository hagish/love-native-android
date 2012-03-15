package net.schattenkind.nativelove;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.opengl.GLSurfaceView;

class LoveRenderView extends GLSurfaceView
{
	private final String loveFile;

	public LoveRenderView(Context context, String filePath)
    {
	    super(context);

	    loveFile = filePath;

	    setRenderer(new Renderer() {
			@Override
            public void onDrawFrame(GL10 gl)
            {
				//System.out.println("java: step");
	            LoveJNI.step();
            }

			@Override
            public void onSurfaceChanged(GL10 gl, int width, int height)
            {
				System.out.println("java: init");
	            LoveJNI.init(width, height, loveFile);
            }

			@Override
            public void onSurfaceCreated(GL10 gl, EGLConfig config)
            {
	            
            }
	    });
    }
}
