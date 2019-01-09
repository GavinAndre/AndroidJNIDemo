package com.gavinandre.imageprocesslibrary;

import android.graphics.Bitmap;

/**
 * Created by gavinandre on 17-8-9.
 */

public class ImageProcess {

    static {
        System.loadLibrary("image_process_lib");
    }

    public native static String encode(String imagePath);

    public native static Bitmap decode(String imageData);

    public native static Bitmap processBitmap(Bitmap bitmap);

}
