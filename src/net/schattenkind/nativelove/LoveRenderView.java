package net.schattenkind.nativelove;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.Log;

class LoveRenderView extends GLSurfaceView
{
	private final String loveFile;
	private final String apkPackageFile;
	private boolean loveCreated = false;
	private boolean mSkipRendering = false;

	public LoveRenderView(Context context, String filePath, String apkFile)
    {
		super(context);
	 // Create an OpenGL ES 2.0 context
	    setEGLContextClientVersion(2);

	    loveFile = filePath;
	    apkPackageFile = apkFile;
	    Log.i("LoveRenderView", "java: LoveRenderView");

	    setRenderer(new Renderer() {
			@Override
            public void onDrawFrame(GL10 gl)
            {
				Log.i("LoveRenderView", "java: step");
				if(!LoveJNI.mExitQueued)
				{
					if(mSkipRendering)
					{
						try
                        {
	                        Thread.sleep(250);
                        }
                        catch(InterruptedException e)
                        {
                        }
					}
					else
					{
						LoveJNI.step();
					}

				}
				else
				{
					LoveJNI.doExit();
				}
            }

			@Override
            public void onSurfaceChanged(GL10 gl, int width, int height)
            {
				if (loveCreated == false)
				{
					Log.i("LoveRenderView", "java: init");
					LoveJNI.setPackageFile(apkPackageFile);
		            LoveJNI.init(width, height, loveFile);
		            loveCreated = true;
		            Log.i("LoveRenderView", "java: init done");
				}
				else
				{
					LoveJNI.onScreenSizeChanged(width, height);
				}
            }

			@Override
            public void onSurfaceCreated(GL10 gl, EGLConfig config)
            {

            }
	    });
    }

	@Override
	public void onPause()
    {
		LoveJNI.saveOpenGLState();
		super.onPause();
    }

	@Override
    public void onResume() {
        super.onResume();
        LoveJNI.restoreOpenGLState();
    }

	public void stopRendering()
    {
	    mSkipRendering = true;

    }

	public void continueRendering()
    {
		mSkipRendering  = false;

    }
}
