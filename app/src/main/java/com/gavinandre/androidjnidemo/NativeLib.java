package com.gavinandre.androidjnidemo;

import com.gavinandre.androidjnidemo.bean.ComplexBean;
import com.gavinandre.androidjnidemo.bean.SimpleBean;

import java.util.ArrayList;

public class NativeLib {

    static {
        System.loadLibrary("native_lib");
    }

    public static native void enableCout();

    public static native void disableCout();

    public static native String stringFromJNI();

    public static native void stringToJNI(String string);

    public static native void intListToJNI(ArrayList list);

    public static native void doubleListToJNI(ArrayList list);

    public static native void stringListToJNI(ArrayList list);

    public static native void simpleObjectListToJNI(ArrayList list);

    public static native void complexObjectListToJNI(ArrayList list);

    public static native ArrayList<Integer> intListFromJNI();

    public static native ArrayList<Double> doubleListFromJNI();

    public static native ArrayList<String> stringListFromJNI();

    public static native ArrayList<SimpleBean> simpleObjectListFromJNI();

    public static native ArrayList<ComplexBean> complexObjectListFromJNI();

    public static native String uuidFromJNI();

    public static native String getSDPath();

    public static native String base64Encode();

    public static native void base64Decode(String string);

}


