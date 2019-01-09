#include "camera_process_jni.h"
#include "camera_process_lib.hpp"
#include "util/android_buf.hpp"

JNIEXPORT jint JNICALL
Java_com_gavinandre_cameraprocesslibrary_CameraProcessLib_prepareSystemCamera(
        JNIEnv *env, jclass type, jint width, jint height) {
    return prepareSystemCamera(width, height);
}

JNIEXPORT jint JNICALL
Java_com_gavinandre_cameraprocesslibrary_CameraProcessLib_prepareUsbCamera(
        JNIEnv *env, jclass type, jint width, jint height) {
    return prepareUsbCamera(width, height);
}

JNIEXPORT void JNICALL
Java_com_gavinandre_cameraprocesslibrary_CameraProcessLib_processSystemCamera(
        JNIEnv *env, jclass type, jbyteArray yuv) {
    processSystemCamera(env, yuv);
}

JNIEXPORT void JNICALL
Java_com_gavinandre_cameraprocesslibrary_CameraProcessLib_processUsbCamera
        (JNIEnv *env, jclass type) {
    processUsbCamera();
}

JNIEXPORT void JNICALL
Java_com_gavinandre_cameraprocesslibrary_CameraProcessLib_pixelToBmp(
        JNIEnv *env, jclass type, jobject bitmap) {
    pixelToBmp(env, bitmap);
}

JNIEXPORT void JNICALL
Java_com_gavinandre_cameraprocesslibrary_CameraProcessLib_releaseSystemCamera(
        JNIEnv *env, jclass type) {
    releaseSystemCamera();
}

JNIEXPORT void JNICALL
Java_com_gavinandre_cameraprocesslibrary_CameraProcessLib_releaseUsbCamera(
        JNIEnv *env, jclass type) {
    releaseUsbCamera();
}

