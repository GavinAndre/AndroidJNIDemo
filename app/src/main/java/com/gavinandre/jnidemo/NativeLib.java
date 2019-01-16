package com.gavinandre.jnidemo;

import java.util.ArrayList;

public class NativeLib {

    static {
        System.loadLibrary("native-lib");
    }

    public static native void enableCout();

    public static native void disableCout();

    public static native String stringFromJNI();

    public static native void stringToJNI(String string);

    public static native void listToJNI(ArrayList list);

    public static native ArrayList<String> listFromJNI();

    public static native String uuidFromJNI();

    public static native String getSDPath();

    public static native String base64Encode();

    public static native void base64Decode(String string);

}


