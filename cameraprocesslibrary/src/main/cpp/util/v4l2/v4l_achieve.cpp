/*
 * Vedio.cpp
 *
 *  Created on: 2014-6-1
 *      Author: sfe1012
 */
#include "v4l_achieve.h"

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

V4LAchieve::V4LAchieve() : miCameraIndex(-1), miOpenedCameraFd(-1),
                           miBufferCount(-1), mpstV4LBuffers(NULL) {}

V4LAchieve::V4LAchieve(const int &iCameraIndex, const int &iWidth, const int &iHeigth)
        : miCameraIndex(-1), miOpenedCameraFd(-1),
          miBufferCount(-1), mpstV4LBuffers(NULL) {
    this->miCameraIndex = iCameraIndex;
    m_pYUYV422 = new unsigned char[iWidth * iHeigth * 2];
    m_iYUYV422Len = 0;
    m_iWidth = iWidth;
    m_iHeight = iHeigth;
}

V4LAchieve::~V4LAchieve() {
    delete[] m_pYUYV422;
}

void V4LAchieve::Init(const int &iCameraIndex, const int &iWidth, const int &iHeigth) {
    this->miCameraIndex = iCameraIndex;
    m_pYUYV422 = new unsigned char[iWidth * iHeigth * 2];
    m_iYUYV422Len = 0;
    m_iWidth = iWidth;
    m_iHeight = iHeigth;
}

bool V4LAchieve::OpenCamera() {
    const int iMAXPATH = 50;
    char *chPCameraDevicename = new char[iMAXPATH];
    std::memset(chPCameraDevicename, 0, iMAXPATH);
    std::sprintf(chPCameraDevicename, "/dev/video%d", miCameraIndex);
#ifdef V4LDEBUG
    V4L_LOGI("Open Camera Device : %s", chPCameraDevicename);
#endif
    miOpenedCameraFd = open(chPCameraDevicename, O_RDWR /* required */ | O_NONBLOCK, 0);//打开设备
    if (miOpenedCameraFd < 0) {
#ifdef V4LDEBUG
        V4L_LOGI(" Open Camera Device : %s Failed", chPCameraDevicename);
#endif
        return false;
    }
    delete[] chPCameraDevicename;
    chPCameraDevicename = NULL;

    if (!GetCameraParameters()) {
#ifdef V4LDEBUG
        V4L_LOGE("GetCameraParameters Fail");
#endif
        return false;
    }
    if (!SetCameraVideoFormat()) {
#ifdef V4LDEBUG
        V4L_LOGE("SetCameraVideoFormat Fail");
#endif
        return false;
    }
    if (!StartCameraCapture()) {
#ifdef V4LDEBUG
        V4L_LOGI("StartCameraCapture Fail");
#endif
        return false;
    }
    return true;
}

//Get camera parameters
bool V4LAchieve::GetCameraParameters() {
    if (miOpenedCameraFd < 0) {
#ifdef V4LDEBUG
        V4L_LOGE("Invalid Camera File Descriptor");
#endif
        return false;
    }
    struct v4l2_capability stV4l2Capability;
    std::memset(&stV4l2Capability, 0, sizeof(struct v4l2_capability));
    if (ioctl(miOpenedCameraFd, VIDIOC_QUERYCAP, &stV4l2Capability) < 0) {
#ifdef V4LDEBUG
        V4L_LOGE("Get Camera Parameters Failed!");
#endif
        return false;
    }
#ifdef V4LDEBUG
    V4L_LOGI("Camera Capability as :");
    V4L_LOGI("Camera Bus info: %s", stV4l2Capability.bus_info);
    V4L_LOGI("Camera Name: %s", stV4l2Capability.card);
    V4L_LOGI("Camera Kernel Version: %d", stV4l2Capability.version);
    V4L_LOGI("Camera Driver Info: %s", stV4l2Capability.driver);
#endif
    return true;
}

