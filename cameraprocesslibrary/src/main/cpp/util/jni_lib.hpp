#include <jni.h>
#include <iostream>
#include <vector>
#include <android/bitmap.h>
#include "android_log.h"

jstring string_to_jstring(JNIEnv *env, std::string str) {
    return env->NewStringUTF(str.c_str());
}

std::string jstring_to_string(JNIEnv *env, jstring j_str) {
    const char *c = env->GetStringUTFChars(j_str, 0);
    std::string c_str = std::string(c);
    env->ReleaseStringUTFChars(j_str, c);
    return c_str;
}

// 定义调用java中的方法的函数
std::string callJavaStaticMethod(JNIEnv *env, std::string class_name,
                                 std::string method_name, std::string method_sign) {
    // 先找到要调用的类
    jclass clazz = env->FindClass(class_name.c_str());
    if (clazz == nullptr) {
        LOGE("find class %s error !", class_name.c_str());
        return nullptr;
    }
    // 获取java方法id
    // 参数二是调用的方法名,  参数三是方法的签名
    jmethodID id = env->GetStaticMethodID(clazz, method_name.c_str(), method_sign.c_str());
    if (id == nullptr) {
        LOGE("find method %s error !", method_name.c_str());
        return nullptr;
    }
    // 开始调用java中的静态方法
    jstring result = (jstring) env->CallStaticObjectMethod(clazz, id); //调用java方法
    // 释放资源
    env->DeleteLocalRef(clazz);
    // 转换jstring为string用于显示
    return jstring_to_string(env, result);
}

void pixel2Bmp(JNIEnv *env, jobject &bitmap, void *data) {

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

    if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Bitmap format is not RGBA_8888 !");
        return;
    }

    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0) {
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }

    memcpy(pixels, data, (size_t) (width * height * 4));

    AndroidBitmap_unlockPixels(env, bitmap);
}