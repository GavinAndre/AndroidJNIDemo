package com.gavinandre.jnidemo;

public class NativeLib {

    static {
        System.loadLibrary("native-lib");
    }

    public static native void enableCout();

    public static native void disableCout();

    public static native String stringFromJNI();

    public static native void stringToJNI(String string);

}


