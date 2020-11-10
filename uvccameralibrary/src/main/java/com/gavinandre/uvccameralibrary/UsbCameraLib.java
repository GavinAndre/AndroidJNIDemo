package com.gavinandre.uvccameralibrary;

import android.graphics.Bitmap;

public class UsbCameraLib {
    
    static {
        System.loadLibrary("uvccamera");
    }
    
    public static native int connect(int index, int venderId, int productId, int fileDescriptor,
                                     int busNum, int devAddr, String usbfs);
    
    public static native void release(int index);
    
    public static native boolean setPara(int index, int autoExpo, float expoTime, float gain, int brightness);
    
    public static native boolean setHeatCoil(int index, boolean on);
    
    public static native boolean getHeatCoil(int index);
    
    public static native void setStreamMode(int streamMode);
    
    public static native boolean takePhoto(int index, String path);
    
    public static native void pixeltobmp(int index, Bitmap bitmap);
    
}
