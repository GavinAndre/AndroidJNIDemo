package com.gavinandre.rtsplibrary;

import android.util.Log;

public class RtspClient {

    private static final String TAG = RtspClient.class.getSimpleName();

    public RtspClient(NativeCallback callback) {
        if (initialize(callback) == -1) {
            Log.i(TAG, "RtspClient initialize failed");
        } else {
            Log.i(TAG, "RtspClient initialize successfully");
        }
    }

    static {
        System.loadLibrary("rtsp_lib");
    }

    private native int initialize(NativeCallback callback);

    /**
     * Play stream synchronously.
     *
     * @param endpoint resource endpoint
     * @return 0 if exit normally or -1 otherwise
     */
    public native int play(String endpoint);

    public native void stop();

    public native void dispose();
}
