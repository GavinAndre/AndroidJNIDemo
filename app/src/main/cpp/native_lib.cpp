#include <jni.h>
#include "native_lib.hpp"

extern "C" JNIEXPORT void JNICALL
Java_com_gavinandre_jnidemo_NativeLib_enableCout(JNIEnv *env, jclass type) {
    enableCout();
}

extern "C" JNIEXPORT void JNICALL
Java_com_gavinandre_jnidemo_NativeLib_disableCout(JNIEnv *env, jclass type) {
    disableCout();
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_gavinandre_jnidemo_NativeLib_stringFromJNI(JNIEnv *env, jclass type) {
    return stringFromJNI(env);
}

extern "C" JNIEXPORT void JNICALL
Java_com_gavinandre_jnidemo_NativeLib_stringToJNI(JNIEnv *env, jclass type, jstring str) {
    stringToJNI(env, str);
}

extern "C" JNIEXPORT void JNICALL
Java_com_gavinandre_jnidemo_NativeLib_listToJNI(JNIEnv *env, jclass type, jobject array_list) {
    listToJNI(env, array_list);
}

extern "C" JNIEXPORT jobject JNICALL
Java_com_gavinandre_jnidemo_NativeLib_listFromJNI(JNIEnv *env, jclass type) {
    return listFromJNI(env);
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_gavinandre_jnidemo_NativeLib_uuidFromJNI(JNIEnv *env, jclass type) {
    return uuidFromJNI(env);
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_gavinandre_jnidemo_NativeLib_getSDPath(JNIEnv *env, jclass type) {
    return getSDPath(env);
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_gavinandre_jnidemo_NativeLib_base64Encode(JNIEnv *env, jclass type) {
    return base64Encode(env);
}

extern "C" JNIEXPORT void JNICALL
Java_com_gavinandre_jnidemo_NativeLib_base64Decode(JNIEnv *env, jclass type, jstring encode_data) {
    return base64decode(env,encode_data);
}