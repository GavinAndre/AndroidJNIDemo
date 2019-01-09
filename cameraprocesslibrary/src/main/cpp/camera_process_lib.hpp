#include "util/v4l2/v4l_achieve.h"
#include "util/v4l2/v4l_lib.hpp"
#include "util/jni_lib.hpp"

#include <opencv2/opencv.hpp>
#include <android/bitmap.h>
#include <iostream>
#include <dirent.h>
#include <regex>
#include <sys/stat.h>

int IMG_WIDTH, IMG_HEIGHT, PREVIEW_IMG_WIDTH, PREVIEW_IMG_HEIGHT;

cv::Mat frame;

V4LAchieve *oV4LAchieve;

int prepareSystemCamera(int width, int height) {
    LOGE("prepareSystemCamera");
    IMG_WIDTH = width;
    IMG_HEIGHT = height;
    PREVIEW_IMG_WIDTH = height;
    PREVIEW_IMG_HEIGHT = width;
    return JNI_OK;
}

int prepareUsbCamera(int width, int height) {
    LOGE("prepareUsbCamera");
    IMG_WIDTH = width;
    IMG_HEIGHT = height;
    PREVIEW_IMG_WIDTH = width;
    PREVIEW_IMG_HEIGHT = height;
    if (oV4LAchieve == nullptr) {
        oV4LAchieve = new V4LAchieve(getVideoId(), IMG_WIDTH, IMG_HEIGHT);
        if (!oV4LAchieve->OpenCamera()) {
            LOGE("V4LAchieve OpenCamera ERROR");
            return JNI_ERR;
        }
    }
    return JNI_OK;
}

void processSystemCamera(JNIEnv *env, jbyteArray yuv) {
    //    LOGE("processSystemCamera");

    jbyte *_yuv = env->GetByteArrayElements(yuv, 0);

    cv::Mat yuvimg(IMG_HEIGHT + IMG_HEIGHT / 2, IMG_WIDTH, CV_8UC1, (uchar *) _yuv);
    cv::cvtColor(yuvimg, yuvimg, cv::COLOR_YUV420sp2RGBA);

    //flip图像旋转角度校准
    cv::transpose(yuvimg, yuvimg);
    //图像垂直镜像
    cv::flip(yuvimg, yuvimg, 0);

    frame = yuvimg;

    env->ReleaseByteArrayElements(yuv, _yuv, 0);
}

void processUsbCamera() {
    oV4LAchieve->CameraVideoGetLoop();
    cv::Mat yuvimg(IMG_HEIGHT, IMG_WIDTH, CV_8UC2, oV4LAchieve->GetpYUYV422());
    cv::cvtColor(yuvimg, yuvimg, cv::COLOR_YUV2RGBA_YUYV);

    frame = yuvimg;
}

void pixelToBmp(JNIEnv *env, jobject bitmap) {
    cv::Mat tmp = !frame.empty() ?
                  frame(cv::Rect(0, 0, PREVIEW_IMG_WIDTH, PREVIEW_IMG_HEIGHT)).clone() :
                  cv::Mat(PREVIEW_IMG_HEIGHT, PREVIEW_IMG_WIDTH, CV_8UC4,
                          cv::Scalar(255, 0, 0, 255));
    pixel2Bmp(env, bitmap, tmp.data);
}

void releaseSystemCamera() {
    LOGE("releaseSystemCamera");
}

void releaseUsbCamera() {
    LOGE("releaseUsbCamera");
    if (oV4LAchieve != nullptr) {
        oV4LAchieve->CloseCamera();
        delete oV4LAchieve;
        oV4LAchieve = nullptr;
    }
}
