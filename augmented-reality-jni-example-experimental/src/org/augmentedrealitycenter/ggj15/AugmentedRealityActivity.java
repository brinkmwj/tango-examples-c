/*
 * Copyright 2014 Google Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.augmentedrealitycenter.ggj15;

import android.app.Activity;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.graphics.Point;
import android.graphics.Typeface;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnCompletionListener;
import android.media.MediaPlayer.OnPreparedListener;
import android.net.Uri;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;
import android.view.Display;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.MediaController;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.VideoView;

/**
 * Main activity shows augmented reality scene.
 */
public class AugmentedRealityActivity extends Activity implements View.OnClickListener{

    public static final String EXTRA_KEY_PERMISSIONTYPE = "PERMISSIONTYPE";
    public static final String EXTRA_VALUE_VIO = "MOTION_TRACKING_PERMISSION";
    public static final String EXTRA_VALUE_VIOADF = "ADF_LOAD_SAVE_PERMISSION";

    private GLSurfaceView arView;
    private TextView tangoPoseStatusText;
    private TextView score;
    private VideoView introVideoView;
    private MediaController mediaControls;
    private MediaPlayer mediaPlayer;
    private ImageView imageView;
    private ImageView vinesView;
    private AugmentedRealityView arViewRenderer;
    private Typeface romancium;
    
    private float[] touchStartPos = new float[2];
    private float[] touchCurPos = new float[2];
    private float touchStartDist = 0.0f;
    private float touchCurDist = 0.0f;
    private Point screenSize = new Point();
    private float screenDiagnal = 0.0f;
    private String appVersionString;

