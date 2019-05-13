#include "camera_process_lib.hpp"
#include "util/android_buf.hpp"

extern "C" JNIEXPORT jint JNICALL
Java_com_gavinandre_cameraprocesslibrary_CameraProcessLib_prepareSystemCamera(
        JNIEnv *env, jclass type, jint width, jint height) {
    return prepareSystemCamera(width, height);
}

extern "C" JNIEXPORT jint JNICALL
Java_com_gavinandre_cameraprocesslibrary_CameraProcessLib_prepareUsbCamera(
        JNIEnv *env, jclass type, jint width, jint height) {
    return prepareUsbCamera(width, height);
}

extern "C" JNIEXPORT void JNICALL
Java_com_gavinandre_cameraprocesslibrary_CameraProcessLib_processSystemCamera(
        JNIEnv *env, jclass type, jbyteArray yuv) {
    processSystemCamera(env, yuv);
}

extern "C" JNIEXPORT void JNICALL
Java_com_gavinandre_cameraprocesslibrary_CameraProcessLib_processSystemCamera1(
        JNIEnv *env, jclass type, jbyteArray yuv) {
    processSystemCamera1(env, yuv);
}

extern "C" JNIEXPORT void JNICALL
Java_com_gavinandre_cameraprocesslibrary_CameraProcessLib_processUsbCamera
        (JNIEnv *env, jclass type) {
    processUsbCamera();
}

extern "C" JNIEXPORT void JNICALL
Java_com_gavinandre_cameraprocesslibrary_CameraProcessLib_pixelToBmp(
        JNIEnv *env, jclass type, jobject bitmap) {
    pixelToBmp(env, bitmap);
}

extern "C" JNIEXPORT void JNICALL
Java_com_gavinandre_cameraprocesslibrary_CameraProcessLib_pixelToBmp1(
        JNIEnv *env, jclass type, jobject bitmap) {
    pixelToBmp1(env, bitmap);
}

extern "C" JNIEXPORT void JNICALL
Java_com_gavinandre_cameraprocesslibrary_CameraProcessLib_releaseSystemCamera(
        JNIEnv *env, jclass type) {
    releaseSystemCamera();
}

extern "C" JNIEXPORT void JNICALL
Java_com_gavinandre_cameraprocesslibrary_CameraProcessLib_releaseUsbCamera(
        JNIEnv *env, jclass type) {
    releaseUsbCamera();
}

