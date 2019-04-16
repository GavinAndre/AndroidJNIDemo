#include <jni.h>
#include <iostream>
#include <vector>
#include <android/bitmap.h>
#include "android_log.h"

jstring string_to_jstring(JNIEnv *env, std::string &str) {
    return env->NewStringUTF(str.c_str());
}

std::string jstring_to_string(JNIEnv *env, jstring &j_str) {
    const char *c = env->GetStringUTFChars(j_str, nullptr);
    std::string c_str = std::string(c);
    env->ReleaseStringUTFChars(j_str, c);
    return c_str;
}

void pixel2Bmp(JNIEnv *env, jobject &bitmap, void *data, int channels) {

    AndroidBitmapInfo info;
    void *pixels;
    int ret, width = 0, height = 0;

    if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0) {
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }

    width = info.width;
    height = info.height;

    //    LOGI("pixel2Bmp width %d", width);
    //    LOGI("pixel2Bmp height %d", height);

    if (info.format != ANDROID_BITMAP_FORMAT_RGB_565 &&
        info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmap format is not RGB_565 or RGBA_8888!");
        return;
    }

    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
        return;
    }

    memcpy(pixels, data, (size_t) (width * height * channels));

    AndroidBitmap_unlockPixels(env, bitmap);
}
