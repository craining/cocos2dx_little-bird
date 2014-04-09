/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package com.zgy.hellobirdcoco;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.json.JSONObject;

import android.app.Activity;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.util.TypedValue;
import android.view.ViewGroup.LayoutParams;
import android.widget.RelativeLayout;

import com.baidu.mobads.AdView;
import com.baidu.mobads.AdViewListener;
import com.zgy.hellobirdcoco.view.AppDialog;

public class HelloBirdCoCo extends Cocos2dxActivity {

	private static final String TAG = "Cocos2dxActivity";
	private static Activity mContext;

	private AdView mAdView;

	protected void onCreate(Bundle savedInstanceState) {
		mContext = HelloBirdCoCo.this;
		Log.e("", " before super.onCreate");
		super.onCreate(savedInstanceState);
		Log.e("", " after super.onCreate");
		setupAds();
	}

	// public RelativeLayout onCreateLayout(Cocos2dxGLSurfaceView surfaceView) {
	// Log.e("", "onCreateLayout");
	// RelativeLayout layout = new RelativeLayout(this);
	// layout.addView(surfaceView);
	// return layout;
	// }

	public Cocos2dxGLSurfaceView onCreateView() {
		Log.e("", "onCreateView");
		Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
		// HelloBirdCoCo should create stencil buffer
		glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
		return glSurfaceView;
	}

	static {
		System.loadLibrary("cocos2dcpp");
	}

	@Override
	public void showDialog(String pTitle, String pMessage) {
		super.showDialog(pTitle, pMessage);

		Log.e(TAG, "showDialog");
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();

	}

	@Override
	public void onBackPressed() {
		super.onBackPressed();
	}

	private void setupAds() {

		RelativeLayout layout = new RelativeLayout(this);
		addContentView(layout, new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
		mAdView = new AdView(this);
		
		float addHeight = TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, 45, getApplication().getResources().getDisplayMetrics());
		RelativeLayout.LayoutParams layoutParams = new RelativeLayout.LayoutParams(LayoutParams.MATCH_PARENT, Math.round(addHeight));
		layoutParams.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
		layout.addView(mAdView, layoutParams);

		mAdView.setListener(new AdViewListener() {

			@Override
			public void onVideoStart() {
			}

			@Override
			public void onVideoFinish() {
			}

			@Override
			public void onVideoError() {
			}

			@Override
			public void onVideoClickReplay() {
			}

			@Override
			public void onVideoClickClose() {
			}

			@Override
			public void onVideoClickAd() {
			}

			@Override
			public void onAdSwitch() {
				Log.e("", "onAdSwitch");
			}

			@Override
			public void onAdShow(JSONObject arg0) {
				Log.e("", "onAdShow");
			}

			@Override
			public void onAdReady(AdView arg0) {
				Log.e("", "onAdReady");
			}

			@Override
			public void onAdFailed(String arg0) {
				Log.e("", "onAdFailed");
			}

			@Override
			public void onAdClick(JSONObject arg0) {
			}
		});
	}

	public void share() {
		Log.i("cppCall", "share");
		mHandler.sendEmptyMessage(MSG_SHARE);
	}

	public void showScore(int score) {
		Log.i("cppCall", "show score");
		Message msg = mHandler.obtainMessage();
		msg.what = MSG_SHOW_SCORE;
		msg.arg1 = score;
		mHandler.sendMessage(msg);
	}

	public static Object cppCall_logsth() {
		// 静态方法
		Log.i("cppCall", "getClassObject");
		return mContext;
	}

	private static final int MSG_SHARE = 100;
	private static final int MSG_SHOW_SCORE = 101;
	private Handler mHandler = new Handler() {

		@Override
		public void handleMessage(Message msg) {
			super.handleMessage(msg);
			switch (msg.what) {
			case MSG_SHARE:
				Log.i("cppCall", "test~~~~!!!");
				shareApp();
				// startActivity(new Intent(HelloBirdCoCo.this, ShowScoreActivity.class));
				break;
			case MSG_SHOW_SCORE:

				AppDialog.Builder dlg = new AppDialog.Builder(HelloBirdCoCo.this);
				dlg.setTitle("全球排名");
				dlg.setMessage(String.format("您得分为 %d, 全球排名第 %d！", msg.arg1, 10));
				dlg.setPositiveButton("好的", new DialogInterface.OnClickListener() {

					@Override
					public void onClick(DialogInterface dialog, int which) {

						dialog.dismiss();
					}
				});
				dlg.setNegativeButton("分享", new DialogInterface.OnClickListener() {

					@Override
					public void onClick(DialogInterface dialog, int which) {
						dialog.dismiss();
						shareScore(5, 10);

					}
				});
				dlg.create().show();

				break;
			default:
				break;
			}
		}

	};

	/**
	 * 分享
	 * 
	 * @param
	 * @author zhuanggy
	 * @date 2014-1-7
	 */
	private void shareApp() {
		Intent intent = new Intent(Intent.ACTION_SEND);
		intent.setType("text/plain");
		intent.putExtra(Intent.EXTRA_SUBJECT, "游戏分享：");
		intent.putExtra(Intent.EXTRA_TEXT, "发现一款好玩的游戏，快来下载体验一下吧！http://www.baidu.com");
		startActivity(Intent.createChooser(intent, "游戏分享"));
	}

	/**
	 * 分享
	 * 
	 * @param
	 * @author zhuanggy
	 * @date 2014-1-7
	 */
	private void shareScore(int score, int sort) {
		Intent intent = new Intent(Intent.ACTION_SEND);
		intent.setType("text/plain");
		intent.putExtra(Intent.EXTRA_SUBJECT, "游戏分享：");
		intent.putExtra(Intent.EXTRA_TEXT, String.format("我竟然在此游戏中得了 %d 分，全球排名第 %d！太了不起了，你也快来下载玩玩吧！http://www.baidu.com", score, sort));
		startActivity(Intent.createChooser(intent, "游戏分享"));
	}
}
