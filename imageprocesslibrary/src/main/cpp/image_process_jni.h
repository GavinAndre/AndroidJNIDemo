#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jstring JNICALL
Java_com_gavinandre_imageprocesslibrary_ImageProcess_encode
        (JNIEnv *env, jclass type, jstring imagePath);

JNIEXPORT jobject JNICALL
Java_com_gavinandre_imageprocesslibrary_ImageProcess_decode
        (JNIEnv *env, jclass type, jstring imageData);

JNIEXPORT jobject JNICALL
Java_com_gavinandre_imageprocesslibrary_ImageProcess_processBitmap
        (JNIEnv *env, jclass type, jobject srcBitmap);

#ifdef __cplusplus
}
#endif
