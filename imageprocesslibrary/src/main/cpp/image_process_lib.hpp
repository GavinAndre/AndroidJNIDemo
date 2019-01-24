#include "utils/jni_lib.hpp"

#include <android/bitmap.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs/imgcodecs_c.h>

void bmp2mat(JNIEnv *env, jobject &srcBitmap, cv::Mat &dstMat, bool needPremultiplyAlpha) {
    void *srcPixels = 0;
    AndroidBitmapInfo srcBitmapInfo;
    try {
        CV_Assert(AndroidBitmap_getInfo(env, srcBitmap, &srcBitmapInfo) >= 0);
        CV_Assert(srcBitmapInfo.format == ANDROID_BITMAP_FORMAT_RGBA_8888 ||
                  srcBitmapInfo.format == ANDROID_BITMAP_FORMAT_RGB_565);
        CV_Assert(AndroidBitmap_lockPixels(env, srcBitmap, &srcPixels) >= 0);
        CV_Assert(srcPixels);
        uint32_t srcHeight = srcBitmapInfo.height;
        uint32_t srcWidth = srcBitmapInfo.width;
        dstMat.create(srcHeight, srcWidth, CV_8UC4);
        if (srcBitmapInfo.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
            LOGI("RGBA_8888");
            cv::Mat tmp(srcHeight, srcWidth, CV_8UC4, srcPixels);
            if (needPremultiplyAlpha) {
                cvtColor(tmp, dstMat, cv::COLOR_mRGBA2RGBA);
            } else {
                tmp.copyTo(dstMat);
            }
        } else {
            LOGI("RGB_565");
            cv::Mat tmp = cv::Mat(srcHeight, srcWidth, CV_8UC2, srcPixels);
            cvtColor(tmp, dstMat, cv::COLOR_BGR5652RGBA);
        }
        AndroidBitmap_unlockPixels(env, srcBitmap);
    } catch (cv::Exception &e) {
        AndroidBitmap_unlockPixels(env, srcBitmap);
        jclass je = env->FindClass("java/lang/Exception");
        env->ThrowNew(je, e.what());
    } catch (...) {
        AndroidBitmap_unlockPixels(env, srcBitmap);
        jclass je = env->FindClass("java/lang/Exception");
        env->ThrowNew(je, "unknown");
    }
}

void mat2bmp(JNIEnv *env, cv::Mat &src, const jobject &bitmap, bool needPremultiplyAlpha) {
    AndroidBitmapInfo info;
    void *pixels = 0;

    try {
        CV_Assert(AndroidBitmap_getInfo(env, bitmap, &info) >= 0);
        CV_Assert(src.type() == CV_8UC1 || src.type() == CV_8UC3 || src.type() == CV_8UC4);
        CV_Assert(AndroidBitmap_lockPixels(env, bitmap, &pixels) >= 0);
        CV_Assert(pixels);
        if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
            LOGI("RGBA_8888");
            cv::Mat tmp(info.height, info.width, CV_8UC4, pixels);
            if (src.type() == CV_8UC1) {
                LOGI("CV_8UC1");
                cvtColor(src, tmp, CV_GRAY2RGBA);
            } else if (src.type() == CV_8UC3) {
                LOGI("CV_8UC3");
                cvtColor(src, tmp, CV_BGR2RGBA);
            } else if (src.type() == CV_8UC4) {
                LOGI("CV_8UC4");
                if (needPremultiplyAlpha) {
                    cvtColor(src, tmp, cv::COLOR_RGBA2mRGBA);
                } else {
                    src.copyTo(tmp);
                }
            }
        } else {
            LOGI("RGB_565");
            // info.format == ANDROID_BITMAP_FORMAT_RGB_565
            cv::Mat tmp(info.height, info.width, CV_8UC2, pixels);
            if (src.type() == CV_8UC1) {
                LOGI("CV_8UC1");
                cvtColor(src, tmp, CV_GRAY2BGR565);
            } else if (src.type() == CV_8UC3) {
                LOGI("CV_8UC3");
                cvtColor(src, tmp, CV_BGR2BGR565);
            } else if (src.type() == CV_8UC4) {
                LOGI("CV_8UC4");
                cvtColor(src, tmp, CV_RGBA2BGR565);
            }
        }
        AndroidBitmap_unlockPixels(env, bitmap);
    } catch (cv::Exception &e) {
        AndroidBitmap_unlockPixels(env, bitmap);
        jclass je = env->FindClass("org/opencv/core/CvException");
        if (!je) je = env->FindClass("java/lang/Exception");
        env->ThrowNew(je, e.what());
    } catch (...) {
        AndroidBitmap_unlockPixels(env, bitmap);
        jclass je = env->FindClass("java/lang/Exception");
        env->ThrowNew(je, "Unknown exception in JNI code {nMatToBitmap}");
    }
}

jobject createBitmap(JNIEnv *env, cv::Mat &pngimage) {
    // Image Details
    int imgWidth = pngimage.cols;
    int imgHeight = pngimage.rows;
    int numPix = imgWidth * imgHeight;
    // Creaing Bitmap Config Class
    jclass bmpCfgCls = env->FindClass("android/graphics/Bitmap$Config");
    jmethodID bmpClsValueOfMid = env->GetStaticMethodID(bmpCfgCls, "valueOf",
                                                        "(Ljava/lang/String;)Landroid/graphics/Bitmap$Config;");
    jobject jBmpCfg = env->CallStaticObjectMethod(bmpCfgCls, bmpClsValueOfMid,
                                                  env->NewStringUTF("RGB_565" /*or*/ /*"ARGB_8888"*/));
    // Creating a Bitmap Class
    jclass bmpCls = env->FindClass("android/graphics/Bitmap");
    jmethodID createBitmapMid = env->GetStaticMethodID(bmpCls, "createBitmap",
                                                       "(IILandroid/graphics/Bitmap$Config;)Landroid/graphics/Bitmap;");
    return env->CallStaticObjectMethod(bmpCls, createBitmapMid, imgWidth, imgHeight, jBmpCfg);
}

jobject pngToBitmap(JNIEnv *env, jstring &imagePath) {
    std::string path = jstring_to_string(env, imagePath);
    LOGI("imagePath: %s", path.c_str());
    cv::Mat pngMat = cv::imread(path);
    jobject bitmap = createBitmap(env, pngMat);
    mat2bmp(env, pngMat, bitmap, false);
    return bitmap;
}

void bitmapToPng(JNIEnv *env, jstring &savePath, jobject &srcBitmap) {
    std::string path = jstring_to_string(env, savePath);
    LOGI("imagePath: %s", path.c_str());
    cv::Mat pngMat;
    bmp2mat(env, srcBitmap, pngMat, false);
    std::vector<int> param;
    param.push_back(CV_IMWRITE_PNG_COMPRESSION);
    param.push_back(9);
    cv::cvtColor(pngMat, pngMat, CV_RGBA2BGR);
    cv::imwrite(path, pngMat, param);
    LOGI("bitmapToPng complete");
}
