package net.schattenkind.nativelove;

import java.io.File;
import java.util.LinkedList;

import android.app.Activity;
import android.content.pm.PackageInfo;
import android.content.res.Configuration;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.media.AudioManager;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;

public class LoveNative extends Activity implements SensorEventListener {
    /** Called when the activity is first created. */
	private LoveRenderView mGLView;
	private float mOldVolume = 1.f;

	//TODO: Add
	/*
	 * getSystemService(Context.SENSOR_SERVICE); and SensorManager
	 * getSystemService(Context.STORAGE_SERVICE); and all file stuff
	 * finish android_sensors in love.cpp (in real love)
	 * */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setVolumeControlStream(AudioManager.STREAM_MUSIC);
        mSensorManager = (SensorManager)getSystemService(SENSOR_SERVICE);
        mSensorList = new LinkedList<Sensor>();

        String filePath = "";
        
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
        
        Log.i("liblove", "apk file/directory: " + getApplicationInfo().sourceDir);
        Log.i("liblove", "game file/directory: " + filePath);

        //setContentView(R.layout.main);
        LoveJNI.setActivity(this);
        mGLView = new LoveRenderView(this, filePath, getApplicationInfo().sourceDir);
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
    	mSensorManager.unregisterListener(this);
    	super.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        mGLView.continueRendering();
        LoveJNI.setDeviceAudioVolume(mOldVolume);
        for(int i = 0; i < mSensorList.size(); ++i)
        	mSensorManager.registerListener(this, mSensorList.get(i), SensorManager.SENSOR_DELAY_GAME);
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

    private int strToSensor(String name)
    {
    	if(name.equals("TYPE_ACCELEROMETER"))
    		return Sensor.TYPE_ACCELEROMETER;
    	else if(name.equals("TYPE_ALL"))
    		return Sensor.TYPE_ALL;
    	else if(Build.VERSION.SDK_INT >= 14 && name.equals("TYPE_AMBIENT_TEMPERATURE "))
    		return 0xd; //Sensor.TYPE_AMBIENT_TEMPERATURE
    	else if(Build.VERSION.SDK_INT >= 9 && name.equals("TYPE_GRAVITY"))
    		return 0x9; //Sensor.TYPE_GRAVITY
    	else if(name.equals("TYPE_GYROSCOPE"))
    		return Sensor.TYPE_GYROSCOPE;
    	else if(name.equals("TYPE_LIGHT"))
    		return Sensor.TYPE_LIGHT;
    	else if(Build.VERSION.SDK_INT >= 9 && name.equals("TYPE_LINEAR_ACCELERATION"))
    		return 0xa; //Sensor.TYPE_LINEAR_ACCELERATION
    	else if(name.equals("TYPE_MAGNETIC_FIELD"))
    		return Sensor.TYPE_MAGNETIC_FIELD;
    	else if(name.equals("TYPE_ORIENTATION"))
    		return Sensor.TYPE_ORIENTATION;
    	else if(name.equals("TYPE_PRESSURE"))
    		return Sensor.TYPE_PRESSURE;
    	else if(name.equals("TYPE_PROXIMITY"))
    		return Sensor.TYPE_PROXIMITY;
    	else if(Build.VERSION.SDK_INT >= 14 && name.equals("TYPE_RELATIVE_HUMIDITY"))
    		return 0xc; //Sensor.TYPE_RELATIVE_HUMIDITY
    	else if(Build.VERSION.SDK_INT >= 9 && name.equals(" TYPE_ROTATION_VECTOR"))
    		return 0xb; //Sensor.TYPE_ROTATION_VECTOR
    	else if(name.equals("TYPE_TEMPERATURE"))
    		return Sensor.TYPE_TEMPERATURE ;
    	else
    		return Sensor.TYPE_ALL;
    }

    private String SensorToStr(int type)
    {
    	switch(type)
    	{
    		case Sensor.TYPE_ACCELEROMETER:
    			return "TYPE_ACCELEROMETER";
    		case Sensor.TYPE_ALL:
    			return "TYPE_ALL";
    		case 0xd:
    			return "TYPE_AMBIENT_TEMPERATURE";
    		case 0x9:
    			return "TYPE_GRAVITY";
    		case Sensor.TYPE_GYROSCOPE:
    			return "TYPE_GYROSCOPE";
    		case Sensor.TYPE_LIGHT:
    			return "TYPE_LIGHT";
    		case 0xa:
    			return "TYPE_LINEAR_ACCELERATION";
    		case Sensor.TYPE_MAGNETIC_FIELD:
    			return "TYPE_MAGNETIC_FIELD";
    		case Sensor.TYPE_ORIENTATION:
    			return "TYPE_ORIENTATION";
    		case Sensor.TYPE_PRESSURE:
    			return "TYPE_PRESSURE";
    		case Sensor.TYPE_PROXIMITY:
    			return "TYPE_PROXIMITY";
    		case 0xc:
    			return "TYPE_RELATIVE_HUMIDITY";
    		case 0xb:
    			return "TYPE_ROTATION_VECTOR";
    		case Sensor.TYPE_TEMPERATURE:
    			return "TYPE_TEMPERATURE";
			default:
				return "UNKNOWN";
    	}
    }

    private SensorManager mSensorManager;
    private LinkedList<Sensor> mSensorList;
	public void disableSensor(String name)
    {
		int type = strToSensor(name);
		Sensor sensor = mSensorManager.getDefaultSensor(type);
		mSensorManager.unregisterListener(this, sensor);
		mSensorList.remove(sensor);
    }

	public void enableSensor(String name)
    {
		int type = strToSensor(name);
		Sensor sensor = mSensorManager.getDefaultSensor(type);
		mSensorManager.registerListener(this, sensor, SensorManager.SENSOR_DELAY_GAME);
		mSensorList.add(sensor);
    }

	@Override
    public void onAccuracyChanged(Sensor sensor, int accuracy)
    {
	    // should not happen...
		// TODO: check if it does not happen :D

    }

	@Override
    public void onSensorChanged(SensorEvent event)
    {
	    LoveJNI.onSensorChanged(event.sensor.getName(), SensorToStr(event.sensor.getType()), event.values);
    }

//    @Override
//    public void onStop()
//    {
//    	android.os.Process.killProcess(android.os.Process.myPid());
//    }
}
