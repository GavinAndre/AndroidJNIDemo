package com.gavinandre.cameraprocesslibrary;

import android.graphics.Bitmap;

/**
 * Created by gavinandre on 17-5-23.
 */

public class CameraProcessLib {
    
    static {
        System.loadLibrary("camera_process_lib");
    }
    
    public static native int prepareSystemCamera(int width, int height);
    
    public static native int prepareUsbCamera(int width, int height);
    
    public static native void processSystemCamera(byte[] data);
    
    public static native void processUsbCamera();
    
    public static native void releaseSystemCamera();
    
    public static native void releaseUsbCamera();
    
    public static native void pixelToBmp(Bitmap bitmap);
    
}
