//
// Created by gavinandre on 17-8-9.
//

#ifndef DISINFECTIONROBOT_IMAGEPROCESS_H
#define DISINFECTIONROBOT_IMAGEPROCESS_H

#include <jni.h>
#include <android/log.h>
#include <android/bitmap.h>
#include <opencv2/opencv.hpp>
#include "base64.h"
#include <string>
#include <vector>

#define  LOG_TAG    "LibImageProcess"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

jobject createBitmap(JNIEnv *env, cv::Mat &pngimage);

void bmp2mat(JNIEnv *env, jobject &srcBitmap, cv::Mat &srcMat);

jobject mat2bmp(JNIEnv *env, cv::Mat &src, const jobject &bitmap, bool needPremultiplyAlpha);

#ifdef __cplusplus
extern "C" {
#endif

jstring Java_com_lingzhi_imageprocesslibrary_ImageProcess_Encode(JNIEnv *env, jobject,
                                                                 jstring imagePath);

jobject
Java_com_lingzhi_imageprocesslibrary_ImageProcess_Decode(JNIEnv *env, jobject, jstring imageData);

jobject
Java_com_lingzhi_imageprocesslibrary_ImageProcess_processBitmap(JNIEnv *env, jobject,
                                                                jobject srcBitmap);

#ifdef __cplusplus
}
#endif

#endif //DISINFECTIONROBOT_IMAGEPROCESS_H
