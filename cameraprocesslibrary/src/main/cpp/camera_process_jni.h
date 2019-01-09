#include <jni.h>

#ifdef  __cplusplus
extern "C" {
#endif

JNIEXPORT jboolean JNICALL
Java_com_gavinandre_cameraprocesslibrary_CameraProcessLib_initFaceAnalyzer(
        JNIEnv *env, jclass type);

JNIEXPORT jint JNICALL
Java_com_gavinandre_cameraprocesslibrary_CameraProcessLib_prepareSystemCamera(
        JNIEnv *env, jclass type, jint width, jint height);

JNIEXPORT jint JNICALL
Java_com_gavinandre_cameraprocesslibrary_CameraProcessLib_prepareUsbCamera(
        JNIEnv *env, jclass type, jint width, jint height);

JNIEXPORT void JNICALL
Java_com_gavinandre_cameraprocesslibrary_CameraProcessLib_processSystemCamera(
        JNIEnv *env, jclass type, jbyteArray yuv);

JNIEXPORT void JNICALL
Java_com_gavinandre_cameraprocesslibrary_CameraProcessLib_processUsbCamera
        (JNIEnv *env, jclass type);

JNIEXPORT void JNICALL
Java_com_gavinandre_cameraprocesslibrary_CameraProcessLib_pixelToBmp(
        JNIEnv *env, jclass type, jobject bitmap);

JNIEXPORT void JNICALL
Java_com_gavinandre_cameraprocesslibrary_CameraProcessLib_releaseSystemCamera(
        JNIEnv *env, jclass type);

JNIEXPORT void JNICALL
Java_com_gavinandre_cameraprocesslibrary_CameraProcessLib_releaseUsbCamera(
        JNIEnv *env, jclass type);

JNIEXPORT void JNICALL
Java_com_gavinandre_cameraprocesslibrary_CameraProcessLib_mxnetFace(
        JNIEnv *env, jclass type);

#ifdef __cplusplus
}
#endif
