package com.gavinandre.nativeloglibrary;

public class JLogLib {

    static {
        System.loadLibrary("jlog_lib");
    }

    public static native void initLog(String logPath);

    public static native void closeLog();

    public static native void logFromJava(String string);

    public static native void logFromNative();

}