// set camera capture property
bool V4LAchieve::SetCameraVideoFormat() {
    if (miOpenedCameraFd < 0) {
#ifdef V4LDEBUG
        V4L_LOGE("Invalid Camera File Descriptor");
#endif
        return false;
    }
    struct v4l2_format stV4l2Format;
    std::memset(&stV4l2Format, 0, sizeof(struct v4l2_format));
    stV4l2Format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    stV4l2Format.fmt.pix.width = m_iWidth;
    stV4l2Format.fmt.pix.height = m_iHeight;
    stV4l2Format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
//    stV4l2Format.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
    stV4l2Format.fmt.pix.field = V4L2_FIELD_INTERLACED;
    if (ioctl(miOpenedCameraFd, VIDIOC_S_FMT, &stV4l2Format) < 0) {
#ifdef V4LDEBUG
        V4L_LOGE("set camera Capture format error! ");
#endif
        return false;
    }
#ifdef V4LDEBUG
    V4L_LOGI("set camera capture format is ok !");
#endif

    struct v4l2_requestbuffers stV4l2RequestBuffers;
    std::memset(&stV4l2RequestBuffers, 0, sizeof(struct v4l2_requestbuffers));
    stV4l2RequestBuffers.count = 4;
    stV4l2RequestBuffers.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    stV4l2RequestBuffers.memory = V4L2_MEMORY_MMAP;

    if (ioctl(miOpenedCameraFd, VIDIOC_REQBUFS, &stV4l2RequestBuffers) < 0) //申请缓冲，count是申请的数量
    {
#ifdef V4LDEBUG
        V4L_LOGE("申请缓存失败! ");
#endif
        return false;
    }
    if (stV4l2RequestBuffers.count < 2) {
#ifdef V4LDEBUG
        V4L_LOGE("Insufficient buffer memory");
#endif
    }
#ifdef V4LDEBUG
    V4L_LOGI("The Camera Apply Cache Success");;
    V4L_LOGI("Cache Num = %d", stV4l2RequestBuffers.count);;
    V4L_LOGI("Cache  Size = %d", stV4l2RequestBuffers.memory);
    V4L_LOGI("Cache  Type = %d", stV4l2RequestBuffers.type);
#endif
    miBufferCount = stV4l2RequestBuffers.count; //保存缓存的帧数
    //开始分配缓存
    mpstV4LBuffers = (struct st_V4LBuffer *) calloc(stV4l2RequestBuffers.count,
                                                    sizeof(struct st_V4LBuffer));//内存中建立对应空间
    unsigned int iReqBuffersNum = 0;
    for (iReqBuffersNum = 0; iReqBuffersNum < stV4l2RequestBuffers.count; ++iReqBuffersNum) {
        struct v4l2_buffer stV4l2Buffer;   //驱动中的一帧
        std::memset(&stV4l2Buffer, 0, sizeof(struct v4l2_buffer));

        stV4l2Buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        stV4l2Buffer.memory = V4L2_MEMORY_MMAP;
        stV4l2Buffer.index = iReqBuffersNum;
        if (-1 == ioctl(miOpenedCameraFd, VIDIOC_QUERYBUF, &stV4l2Buffer)) //映射用户空间
        {
#ifdef V4LDEBUG
            V4L_LOGE("VIDIOC_QUERYBUF error");
#endif
            break;
        }
        mpstV4LBuffers[iReqBuffersNum].iLength = stV4l2Buffer.length;
        mpstV4LBuffers[iReqBuffersNum].pStart =
                mmap(NULL /* start anywhere */,    //通过mmap建立映射关系
                     stV4l2Buffer.length,
                     PROT_READ | PROT_WRITE /* required */,
                     MAP_SHARED /* recommended */,
                     miOpenedCameraFd, stV4l2Buffer.m.offset);
        if (MAP_FAILED == mpstV4LBuffers[iReqBuffersNum].pStart) {
#ifdef V4LDEBUG
            V4L_LOGE("mmap failed\n");
#endif
            break;
        }
        ::memset(mpstV4LBuffers[iReqBuffersNum].pStart, 0, mpstV4LBuffers[iReqBuffersNum].iLength);
    }
    if (iReqBuffersNum < stV4l2RequestBuffers.count) {
        V4L_LOGE(" error in request v4l2 buffer ");
        return false;
    }
#ifdef V4LDEBUG
    V4L_LOGI(" request v4l2 buffer finsihed");
#endif
    unsigned int index = 0;
    for (index = 0; index < iReqBuffersNum; ++index) {
        struct v4l2_buffer buf;
        std::memset(&buf, 0, sizeof(struct v4l2_buffer));

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = index;

        if (-1 == ioctl(miOpenedCameraFd, VIDIOC_QBUF, &buf))//申请到的缓冲进入列队
        {
#ifdef V4LDEBUG
            V4L_LOGE("VIDIOC_QBUF failed with index= %d", index);
#endif
            break;
        }
    }
    if (index < iReqBuffersNum) {
#ifdef V4LDEBUG
        V4L_LOGE(" error in  v4l2 buffer queue ");
#endif
        return false;
    }
    return true;
}

