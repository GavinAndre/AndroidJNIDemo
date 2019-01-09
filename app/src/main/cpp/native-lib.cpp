#include "native-lib.h"
#include "native-lib.hpp"

JNIEXPORT void JNICALL
Java_com_gavinandre_jnidemo_NativeLib_enableCout(JNIEnv *env, jclass type) {
    enableCout();
}

JNIEXPORT void JNICALL
Java_com_gavinandre_jnidemo_NativeLib_disableCout(JNIEnv *env, jclass type) {
    disableCout();
}

JNIEXPORT jstring JNICALL
Java_com_gavinandre_jnidemo_NativeLib_stringFromJNI(JNIEnv *env, jclass type) {
    return stringFromJNI(env);
}

JNIEXPORT void JNICALL
Java_com_gavinandre_jnidemo_NativeLib_stringToJNI(JNIEnv *env, jclass type, jstring str) {
    stringToJNI(env, str);
}

JNIEXPORT void JNICALL
Java_com_gavinandre_jnidemo_NativeLib_listToJNI(JNIEnv *env, jclass type, jobject array_list) {
    listToJNI(env, array_list);
}

JNIEXPORT jobject JNICALL
Java_com_gavinandre_jnidemo_NativeLib_listFromJNI(JNIEnv *env, jclass type) {
    return listFromJNI(env);
}

JNIEXPORT jstring JNICALL
Java_com_gavinandre_jnidemo_NativeLib_uuidFromJNI(JNIEnv *env, jclass type) {
    return uuidFromJNI(env);
}

JNIEXPORT jstring JNICALL
Java_com_gavinandre_jnidemo_NativeLib_getSDPath(JNIEnv *env, jclass type) {
    return getSDPath(env);
}