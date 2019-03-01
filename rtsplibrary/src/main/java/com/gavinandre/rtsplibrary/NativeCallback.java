package com.gavinandre.rtsplibrary;

public interface NativeCallback {
    void onFrame(byte[] frame, int nChannel, int width, int height);
}
