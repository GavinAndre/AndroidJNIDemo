package com.gavinandre.imageprocesslibrary;

import android.graphics.Bitmap;

/**
 * Created by gavinandre on 17-8-9.
 */

public class ImageProcess {

    static {
        System.loadLibrary("image_process_lib");
    }

    public native static Bitmap pngToBitmap(String imageData);

    public native static void bitmapToPng(String savePath, Bitmap bitmap);

}
