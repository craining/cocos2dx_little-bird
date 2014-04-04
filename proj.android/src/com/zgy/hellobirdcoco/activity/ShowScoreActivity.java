package com.zgy.hellobirdcoco.activity;

import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import android.app.Activity;
import android.os.Bundle;
import android.view.Window;

public class ShowScoreActivity extends Activity{

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
	}



}
