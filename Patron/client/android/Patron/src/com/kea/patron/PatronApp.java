// Copyright (c) 2014 Mitch T. Besser, All rights reserved.

package com.kea.patron;

import android.app.Application;
import android.content.Context;

/**
 * @author bessermt
 *
 */
public final class PatronApp extends Application {

	private static Context context_;

	public PatronApp() {
		// TODO Auto-generated constructor stub
	}

	public void onCreate(){
		super.onCreate();
		context_ = getApplicationContext();
	}

	public static Context getAppContext() {
		final Context result = context_;
		return result;
	}
}
