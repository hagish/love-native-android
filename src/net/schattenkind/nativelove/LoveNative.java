package net.schattenkind.nativelove;

import java.io.File;

import android.app.Activity;
import android.content.res.Configuration;
import android.media.AudioManager;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;

public class LoveNative extends Activity {
    /** Called when the activity is first created. */
	private LoveRenderView mGLView;
	private float mOldVolume = 1.f;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setVolumeControlStream(AudioManager.STREAM_MUSIC);

        String filePath = "/mnt/sdcard/love/iyfct";

        Uri data = getIntent().getData();
        if (data != null) {
        	String path = data.getEncodedPath();
        	File file = new File(path);
        	if (file.exists()) {
        		filePath = path;

        		//Only store custom paths
        		RecentHelper.addRecent(this, path);
        	}
        }

        //setContentView(R.layout.main);
        LoveJNI.setActivity(this);
        mGLView = new LoveRenderView(this, filePath);
        setContentView(mGLView);
        //LoveJNI.step();
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event)
    {
    	// hardware sound button handling
    	if (keyCode == KeyEvent.KEYCODE_MUTE)
    	{
    		LoveJNI.setDeviceAudioVolume(0f);
    	}
    	if (keyCode == KeyEvent.KEYCODE_VOLUME_UP)
    	{
    		LoveJNI.setDeviceAudioVolume(LoveJNI.getDeviceAudioVolume() + 0.1f);
    	}
    	if (keyCode == KeyEvent.KEYCODE_VOLUME_DOWN)
    	{
    		LoveJNI.setDeviceAudioVolume(LoveJNI.getDeviceAudioVolume() - 0.1f);
    	}

    	//Temporary fix for exiting the application
	//TODO Remove this when implemented into Love.cpp instead
    	if (keyCode == KeyEvent.KEYCODE_BACK) {
    		return super.onKeyDown(keyCode, event);
    	}

        if(LoveJNI.onKeyDown(keyCode))
        	return true;
        return super.onKeyDown(keyCode, event);
    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event)
    {
    	//Temporary fix for exiting the application
	//TODO Remove this when implemented into Love.cpp instead
    	if (keyCode == KeyEvent.KEYCODE_BACK) {
    		return super.onKeyUp(keyCode, event);
    	}

    	if(LoveJNI.onKeyUp(keyCode))
        	return true;
        return super.onKeyUp(keyCode, event);
    }

    @Override
    public boolean onTouchEvent(MotionEvent event)
    {
    	boolean processed = false;
    	if(event.getAction() == MotionEvent.ACTION_DOWN)
    	{
    		LoveJNI.onMouseDown((int)event.getX(), (int)event.getY());
    		processed = true;
    	}
    	else if(event.getAction() == MotionEvent.ACTION_UP)
    	{
    		LoveJNI.onMouseUp((int)event.getX(), (int)event.getY());
    		processed = true;
    	}
    	else if (event.getAction() == MotionEvent.ACTION_MOVE)
    	{
    		LoveJNI.onMouseMove((int)event.getX(), (int)event.getY());
    		processed = true;
    	}

    	int count = event.getPointerCount();
    	if(count > 0)
    	{
    		int x[] = new int[count];
    		int y[] = new int[count];

    		for(int i = 0; i < count; ++i)
    		{
    			x[i] = (int)event.getX(i);
    			y[i] = (int)event.getY(i);
    		}

    		if(event.getAction() == MotionEvent.ACTION_DOWN)
        	{
    			if(Build.VERSION.SDK_INT >= 8)
    				LoveJNI.onTouchDown(count, event.getActionIndex(), x, y);
    			else
    				LoveJNI.onTouchDown(count, -1, x, y);
    			processed = true;
        	}
        	else if(event.getAction() == MotionEvent.ACTION_UP)
        	{
        		if(Build.VERSION.SDK_INT >= 8)
        			LoveJNI.onTouchUp(count, event.getActionIndex(), x, y);
        		else
        			LoveJNI.onTouchUp(count, -1, x, y);
        		processed = true;
        	}
        	else if (event.getAction() == MotionEvent.ACTION_MOVE)
        	{
        		if(Build.VERSION.SDK_INT >= 8)
        			LoveJNI.onTouchMove(count, event.getActionIndex(), x, y);
        		else
        			LoveJNI.onTouchMove(count, -1, x, y);
        		processed = true;
        	}
    	}
    	if(processed)
    		return true;
    	return super.onTouchEvent(event);
    }

    @Override
    public void onBackPressed()
    {
        super.onBackPressed();
//        LoveJNI.deinit();
        mGLView.onPause();
    }

    @Override
    public boolean onKeyLongPress(int keyCode, KeyEvent event)
    {
        // TODO Auto-generated method stub
        return super.onKeyLongPress(keyCode, event);
    }

    // clean up love at onPause call - native memory is not protected afterwards
    // we need a solution to store the game state at that moment
    @Override
    public void onPause()
    {
    	//mGLView.onPause(); // TODO: FIX OpenGL cleanup
    	mGLView.stopRendering();
    	mOldVolume = LoveJNI.getDeviceAudioVolume();
    	LoveJNI.setDeviceAudioVolume(0.f);
    	super.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        mGLView.continueRendering();
        LoveJNI.setDeviceAudioVolume(mOldVolume);
        //mGLView.onResume(); // TODO: FIX OpenGL cleanup
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig)
    {
    	super.onConfigurationChanged(newConfig);
    }

    @Override
    public void onDestroy()
    {
    	Log.i("LoveNative", "onDestroy");
    	LoveJNI.deinit();
    	super.onDestroy();
    }

//    @Override
//    public void onStop()
//    {
//    	android.os.Process.killProcess(android.os.Process.myPid());
//    }
}
