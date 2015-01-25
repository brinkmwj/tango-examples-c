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

import java.util.HashMap;
import java.util.Random;

import android.content.Context;
import android.media.AudioManager;
import android.media.SoundPool;
import android.opengl.GLSurfaceView;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * AugmentedRealityView renders graphic content.
 */
public class AugmentedRealityView implements GLSurfaceView.Renderer {

    public boolean isAutoRecovery;
    public AugmentedRealityActivity activity;

    private SoundPool mShortPlayer= null;
    private HashMap mSounds = new HashMap();
    private Context pContext;
    
    private Random r;
    
    public AugmentedRealityView(Context applicationContext) {
		// TODO Auto-generated constructor stub
    	pContext = applicationContext;
    	r = new Random();
	}

	public void setupSoundPoolPlayer()
    {
        // setup Soundpool
        this.mShortPlayer = new SoundPool(4, AudioManager.STREAM_MUSIC, 0);
        
        mSounds.put(R.raw.oot_navi_bonk, this.mShortPlayer.load(pContext, R.raw.oot_navi_bonk, 1));
        mSounds.put(R.raw.oot_navi_hey5, this.mShortPlayer.load(pContext, R.raw.oot_navi_hey5, 1));
        mSounds.put(R.raw.oot_navi_in, this.mShortPlayer.load(pContext, R.raw.oot_navi_in, 1));
        mSounds.put(R.raw.oot_navi_out, this.mShortPlayer.load(pContext, R.raw.oot_navi_out, 1));
        mSounds.put(R.raw.oot_navi_watchout5, this.mShortPlayer.load(pContext, R.raw.oot_navi_watchout5, 1));
        mSounds.put(R.raw.whew, this.mShortPlayer.load(pContext, R.raw.whew, 1));
        mSounds.put(R.raw.auuch, this.mShortPlayer.load(pContext, R.raw.auuch, 1));
        mSounds.put(R.raw.missed_it, this.mShortPlayer.load(pContext, R.raw.missed_it, 1));
        mSounds.put(R.raw.gotcha, this.mShortPlayer.load(pContext, R.raw.gotcha, 1));
        mSounds.put(R.raw.squish, this.mShortPlayer.load(pContext, R.raw.squish, 1));
        mSounds.put(R.raw.summon, this.mShortPlayer.load(pContext, R.raw.summon, 1));
    }

    public void playShortResource(int piResource) {
        int iSoundId = (Integer) mSounds.get(piResource);
        float volume = 0.99f;
        if(piResource == R.raw.summon){
        	volume = 0.5f;
        }
        this.mShortPlayer.play(iSoundId, volume, volume, 0, 0, 1);
    }

    // Cleanup
    public void release() {
        // Cleanup
        this.mShortPlayer.release();
        this.mShortPlayer = null;
    }
    
    public void onDrawFrame(GL10 gl) {
        TangoJNINative.render();
        if(TangoJNINative.getPixieCreated() > 0){
        	playShortResource(R.raw.oot_navi_hey5);
        	//playShortResource(R.raw.summon);
        }
        if(TangoJNINative.getPixieAttacked() > 0){
        	playShortResource(R.raw.oot_navi_watchout5);
        }
        if(TangoJNINative.getPixieMissed() > 0){
        	playShortResource(R.raw.oot_navi_in);
        	
        	if(r.nextFloat() < 0.05){
        		playShortResource(R.raw.missed_it);
        	} else {
        		playShortResource(R.raw.whew);
        	}
        }
        if(TangoJNINative.getPixieBit() > 0){
        	playShortResource(R.raw.oot_navi_bonk);
        	playShortResource(R.raw.auuch);
        }
        if(TangoJNINative.getPixieSquashed() > 0){
        	playShortResource(R.raw.gotcha);
        	playShortResource(R.raw.squish);
        }
    }

    public void onSurfaceChanged(GL10 gl, int width, int height) {
        TangoJNINative.connectTexture();
        TangoJNINative.connectService();
        TangoJNINative.setupViewport(width, height);
    }

    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
    	setupSoundPoolPlayer();
        TangoJNINative.setupGraphic();
        TangoJNINative.initialize(activity);
        TangoJNINative.setupConfig(isAutoRecovery);
    }
}
