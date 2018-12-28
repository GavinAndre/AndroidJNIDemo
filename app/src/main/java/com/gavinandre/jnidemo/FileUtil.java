package com.gavinandre.jnidemo;

import android.content.Context;
import android.graphics.Bitmap;
import android.os.Environment;

import java.io.File;
import java.io.FileOutputStream;

/**
 * Created by gavinandre on 17-6-26.
 */

public class FileUtil {

    private Context context;

    private FileUtil() {
    }

    /**
     * 在访问SingletonHolder时创建单例
     */
    private static class SingletonHolder {
        private static final FileUtil INSTANCE = new FileUtil();
    }

    /**
     * 获取单例
     */
    public static FileUtil getInstance() {
        return SingletonHolder.INSTANCE;
    }

    /**
     * 初始化
     *
     * @param context
     */
    public void init(Context context) {
        this.context = context;
    }

    /**
     * 获取包名目录的Pictures文件夹路径
     */
    public String getPicturePath() {
        boolean hasSDCard = Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED);
        if (hasSDCard) {
            return context.getExternalFilesDir(Environment.DIRECTORY_PICTURES).getAbsolutePath();
        } else {
            return null;
        }
    }

    /**
     * 保存地图文件并压缩成png格式
     */
    public void saveMap(Bitmap mBitmap, String fileName) {
        try {
            File file = new File(fileName + ".png");
            FileOutputStream out = new FileOutputStream(file);
            mBitmap.compress(Bitmap.CompressFormat.PNG, 100, out);
            //mBitmap.compress(Bitmap.CompressFormat.JPEG, 100, out);
            out.flush();
            out.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * 获取外部存储目录
     */
    public String getSDPath() {
        File sdDir = null;
        boolean sdCardExist = Environment.getExternalStorageState().equals("mounted");
        if (sdCardExist) {
            sdDir = Environment.getExternalStorageDirectory();
        }

        return sdDir != null ? sdDir.toString() : null;
    }

    /**
     * 保存文件
     */
    public void save(String filename, String fileContent) throws Exception {
        File file = new File(Environment.getExternalStorageDirectory(), filename);
        FileOutputStream output = new FileOutputStream(file);
        //将String字符串以字节流的形式写入到输出流中
        output.write(fileContent.getBytes());
        //关闭输出流
        output.close();
    }
}
