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

cv::Mat row_frame, preview_frame;

V4LAchieve *oV4LAchieve;

int prepareSystemCamera(int width, int height) {
    LOGI("prepareSystemCamera");
    IMG_WIDTH = width;
    IMG_HEIGHT = height;
    PREVIEW_IMG_WIDTH = height;
    PREVIEW_IMG_HEIGHT = width;
    return JNI_OK;
}

int prepareUsbCamera(int width, int height) {
    LOGI("prepareUsbCamera");
    IMG_WIDTH = width;
    IMG_HEIGHT = height;
    PREVIEW_IMG_WIDTH = width;
    PREVIEW_IMG_HEIGHT = height;
    if (oV4LAchieve == nullptr) {
        int videoId = getVideoId();
        if (videoId == -1) {
            LOGE("getVideoId ERROR");
            return JNI_ERR;
        }
        oV4LAchieve = new V4LAchieve(videoId, IMG_WIDTH, IMG_HEIGHT);
        if (!oV4LAchieve->OpenCamera()) {
            LOGE("V4LAchieve OpenCamera ERROR");
            return JNI_ERR;
        }
    }
    return JNI_OK;
}

void processSystemCamera(JNIEnv *env, jbyteArray &yuv) {
    cv::Mat yuv_img;
    if (yuv != nullptr) {
        jbyte *_yuv = env->GetByteArrayElements(yuv, nullptr);
        env->ReleaseByteArrayElements(yuv, _yuv, 0);

        yuv_img = cv::Mat(IMG_HEIGHT + IMG_HEIGHT / 2, IMG_WIDTH, CV_8UC1, (uchar *) _yuv);
        cv::cvtColor(yuv_img, yuv_img, cv::COLOR_YUV420sp2BGR);

        //flip图像旋转角度校准
        cv::transpose(yuv_img, yuv_img);

        // flipCode，翻转模式:
        // flipCode == 0 垂直翻转（沿X轴翻转），
        // flipCode > 0 水平翻转（沿Y轴翻转），
        // flipCode < 0 水平垂直翻转（先沿X轴翻转，再沿Y轴翻转，等价于旋转180°）
        cv::flip(yuv_img, yuv_img, -1);
    } else {
        yuv_img = cv::Mat(IMG_HEIGHT, IMG_WIDTH, CV_8UC3, cv::Scalar(0, 0, 0));
    }
    row_frame = yuv_img;
    preview_frame = row_frame.clone();

}

void processUsbCamera() {
    oV4LAchieve->CameraVideoGetLoop();
    cv::Mat yuvimg(IMG_HEIGHT, IMG_WIDTH, CV_8UC2, oV4LAchieve->GetpYUYV422());
    cv::cvtColor(yuvimg, yuvimg, cv::COLOR_YUV2BGR_YUYV);
    row_frame = yuvimg;
    preview_frame = row_frame.clone();
}

void pixelToBmp(JNIEnv *env, jobject &bitmap) {
    cv::cvtColor(preview_frame, preview_frame, cv::COLOR_BGR2BGR565);
    pixel2Bmp(env, bitmap, preview_frame.data, preview_frame.channels());
}

void releaseSystemCamera() {
    LOGI("releaseSystemCamera");
}

void releaseUsbCamera() {
    LOGI("releaseUsbCamera");
    if (oV4LAchieve != nullptr) {
        oV4LAchieve->CloseCamera();
        delete oV4LAchieve;
        oV4LAchieve = nullptr;
    }
}
