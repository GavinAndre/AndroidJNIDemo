package com.gavinandre.cameraprocesslibrary.utils;

import android.os.Environment;
import android.support.annotation.Keep;

import java.io.File;

/**
 * Created by gavinandre on 18-3-19.
 */

public class CameraProcessUtil {

    @Keep
    public static String getSDPath() {
        File sdDir = null;
        boolean sdCardExist = Environment.getExternalStorageState()
                .equals(Environment.MEDIA_MOUNTED);   //判断sd卡是否存在
        if (sdCardExist) {
            sdDir = Environment.getExternalStorageDirectory();//获取跟目录
        }
        return sdDir.toString();
    }

}
