#include <jni.h>
#include "image_process_lib.hpp"

extern "C" JNIEXPORT jobject JNICALL
Java_com_gavinandre_imageprocesslibrary_ImageProcess_pngToBitmap
        (JNIEnv *env, jclass type, jstring imagePath) {
    return pngToBitmap(env, imagePath);
}

extern "C" JNIEXPORT void JNICALL
Java_com_gavinandre_imageprocesslibrary_ImageProcess_bitmapToPng
        (JNIEnv *env, jclass type, jstring savePath, jobject srcBitmap) {
    return bitmapToPng(env, savePath, srcBitmap);
}
