#include "image_process_jni.h"
#include "image_process_lib.hpp"

JNIEXPORT jstring JNICALL
Java_com_gavinandre_imageprocesslibrary_ImageProcess_encode
        (JNIEnv *env, jclass type, jstring imagePath) {
    return encode(env, imagePath);
}

JNIEXPORT jobject JNICALL
Java_com_gavinandre_imageprocesslibrary_ImageProcess_decode
        (JNIEnv *env, jclass type, jstring imageData) {
    return decode(env, imageData);
}

JNIEXPORT jobject JNICALL
Java_com_gavinandre_imageprocesslibrary_ImageProcess_processBitmap
        (JNIEnv *env, jclass type, jobject srcBitmap) {
    return processBitmap(env, srcBitmap);
}
