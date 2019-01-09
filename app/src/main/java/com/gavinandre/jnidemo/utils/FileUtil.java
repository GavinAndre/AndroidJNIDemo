package com.gavinandre.jnidemo.utils;

import android.os.Environment;
import android.support.annotation.Keep;

import java.io.File;

public class FileUtil {

    @Keep
    public static String getSDPath() {
        File sdDir = null;
        //判断sd卡是否存在
        boolean sdCardExist = Environment.getExternalStorageState()
                .equals(Environment.MEDIA_MOUNTED);
        //获取根目录
        if (sdCardExist) {
            sdDir = Environment.getExternalStorageDirectory();
        }
        return sdDir != null ? sdDir.toString() : null;
    }

}