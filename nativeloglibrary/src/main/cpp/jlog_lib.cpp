#include <jni.h>
#include "jlog.hpp"

extern "C"
JNIEXPORT void JNICALL
Java_com_gavinandre_nativeloglibrary_JLogLib_initLog(
        JNIEnv *env, jclass clazz, jstring log_path) {
    const char *c = env->GetStringUTFChars(log_path, JNI_FALSE);
    std::string c_path = std::string(c);
    jlog::initLog(c_path);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_gavinandre_nativeloglibrary_JLogLib_closeLog(
        JNIEnv *env, jclass clazz) {
    jlog::closeLog();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_gavinandre_nativeloglibrary_JLogLib_logFromJava(
        JNIEnv *env, jclass clazz, jstring string) {
    const char *c = env->GetStringUTFChars(string, JNI_FALSE);
    std::string c_path = std::string(c);
    LOGI("jlog %s", c);
    LOGE("jlog %s", c);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_gavinandre_nativeloglibrary_JLogLib_logFromNative(
        JNIEnv *env, jclass clazz) {
    LOGI("jlog log from native");
    LOGE("jlog log from native");
}