#include <jni.h>
#include "native_lib.hpp"

extern "C" JNIEXPORT void JNICALL
Java_com_gavinandre_androidjnidemo_NativeLib_enableCout(JNIEnv *env, jclass type) {
    enableCout();
}

extern "C" JNIEXPORT void JNICALL
Java_com_gavinandre_androidjnidemo_NativeLib_disableCout(JNIEnv *env, jclass type) {
    disableCout();
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_gavinandre_androidjnidemo_NativeLib_stringFromJNI(JNIEnv *env, jclass type) {
    return stringFromJNI(env);
}

extern "C" JNIEXPORT void JNICALL
Java_com_gavinandre_androidjnidemo_NativeLib_stringToJNI(JNIEnv *env, jclass type, jstring str) {
    stringToJNI(env, str);
}

extern "C" JNIEXPORT void JNICALL
Java_com_gavinandre_androidjnidemo_NativeLib_intListToJNI(JNIEnv *env, jclass type, jobject array_list) {
    intListToJNI(env, array_list);
}

extern "C" JNIEXPORT void JNICALL
Java_com_gavinandre_androidjnidemo_NativeLib_doubleListToJNI(JNIEnv *env, jclass type, jobject array_list) {
    doubleListToJNI(env, array_list);
}

extern "C" JNIEXPORT void JNICALL
Java_com_gavinandre_androidjnidemo_NativeLib_stringListToJNI(JNIEnv *env, jclass type, jobject array_list) {
    stringListToJNI(env, array_list);
}

extern "C" JNIEXPORT void JNICALL
Java_com_gavinandre_androidjnidemo_NativeLib_simpleObjectListToJNI(JNIEnv *env, jclass type, jobject array_list) {
    simpleObjectListToJNI(env, array_list);
}

extern "C" JNIEXPORT void JNICALL
Java_com_gavinandre_androidjnidemo_NativeLib_complexObjectListToJNI(JNIEnv *env, jclass type, jobject array_list) {
    complexObjectListToJNI(env, array_list);
}

extern "C" JNIEXPORT jobject JNICALL
Java_com_gavinandre_androidjnidemo_NativeLib_intListFromJNI(JNIEnv *env, jclass type) {
    return intListFromJNI(env);
}

extern "C" JNIEXPORT jobject JNICALL
Java_com_gavinandre_androidjnidemo_NativeLib_doubleListFromJNI(JNIEnv *env, jclass type) {
    return doubleListFromJNI(env);
}

extern "C" JNIEXPORT jobject JNICALL
Java_com_gavinandre_androidjnidemo_NativeLib_stringListFromJNI(JNIEnv *env, jclass type) {
    return stringListFromJNI(env);
}

extern "C" JNIEXPORT jobject JNICALL
Java_com_gavinandre_androidjnidemo_NativeLib_simpleObjectListFromJNI(JNIEnv *env, jclass type) {
    return simpleObjectListFromJNI(env);
}

extern "C" JNIEXPORT jobject JNICALL
Java_com_gavinandre_androidjnidemo_NativeLib_complexObjectListFromJNI(JNIEnv *env, jclass type) {
    return complexObjectListFromJNI(env);
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_gavinandre_androidjnidemo_NativeLib_uuidFromJNI(JNIEnv *env, jclass type) {
    return uuidFromJNI(env);
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_gavinandre_androidjnidemo_NativeLib_getSDPath(JNIEnv *env, jclass type) {
    return getSDPath(env);
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_gavinandre_androidjnidemo_NativeLib_base64Encode(JNIEnv *env, jclass type) {
    return base64Encode(env);
}

extern "C" JNIEXPORT void JNICALL
Java_com_gavinandre_androidjnidemo_NativeLib_base64Decode(JNIEnv *env, jclass type, jstring encode_data) {
    return base64decode(env,encode_data);
}