    private int arElement = 0;
    private int interactionType = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);

        Intent intent1 = new Intent();
        intent1.setAction("android.intent.action.REQUEST_TANGO_PERMISSION");
        intent1.putExtra(EXTRA_KEY_PERMISSIONTYPE, EXTRA_VALUE_VIO);
        startActivityForResult(intent1, 0);

        Intent intent2 = new Intent();
        intent2.setAction("android.intent.action.REQUEST_TANGO_PERMISSION");
        intent2.putExtra(EXTRA_KEY_PERMISSIONTYPE, EXTRA_VALUE_VIOADF);
        startActivityForResult(intent2, 0);

        Display display = getWindowManager().getDefaultDisplay();
        display.getSize(screenSize);
        screenDiagnal = (float) Math.sqrt(screenSize.x * screenSize.x
                + screenSize.y * screenSize.y);
        
        setTitle(R.string.app_name);
        setContentView(R.layout.activity_augmented_reality);

        arView = (GLSurfaceView) findViewById(R.id.surfaceview);
        score = (TextView)findViewById(R.id.score);
        score.setTextSize(42.0f);
        romancium = Typeface.createFromAsset(getAssets(),"fonts/leopold.otf");
        score.setTypeface(romancium);
        
        arViewRenderer = new AugmentedRealityView(getApplicationContext());
        arViewRenderer.activity = AugmentedRealityActivity.this;
        arViewRenderer.isAutoRecovery = true;
        arView.setRenderer(arViewRenderer);
        arView.setOnTouchListener(new OnTouchListener(){

			@Override
			public boolean onTouch(View v, MotionEvent event) {
				// TODO Auto-generated method stub
				TangoJNINative.squashPixie();
				return true;
			}
        	
        });
        
        tangoPoseStatusText = (TextView) findViewById(R.id.debug_info);
        introVideoView = (VideoView) findViewById(R.id.videoView1);
        if (mediaControls == null) {
        	mediaControls = new MediaController(this);
        }
        imageView = (ImageView)findViewById(R.id.imageView1);
        vinesView = (ImageView)findViewById(R.id.imageView2);
        
        mediaPlayer = new MediaPlayer();
		
        try {
        	//set the media controller in the VideoView
        	//introVideoView.setMediaController(mediaControls);

        	//set the uri of the video to be played
        	introVideoView.setVideoURI(Uri.parse("android.resource://" + getPackageName() + "/" + R.raw.intro2));
        	mediaPlayer.setDataSource(getApplicationContext(), Uri.parse("android.resource://" + getPackageName() + "/" + R.raw.music_loop));
        	mediaPlayer.prepare();
        	mediaPlayer.setLooping(true);
        } catch (Exception e) {
        	Log.e("Error", e.getMessage());

        	e.printStackTrace();

        }
        introVideoView.requestFocus();
                //we also set an setOnPreparedListener in order to know when the video file is ready for playback
        introVideoView.setOnPreparedListener(new OnPreparedListener() {

		@Override
		public void onPrepared(MediaPlayer arg0) {
			introVideoView.start();
		}
    });
        introVideoView.setOnTouchListener(new OnTouchListener(){

			@Override
			public boolean onTouch(View arg0, MotionEvent arg1) {
				// TODO Auto-generated method stub
				if(introVideoView.getVisibility() == View.VISIBLE){
					introVideoView.stopPlayback();
					introVideoView.setVisibility(View.INVISIBLE);
					arView.setVisibility(View.VISIBLE);
					vinesView.setVisibility(View.VISIBLE);
					score.setVisibility(View.VISIBLE);
					
					arView.requestFocus();
					mediaPlayer.start();
					return true;
				} else {
					return false;
				}
			}
        	
        });
        
        imageView.setOnTouchListener(new OnTouchListener(){

			@Override
			public boolean onTouch(View arg0, MotionEvent arg1) {
				if(imageView.getVisibility() == View.VISIBLE){
					arView.setVisibility(View.VISIBLE);
					vinesView.setVisibility(View.VISIBLE);
					imageView.setVisibility(View.INVISIBLE);
					
					arViewRenderer.startGame();
					
					return true;
				}
				return false;
			}
        	
        });
        
        introVideoView.setOnCompletionListener(new OnCompletionListener(){

			@Override
			public void onCompletion(MediaPlayer arg0) {
				// TODO Auto-generated method stub
				introVideoView.setVisibility(View.INVISIBLE);
				arView.setVisibility(View.VISIBLE);
				vinesView.setVisibility(View.VISIBLE);
				score.setVisibility(View.VISIBLE);
				arView.requestFocus();

				mediaPlayer.start();
			}
        	
        });
        
        PackageInfo pInfo;
        try {
            pInfo = this.getPackageManager().getPackageInfo(this.getPackageName(), 0);
            appVersionString = pInfo.versionName;
        } catch (NameNotFoundException e) {
            e.printStackTrace();
            appVersionString = " ";
        }

        //findViewById(R.id.reset).setOnClickListener(this);
        //findViewById(R.id.third).setOnClickListener(this);
        //findViewById(R.id.first).setOnClickListener(this);
        //findViewById(R.id.top).setOnClickListener(this);

        new Thread(new Runnable() {
            @Override
            public void run() {
                while (true) {
                    try {
                        Thread.sleep(10);

                        runOnUiThread(new Runnable() {
                            public void run() {
                                boolean isLocalized = TangoJNINative.getIsLocalized();
                                /*if(isLocalized) {
                                    findViewById(R.id.reset).setVisibility(View.GONE);
                                } else {
                                    findViewById(R.id.reset).setVisibility(View.VISIBLE);
                                }*/
                                tangoPoseStatusText.setText(
                                    "Service Version:" + TangoJNINative.getVersionNumber() +
                                    "\nApp Version:" + appVersionString +
                                    "\n" + TangoJNINative.getPoseString());
                                
                                if(arViewRenderer.health == 0){
                                	imageView.setVisibility(View.VISIBLE);
                                	arView.setVisibility(View.VISIBLE); //TODO: Should be invisible, but leads to bugs?
                                	vinesView.setVisibility(View.INVISIBLE);
                                	TangoJNINative.stopGame();
                                	score.setText("FINAL SCORE: " +
                                    		arViewRenderer.num_dodged + " Dodged, "
                                    		+  arViewRenderer.num_squashed + " Squashed" );
                                } else {
                                	score.setText("Health: " + arViewRenderer.health + 
                                    		" Dodged: " + arViewRenderer.num_dodged + 
                                    		" Squashed: " +  arViewRenderer.num_squashed + 
                                    		" Attacking: " + (arViewRenderer.num_summoned - arViewRenderer.num_dodged 
                                    				- arViewRenderer.num_squashed - (10-arViewRenderer.health)));
                                }
                            }
                        });

                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }
        }).start();
    }
    @Override
    public void onClick(View v) {
        switch (v.getId()) {
        /*case R.id.reset:
            TangoJNINative.resetMotionTracking();
            break;
        case R.id.first:
            TangoJNINative.setCamera(0);
            break;
        case R.id.third:
            TangoJNINative.setCamera(1);
            break;
        case R.id.top:
            TangoJNINative.setCamera(2);
            break;*/
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        arView.onResume();
        mediaPlayer.start();
        getWindow().getDecorView().setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                );
    }

    @Override
    protected void onPause() {
        super.onPause();
        arView.onPause();
        mediaPlayer.pause();
        TangoJNINative.disconnectService();
    }

    protected void onDestroy() {
        super.onDestroy();
        TangoJNINative.onDestroy();
    }

    public void onRadioButtonClicked(View view) {
        switch (view.getId()) {
        /*case R.id.radio_world:
            arElement = 1;
            break;
        case R.id.radio_cube:
            arElement = 2;
            break;
        case R.id.radio_grid:
            arElement = 3;
            break;
        case R.id.radio_fx:
            arElement = 4;
            break;*/
        }
    }

    public void onDirectionButtonClicked(View view) {
        switch (view.getId()) {
        /*case R.id.radio_left:
            interactionType = 1;
            break;
        case R.id.radio_right:
            interactionType = 2;
            break;
        case R.id.radio_down:
            interactionType = 3;
            break;
        case R.id.radio_up:
            interactionType = 4;
            break;
        case R.id.radio_far:
            interactionType = 5;
            break;
        case R.id.radio_near:
            interactionType = 6;
            break;*/
        }
        if (arElement != 0) {
            TangoJNINative.updateARElement(arElement, interactionType);
        }
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        int pointCount = event.getPointerCount();
        if (pointCount == 1) {
            switch (event.getActionMasked()) {
            case MotionEvent.ACTION_DOWN: {
                TangoJNINative.startSetCameraOffset();
                touchCurDist = 0.0f;
                touchStartPos[0] = event.getX(0);
                touchStartPos[1] = event.getY(0);
                break;
            }
            case MotionEvent.ACTION_MOVE: {
                touchCurPos[0] = event.getX(0);
                touchCurPos[1] = event.getY(0);

                // Normalize to screen width.
                float normalizedRotX = (touchCurPos[0] - touchStartPos[0])
                        / screenSize.x;
                float normalizedRotY = (touchCurPos[1] - touchStartPos[1])
                        / screenSize.y;

                TangoJNINative.setCameraOffset(normalizedRotX, normalizedRotY,
                        touchCurDist / screenDiagnal);
                break;
            }
            }
        }
        if (pointCount == 2) {
            switch (event.getActionMasked()) {
            case MotionEvent.ACTION_POINTER_DOWN: {
                TangoJNINative.startSetCameraOffset();
                float absX = event.getX(0) - event.getX(1);
                float absY = event.getY(0) - event.getY(1);
                touchStartDist = (float) Math.sqrt(absX * absX + absY * absY);
                break;
            }
            case MotionEvent.ACTION_MOVE: {
                float absX = event.getX(0) - event.getX(1);
                float absY = event.getY(0) - event.getY(1);

                touchCurDist = touchStartDist
                        - (float) Math.sqrt(absX * absX + absY * absY);

                TangoJNINative.setCameraOffset(0.0f, 0.0f, touchCurDist
                        / screenDiagnal);
                break;
            }
            case MotionEvent.ACTION_POINTER_UP: {
                int index = event.getActionIndex() == 0 ? 1 : 0;
                touchStartPos[0] = event.getX(index);
                touchStartPos[1] = event.getY(index);
                break;
            }
            }
        }
        return true;
    }
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == 0) {
            if (resultCode == RESULT_CANCELED) {
                Toast.makeText(this,
                "Motion Tracking Permission Needed!", Toast.LENGTH_SHORT).show();
                finish();
            }
        }
        if (requestCode == 1) {
            if (resultCode == RESULT_CANCELED) {
                Toast.makeText(this,
                "ADF Permission Needed!", Toast.LENGTH_SHORT).show();
                finish();
            }
        }
    }
}

