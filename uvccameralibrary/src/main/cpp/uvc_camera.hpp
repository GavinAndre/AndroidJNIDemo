#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif
#include <fcntl.h>
#include <sys/stat.h>
#include <malloc.h>
#ifdef __cplusplus
}
#endif


#include <stdio.h>
#include <unistd.h>
#include <jni.h>
#include <android/log.h>
#include <android/bitmap.h>
#include <list>
#include <mutex>
#include "libuvc/libuvc.h"
#include "jpegUtil.hpp"

#define  LOG_TAG    "LibUVCCamera"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define IMG_WIDTH 1280
#define IMG_HEIGHT 720

// 0 async/callback, 1 sync/polling
int stream_mode;

bool is_connect[2];
bool is_start_stream[2];
int fd[2];

uvc_context_t *ctx[2];
uvc_device_t *dev[2];
uvc_device_handle_t *devh[2];
uvc_stream_ctrl_t ctrl[2];
uvc_stream_handle_t *strmhp[2];

std::mutex _mutex[2];
std::list<uvc_frame_t *> data_list[2];

void uvc_preview_frame_callback(uvc_frame_t *frame, void *ptr) {
    int index = (int) ptr;
    _mutex[index].lock();
    if (data_list[index].size() >= 2) {
        uvc_frame_t *front = data_list[index].front();
        uvc_free_frame(front);
        data_list[index].pop_front();
    }
    uvc_frame_t *copy = uvc_allocate_frame(frame->data_bytes);
    uvc_duplicate_frame(frame, copy);
    data_list[index].push_back(copy);
    _mutex[index].unlock();
}

bool take_photo(int index, std::string path) {
    if (!is_start_stream[index]) {
        LOGE("take photo error, not start_stream, index %d", index);
        return false;
    }
    uvc_frame_t *copy;
    _mutex[index].lock();
    if (stream_mode) {
        uvc_frame_t *frame;
        uvc_frame_t *frame_test;
        uvc_error_t res = uvc_stream_get_frame(strmhp[index], &frame, 50 * 1000);
        uvc_error_t res_test = uvc_stream_get_frame(strmhp[index], &frame_test, 50 * 1000);
        if (res || res_test || !frame || !frame_test) {
            _mutex[index].unlock();
            uvc_perror(res, "take_photo uvc_stream_get_frame error");
            LOGE("take_photo uvc_stream_get_frame error, res %d, frame %p, frame_test %p, index %d",
                 res, frame, frame_test, index);
            return false;
        }
        copy = uvc_allocate_frame(frame->data_bytes);
        uvc_duplicate_frame(frame, copy);
    } else {
        if (data_list[index].empty()) {
            _mutex[index].unlock();
            LOGE("take_photo error, data_list empty, index %d", index);
            return false;
        }

        uvc_frame_t *frame = data_list[index].back();
        copy = uvc_allocate_frame(frame->data_bytes);
        uvc_duplicate_frame(frame, copy);
    }
    _mutex[index].unlock();

    unsigned char *buf = (unsigned char *) copy->data;

    store_MJPG_image(path.c_str(), buf, copy->data_bytes);

    uvc_free_frame(copy);

    return true;
}

uvc_error_t start_stream(int index) {
    uvc_error_t res = uvc_get_stream_ctrl_format_size_fps(
            devh[index], &ctrl[index], UVC_FRAME_FORMAT_MJPEG, IMG_WIDTH, IMG_HEIGHT, 1, 30);
    LOGI("start_stream, stream_mode %d, index %d", stream_mode, index);
    if (res < 0) {
        LOGE("start_stream uvc_get_stream_ctrl_format_size_fps error, res %d, index %d", res,
             index);
        return res;
    }
    if (stream_mode) {
        res = uvc_stream_open_ctrl(devh[index], &strmhp[index], &ctrl[index]);
        LOGI("start_stream uvc_stream_open_ctrl, res %d, index %d", res, index);

        res = uvc_stream_start_bandwidth(strmhp[index], NULL, NULL, 0.49, 0);
        LOGI("start_stream uvc_stream_start_bandwidth, res %d, index %d", res, index);

        uvc_frame_t *frame;
        res = uvc_stream_get_frame(strmhp[index], &frame, 50 * 1000);
        LOGI("start_stream uvc_stream_get_frame, res %d, frame %p, index %d", res, frame, index);
    } else {
        res = uvc_start_streaming_bandwidth(
                devh[index], &ctrl[index], uvc_preview_frame_callback, (int *) index, 0.49, 0);
        LOGI("start_stream uvc_start_streaming_bandwidth, res %d, index %d", res, index);
    }

    if (res == 0) {
        is_start_stream[index] = true;
    } else {
        LOGE("start_stream error, res %d, index %d", res, index);
    }

    return res;
}

