#include "uvc_camera.hpp"

extern "C"
JNIEXPORT jint JNICALL
Java_com_gavinandre_uvccameralibrary_UsbCameraLib_connect(
        JNIEnv *env, jclass thiz, jint index, jint vid, jint pid, jint fd, jint busNum,
        jint devAddr, jstring usbfs_str) {
    const char *c = env->GetStringUTFChars(usbfs_str, JNI_FALSE);
    std::string c_usbfs = std::string(c);
    env->ReleaseStringUTFChars(usbfs_str, c);
    return connect(index, vid, pid, fd, busNum, devAddr, c_usbfs);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_gavinandre_uvccameralibrary_UsbCameraLib_release(
        JNIEnv *env, jclass thiz, jint index) {
    release(index);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_gavinandre_uvccameralibrary_UsbCameraLib_setPara(
        JNIEnv *env, jclass type, jint index, jint autoExpo, jfloat expoTime, jfloat gain,
        jint brightness) {
    int r = set_param(index, autoExpo == 0, expoTime, gain, brightness);
    return (r > 0);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_gavinandre_uvccameralibrary_UsbCameraLib_setHeatCoil(
        JNIEnv *env, jclass type, jint index, jboolean on) {

    int r = set_heat_coil(index, on);
    return (r > 0);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_gavinandre_uvccameralibrary_UsbCameraLib_getHeatCoil(
        JNIEnv *env, jclass type, jint index) {
    int r = get_heat_coil(index);
    return (r > 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_gavinandre_uvccameralibrary_UsbCameraLib_setStreamMode(
        JNIEnv *env, jclass thiz, jint mode) {
    set_stream_mode(mode);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_gavinandre_uvccameralibrary_UsbCameraLib_takePhoto(
        JNIEnv *env, jclass thiz, jint index, jstring path) {
    const char *c = env->GetStringUTFChars(path, JNI_FALSE);
    std::string c_path = std::string(c);
    env->ReleaseStringUTFChars(path, c);
    return take_photo(index, c_path);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_gavinandre_uvccameralibrary_UsbCameraLib_pixeltobmp(
        JNIEnv *env, jclass thiz, jint index, jobject bitmap) {
    pixel_to_bmp(env, thiz, index, bitmap);
}
