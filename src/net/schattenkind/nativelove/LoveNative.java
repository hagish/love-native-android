package net.schattenkind.nativelove;

import java.io.File;

import android.app.Activity;
import android.net.Uri;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.MotionEvent;

public class LoveNative extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
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
        setContentView(new LoveRenderView(this, filePath));
        //LoveJNI.step();
    }
    
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event)
    {
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
    	if(event.getAction() == MotionEvent.ACTION_DOWN)
    	{
    		LoveJNI.onMouseDown((int)event.getX(), (int)event.getY());
    		return true;
    	}
    	else if(event.getAction() == MotionEvent.ACTION_UP)
    	{
    		LoveJNI.onMouseUp((int)event.getX(), (int)event.getY());
    		return true;
    	}
    	else if (event.getAction() == MotionEvent.ACTION_MOVE)
    	{
    		LoveJNI.onMouseMove((int)event.getX(), (int)event.getY());
    	}
        return super.onTouchEvent(event);
    }
    
    @Override
    public void onBackPressed()
    {
        // TODO Auto-generated method stub
        super.onBackPressed();
    }
    
    @Override
    public boolean onKeyLongPress(int keyCode, KeyEvent event)
    {
        // TODO Auto-generated method stub
        return super.onKeyLongPress(keyCode, event);
    }
    
    @Override
    public void onDestroy()
    {
    	LoveJNI.deinit();
    	super.onDestroy();
    }
    
}