bool V4LAchieve::StartCameraCapture() {
    if (miOpenedCameraFd < 0) return false;
    enum v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (-1 == ioctl(miOpenedCameraFd, VIDIOC_STREAMON, &type)) //开始捕捉图像数据
    {
#ifdef V4LDEBUG
        V4L_LOGI(" Start VIDIOC_STREAMON failed \n");
#endif
        return false;
    }
    V4L_LOGI("VIDIOC_STREAMON Start collecting capture graph ok");
    return true;
}

bool V4LAchieve::CameraVideoGetLoop()  //双摄像头将不会加入 IO 监控处理
{
    fd_set fds;
    struct timeval tv;
    FD_ZERO (&fds);//将指定的文件描述符集清空
    FD_SET (miOpenedCameraFd, &fds);//在文件描述符集合中增加一个新的文件描述符
    /* Timeout. */
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    int r = ::select(miOpenedCameraFd + 1, &fds, NULL, NULL, &tv);//判断是否可读（即摄像头是否准备好），tv是定时
    if (-1 == r) {
        if (EINTR == errno)
            return 0;
#ifdef V4LDEBUG
        V4L_LOGE("select err");
#endif
        return false;
    }
    // start to dequeue image systemCameraFrame
    struct v4l2_buffer buf;
    std::memset(&buf, 0, sizeof(struct v4l2_buffer));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    if (ioctl(miOpenedCameraFd, VIDIOC_DQBUF, &buf) < 0) //出列采集的帧缓中
    {
#ifdef V4LDEBUG
        V4L_LOGE("获取数据失败! ");
#endif
        return false;
    }
    assert (buf.index < (unsigned long) miBufferCount);

    m_iYUYV422Len = mpstV4LBuffers[buf.index].iLength;
    ::memcpy(m_pYUYV422, mpstV4LBuffers[buf.index].pStart, mpstV4LBuffers[buf.index].iLength);
    ioctl(miOpenedCameraFd, VIDIOC_QBUF, &buf); //再将其入列
    return true;
}

bool V4LAchieve::CloseCamera() {
    for (int index = 0; index < (mpstV4LBuffers ? miBufferCount : 0); index++)
        ::munmap(mpstV4LBuffers[index].pStart, mpstV4LBuffers[index].iLength);
    if (mpstV4LBuffers) {
        free(mpstV4LBuffers);
        mpstV4LBuffers = NULL;
    }
    if (miOpenedCameraFd != -1) {
        ::close(miOpenedCameraFd);
        miOpenedCameraFd = -1;
    }
    return true;
}

void V4LAchieve::DeInit() {
    delete[] m_pYUYV422;
}