int connect(int index, int vid, int pid, int fd, int busNum, int devAddr, std::string c_usbfs) {
    uvc_error_t res = uvc_init2(&ctx[index], NULL, c_usbfs.c_str());

    if (res < 0) {
        LOGE("connect uvc_init2 error, res %d, index %d", res, index);
        return JNI_ERR;
    }
    LOGI("connect UVC initialized, index %d", index);

    res = uvc_get_device_with_fd(ctx[index], &dev[index], vid, pid, NULL, fd, busNum, devAddr);

    if (res < 0) {
        LOGE("connect uvc_get_device_with_fd error, res %d , index %d", res, index);
        //        close(fd);
        return JNI_ERR;
    } else {
        LOGI("connect Device found, index %d", index);
        res = uvc_open(dev[index], &devh[index]);
        if (res < 0) {
            LOGE("connect uvc_open error, res %d, index %d", res, index);
            uvc_unref_device(dev[index]);
            dev[index] = nullptr;
            devh[index] = nullptr;
            //            close(fd);
            ::fd[index] = 0;
            return JNI_ERR;
        } else {
            LOGI("connect Device opened, index %d", index);
        }
    }

    if (res == 0) {
        ::fd[index] = fd;
        is_connect[index] = true;
    }

    res = start_stream(index);

    LOGI("connect, res %d, index %d", res, index);

    return res;
}

void release(int index) {
    LOGI("start release, index %d", index);
    if (is_start_stream[index]) {
        is_start_stream[index] = false;
        LOGI("start stop stream, stream_mode %d, index %d", stream_mode, index);
        if (stream_mode) {
            uvc_stream_stop(strmhp[index]);
            uvc_stream_close(strmhp[index]);
            strmhp[index] = nullptr;
        } else {
            uvc_stop_streaming(devh[index]);
        }
        LOGI("finish stop stream, stream_mode %d, index %d", stream_mode, index);
    }
    if (is_connect[index]) {
        is_connect[index] = false;
        LOGI("start release device, index %d", index);
        uvc_close(devh[index]);
        devh[index] = nullptr;
        LOGI("uvc_close, index %d", index);
        uvc_unref_device(dev[index]);
        dev[index] = nullptr;
        LOGI("uvc_unref_device, index %d", index);
        uvc_exit(ctx[index]);
        ctx[index] = nullptr;
        LOGI("uvc_exit, index %d", index);
        //        close(fd[index]);
        fd[index] = 0;
        LOGI("finish release device, index %d", index);
    }
    LOGI("start clear data_list, index %d", index);
    _mutex[index].lock();
    while (!data_list[index].empty()) {
        LOGI("data_list size %d, index %d", data_list[index].size(), index);
        uvc_frame_t *front = data_list[index].front();
        uvc_free_frame(front);
        data_list[index].pop_front();
    }
    data_list[index].clear();
    _mutex[index].unlock();
    LOGI("finish clear data_list, index %d", index);
    LOGI("finish release, index %d", index);
}

int set_param(int index, bool autoExpo, float expoTime, float gain, int brightness) {
    if (!is_start_stream[index]) {
        LOGE("set param error, not start_stream, index %d", index);
        return false;
    }
    uint8_t mode;
    uvc_error_t res = uvc_set_ae_mode(devh[index], autoExpo ? 8 : 1);
    LOGI("set_param uvc_set_ae_mode, res %d, ae mode expect %d, index %d",
         res, autoExpo ? 8 : 1, index);
    res = uvc_get_ae_mode(devh[index], &mode, UVC_GET_CUR);
    LOGI("set_param uvc_get_ae_mode, res %d, ae mode current %d, index %d", res, mode, index);
    int ae_abs;
    res = uvc_set_exposure_abs(devh[index], (int) (10000 * expoTime));
    LOGI("set_param uvc_set_exposure_abs, res %d, expo time expect %d, index %d", res,
         (int) (10000 * expoTime), index);
    res = uvc_get_exposure_abs(devh[index], &ae_abs, UVC_GET_CUR);
    LOGI("set_param uvc_get_exposure_abs, res %d, expo time current %d, index %d",
         res, ae_abs, index);
    uint16_t value;
    res = uvc_set_gain(devh[index], (int) gain);
    LOGI("set_param uvc_set_gain, res %d, gain expect %d, index %d", res, (int) gain, index);
    res = uvc_get_gain(devh[index], &value, UVC_GET_CUR);
    LOGI("set_param uvc_get_gain, res %d, gain current %d, index %d", res, value, index);
    int16_t _brightness;
    res = uvc_set_brightness(devh[index], (int) round((brightness - 50) * 1.28));
    LOGI("set_param uvc_set_brightness, res %d, brightness expect %d, index %d",
         res, (int) round((brightness - 50) * 1.28), index);
    res = uvc_get_brightness(devh[index], &_brightness, UVC_GET_CUR);
    LOGI("set_param uvc_get_brightness, res %d, brightness current %d, index %d",
         res, _brightness, index);
    return res;
}

