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
package com.gt.sdk;

import java.util.Arrays;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;

import com.facebook.CallbackManager;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.FacebookSdk;
import com.facebook.login.LoginManager;
import com.facebook.login.LoginResult;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

public class AppActivity extends Cocos2dxActivity {

	private static AppActivity mActivity = null;

	CallbackManager callbackManager;

	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		mActivity = this;

		// printKeyHash();

		FacebookSdk.sdkInitialize(getApplicationContext());
		callbackManager = CallbackManager.Factory.create();
		LoginManager.getInstance().registerCallback(callbackManager,
				new FacebookCallback<LoginResult>() {

					@Override
					public void onSuccess(LoginResult result) {
						String token = "";
						if (result != null) {
							token = result.getAccessToken().getToken();
							Log.i("DEBUG", "Login token :" + token);
						}
						nativeLoginFB(token);
					}

					@Override
					public void onError(FacebookException error) {
						Log.e("DEBUG", error.getMessage());
					}

					@Override
					public void onCancel() {
					}
		});
	}

	public Cocos2dxGLSurfaceView onCreateView() {
		Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
		// gt should create stencil buffer
		glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);

		return glSurfaceView;
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		callbackManager.onActivityResult(requestCode, resultCode, data);
	}

	static {
		System.loadLibrary("cocos2dcpp");
	}
	
	public static void loginFacebook() {
		Log.i("DEBUG", "Login FB");
		mActivity.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				// login
				LoginManager.getInstance().logInWithReadPermissions(mActivity,
						Arrays.asList("public_profile", "user_friends"));
			}
		});
	}
	
	public static void logoutFacebook() {
		mActivity.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				// logout
				LoginManager.getInstance().logOut();
			}		
		});
	}
	
	private static native void nativeLoginFB(String jtoken);
}
