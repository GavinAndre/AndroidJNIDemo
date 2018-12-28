//
// Created by gavinandre on 17-8-9.
//

#include "image_process.h"

jstring charTojstring(JNIEnv *env, const char *pat) {
    //定义java String类 strClass
    jclass strClass = env->FindClass("Ljava/lang/String;");
    //获取String(byte[],String)的构造器,用于将本地byte[]数组转换为一个新String
    jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
    //建立byte数组
    jbyteArray bytes = env->NewByteArray(strlen(pat));
    //将char* 转换为byte数组
    env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte *) pat);
    // 设置String, 保存语言类型,用于byte数组转换至String时的参数
    jstring encoding = env->NewStringUTF("utf-8");
    //将byte数组转换为java String,并输出
    return (jstring) env->NewObject(strClass, ctorID, bytes, encoding);
}

char *jstringToChar(JNIEnv *env, jstring jstr) {
    char *rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("utf-8");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte *ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0) {
        rtn = (char *) malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    return rtn;
}

jstring Java_com_lingzhi_imageprocesslibrary_ImageProcess_Encode(JNIEnv *env, jobject,
                                                                 jstring imagePath) {

    LOGI("Encode %s", jstringToChar(env, imagePath));

    cv::Mat img_encode;
    img_encode = cv::imread(jstringToChar(env, imagePath), CV_LOAD_IMAGE_COLOR);
    std::vector<uchar> data_encode;
    std::vector<int> param = std::vector<int>(2);
    param[0] = CV_IMWRITE_PNG_COMPRESSION;
    param[1] = 9;//default(3)  0-9.
    cv::imencode(".png", img_encode, data_encode, param);
    std::string str_encode(data_encode.begin(), data_encode.end());

    std::string encoded = base64_encode(reinterpret_cast<const unsigned char *>(str_encode.c_str()),
                                        str_encode.length());

    LOGI("Encode %s", encoded.c_str());
    LOGI("Encode %d", encoded.size());

    return env->NewStringUTF(encoded.c_str());
}

jobject
Java_com_lingzhi_imageprocesslibrary_ImageProcess_Decode(JNIEnv *env, jobject,
                                                         jstring imageData) {
    LOGI("Decode");

    std::string decoded = base64_decode(jstringToChar(env, imageData));
    std::vector<uchar> data_decode;
    data_decode.resize(decoded.size());
    data_decode.assign(decoded.begin(), decoded.end());
    cv::Mat pngimage = cv::imdecode(cv::Mat(data_decode), CV_LOAD_IMAGE_COLOR);

    LOGI("pngimage.cols %d , pngimage.rows %d", pngimage.cols, pngimage.rows);
    LOGI("pngimage.channels %d , pngimage.type %d", pngimage.channels(), pngimage.type());

    jobject bitmap = createBitmap(env, pngimage);

    return mat2bmp(env, pngimage, bitmap, false);

}

jobject
Java_com_lingzhi_imageprocesslibrary_ImageProcess_processBitmap(JNIEnv *env, jobject,
                                                                jobject srcBitmap) {
    cv::Mat srcBitmapMat;
    if (srcBitmap == nullptr) {
        LOGE("srcBitmap== nullptr");
    }
    LOGI("srcBitmap!= nullptr");
    bmp2mat(env, srcBitmap, srcBitmapMat);
    LOGI("bmp2mat");
    jobject bitmap = createBitmap(env, srcBitmapMat);
    return mat2bmp(env, srcBitmapMat, bitmap, false);

}

void bmp2mat(JNIEnv *env, jobject &srcBitmap, cv::Mat &srcMat) {
    void *srcPixels = 0;
    AndroidBitmapInfo srcBitmapInfo;
    try {
        AndroidBitmap_getInfo(env, srcBitmap, &srcBitmapInfo);
        AndroidBitmap_lockPixels(env, srcBitmap, &srcPixels);
        uint32_t srcHeight = srcBitmapInfo.height;
        uint32_t srcWidth = srcBitmapInfo.width;
        srcMat.create(srcHeight, srcWidth, CV_8UC4);
        if (srcBitmapInfo.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
            LOGI("ANDROID_BITMAP_FORMAT_RGBA_8888");
            cv::Mat tmp(srcHeight, srcWidth, CV_8UC4, srcPixels);
            tmp.copyTo(srcMat);
        } else {
            LOGI("else");
            cv::Mat tmp = cv::Mat(srcHeight, srcWidth, CV_8UC2, srcPixels);
            cvtColor(tmp, srcMat, cv::COLOR_BGR5652RGBA);
        }
        AndroidBitmap_unlockPixels(env, srcBitmap);
        return;
    } catch (cv::Exception &e) {
        AndroidBitmap_unlockPixels(env, srcBitmap);
        jclass je = env->FindClass("java/lang/Exception");
        env->ThrowNew(je, e.what());
        return;
    } catch (...) {
        AndroidBitmap_unlockPixels(env, srcBitmap);
        jclass je = env->FindClass("java/lang/Exception");
        env->ThrowNew(je, "unknown");
        return;
    }
}

jobject mat2bmp(JNIEnv *env, cv::Mat &src, const jobject &bitmap, bool needPremultiplyAlpha) {
    AndroidBitmapInfo info;
    void *pixels = 0;

    try {
        CV_Assert(AndroidBitmap_getInfo(env, bitmap, &info) >= 0);
        CV_Assert(src.type() == CV_8UC1 || src.type() == CV_8UC3 || src.type() == CV_8UC4);
        CV_Assert(AndroidBitmap_lockPixels(env, bitmap, &pixels) >= 0);
        CV_Assert(pixels);
        if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
            LOGI("ANDROID_BITMAP_FORMAT_RGBA_8888");
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
            LOGI("else");
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
        return bitmap;
    } catch (cv::Exception e) {
        AndroidBitmap_unlockPixels(env, bitmap);
        jclass je = env->FindClass("org/opencv/core/CvException");
        if (!je) je = env->FindClass("java/lang/Exception");
        env->ThrowNew(je, e.what());
        return bitmap;
    } catch (...) {
        AndroidBitmap_unlockPixels(env, bitmap);
        jclass je = env->FindClass("java/lang/Exception");
        env->ThrowNew(je, "Unknown exception in JNI code {nMatToBitmap}");
        return bitmap;
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

