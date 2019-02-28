/*
 * V4LAchieve.h
 *
 *  Created on: 2014-6-1
 *      Author: sfe1012
 */
#ifndef VEDIO_H_
#define VEDIO_H_
#define V4LDEBUG 0;

#include <time.h>
#include <sys/ioctl.h>
#include <asm/types.h>
#include <linux/videodev2.h>
#include <sys/mman.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <android/log.h>

#define V4L_LOG_TAG    "WebCamera"
#define V4L_LOGI(...)  __android_log_print(ANDROID_LOG_INFO,V4L_LOG_TAG,__VA_ARGS__)
#define V4L_LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,V4L_LOG_TAG,__VA_ARGS__)

class V4LAchieve {
public:

    V4LAchieve();

    V4LAchieve(const int &iCameraIndex, const int &iWidth, const int &iHeigth);

    virtual ~V4LAchieve();

    void Init(const int &iCameraIndex, const int &iWidth, const int &iHeigth);

    void DeInit();

    bool OpenCamera();

    bool CloseCamera();

    bool CameraVideoGetLoop();

    unsigned char *GetpYUYV422() const { return m_pYUYV422; }

    int GetpYUYV422Len() const { return m_iYUYV422Len; }

    int GetOpenedCameraFd() const { return miOpenedCameraFd; }

private:
    bool GetCameraParameters();

    bool SetCameraVideoFormat();

    bool StartCameraCapture();

    bool StopCameraCapture();

private:
    int miCameraIndex;
    int miOpenedCameraFd;    //default open camera ;
    int miBufferCount; //保存缓存的帧数
    typedef struct st_V4LBuffer {
        void *pStart;
        int iLength;
    } stV4LBuffer;
    stV4LBuffer *mpstV4LBuffers;
    unsigned char *m_pYUYV422;
    int m_iYUYV422Len;
    int m_iWidth;
    int m_iHeight;
};

#endif /* VEDIO_H_ */
