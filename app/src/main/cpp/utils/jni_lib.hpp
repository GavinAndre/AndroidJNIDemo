#include <jni.h>
#include <iostream>
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

std::string callJavaStaticMethod(JNIEnv *env, const std::string &class_name,
                                 const std::string &method_name, const std::string &method_sign) {
    // 先找到要调用的类
    jclass clazz = env->FindClass(class_name.c_str());
    if (clazz == nullptr) {
        LOGE("find class %s error !", class_name.c_str());
        return nullptr;
    }
    // 获取java方法id
    // 参数二是调用的方法名,参数三是方法的签名
    jmethodID id = env->GetStaticMethodID(clazz, method_name.c_str(), method_sign.c_str());
    if (id == nullptr) {
        LOGE("find method %s error !", method_name.c_str());
        return nullptr;
    }
    // 开始调用java中的静态方法
    jstring result = reinterpret_cast<jstring>(env->CallStaticObjectMethod(clazz, id));
    // 释放资源
    env->DeleteLocalRef(clazz);
    // 将jstring转换为string
    return jstring_to_string(env, result);
}