int set_heat_coil(int index, bool on) {
    if (!is_start_stream[index]) {
        LOGE("set heat coil error, not start_stream, index %d", index);
        return false;
    }
    int16_t value;
    uvc_error_t res = uvc_set_backlight_compensation(devh[index], on ? 3 : 0);
    LOGI("set_heat_coil uvc_set_backlight_compensation, res %d, backlight compensation expect %d, index %d",
         res, on ? 3 : 0, index);
    res = uvc_get_backlight_compensation(devh[index], &value, UVC_GET_CUR);
    LOGI("set_heat_coil uvc_get_backlight_compensation, res %d, backlight compensation current %d, index %d",
         res, value, index);
    return res;
}

int get_heat_coil(int index) {
    if (!is_start_stream[index]) {
        LOGE("set heat coil error, not start_stream, index %d", index);
        return false;
    }
    int16_t value;
    uvc_error_t res = uvc_get_backlight_compensation(devh[index], &value, UVC_GET_CUR);
    LOGI("get_heat_coil uvc_get_backlight_compensation, res %d, backlight compensation current %d, index %d",
         res, value, index);
    return value;
}

void set_stream_mode(int mode) {
    stream_mode = mode;
}

int get_frame(uvc_frame_t *rgb565, int index) {
    if (stream_mode) {
        uvc_frame_t *frame;
        uvc_error_t res;
        _mutex[index].lock();
        res = uvc_stream_get_frame(strmhp[index], &frame, 50 * 1000);
        if (res || !frame) {
            _mutex[index].unlock();
            uvc_perror(res, "get_frame uvc_stream_get_frame error");
            LOGE("get_frame uvc_stream_get_frame error, res %d, frame %p, index %d", res, frame,
                 index);
            return -1;
        }
        uvc_frame_t *copy = uvc_allocate_frame(frame->data_bytes);
        uvc_duplicate_frame(frame, copy);
        _mutex[index].unlock();
        res = uvc_mjpeg2rgb565(copy, rgb565);
        if (res) {
            uvc_perror(res, "get_frame uvc_mjpeg2rgb565 error");
            LOGE("get_frame uvc_mjpeg2rgb565 error, res %d, index %d", res, index);
            uvc_free_frame(copy);
            return -1;
        }
        uvc_free_frame(copy);
    } else {
        _mutex[index].lock();
        if (data_list[index].empty()) {
            _mutex[index].unlock();
            return -1;
        }

        uvc_frame_t *front = data_list[index].front();
        data_list[index].pop_front();
        _mutex[index].unlock();

        uvc_error_t res;
        res = uvc_mjpeg2rgb565(front, rgb565);
        if (res) {
            uvc_perror(res, "get_frame uvc_mjpeg2rgb565 error");
            LOGE("get_frame uvc_mjpeg2rgb565 error, res %d, index %d", res, index);
            uvc_free_frame(front);
            return -1;
        }
        uvc_free_frame(front);
    }
    return 0;
}

void pixel_to_bmp(JNIEnv *env, jclass thiz, int index, jobject bitmap) {
    int ret;
    uvc_frame_t *rgb565 = uvc_allocate_frame(IMG_WIDTH * IMG_HEIGHT * 2);
    ret = get_frame(rgb565, index);
    if (ret) {
        LOGE("pixel_to_bmp get_frame error %d", ret);
        uvc_free_frame(rgb565);
        return;
    }

    AndroidBitmapInfo info;
    void *pixels;
    int i;
    int *colors;

    if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0) {
        LOGE("pixel_to_bmp AndroidBitmap_getInfo() failed ! error=%d", ret);
        uvc_free_frame(rgb565);
        return;
    }

    int width = info.width;
    int height = info.height;

    if (info.format != ANDROID_BITMAP_FORMAT_RGB_565) {
        LOGE("pixel_to_bmp Bitmap format is not RGB_565!");
        return;
    }

    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0) {
        LOGE("pixel_to_bmp AndroidBitmap_lockPixels() failed ! error=%d", ret);
        uvc_free_frame(rgb565);
        return;
    }

    colors = (int *) pixels;

    memcpy(colors, rgb565->data, rgb565->data_bytes);
    uvc_free_frame(rgb565);

    AndroidBitmap_unlockPixels(env, bitmap);
}
