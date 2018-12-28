package com.lingzhi.imageprocesslibrary;

import android.graphics.Bitmap;

/**
 * Created by gavinandre on 17-8-9.
 */

public class ImageProcess {

    static {
        System.loadLibrary("image_process-lib");
    }

    public native static String Encode(String imagePath);

    public native static Bitmap Decode(String imageData);

    public native static Bitmap processBitmap(Bitmap bitmap);

}
