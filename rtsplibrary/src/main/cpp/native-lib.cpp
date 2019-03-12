#include <jni.h>
#include <string>
#include <android/log.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

#define  LOG_TAG    "Rstp"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

const char *TAG = __FILE__;

jobject gCallback;
jmethodID gCallbackMethodId;
volatile bool isStop = false;

void callback(JNIEnv *env, uint8_t *buf, int channel, int width, int height);

extern "C"
jint Java_com_gavinandre_rtsplibrary_RtspClient_initialize(JNIEnv *env, jobject, jobject callback) {
    isStop = false;
    gCallback = env->NewGlobalRef(callback);
    jclass clz = env->GetObjectClass(gCallback);
    if (clz == NULL) {
        return JNI_ERR;
    } else {
        gCallbackMethodId = env->GetMethodID(clz, "onFrame", "([BIII)V");
        return JNI_OK;
    }
}

extern "C"
jint Java_com_gavinandre_rtsplibrary_RtspClient_play(
        JNIEnv *env, jobject, jstring endpoint) {
    try {
        SwsContext *img_convert_ctx;
        //分配一个AVFormatContext，FFMPEG所有的操作都要通过这个AVFormatContext来进行
        AVFormatContext *context = avformat_alloc_context();
        AVCodecContext *ccontext = avcodec_alloc_context3(NULL);

        //初始化FFMPEG  调用了这个才能正常适用编码器和解码器
        av_register_all();
        //初始化网络模块
        avformat_network_init();

        AVDictionary *option = NULL;
//        av_dict_set(&option, "buffer_size", "1024000", 0);
//        av_dict_set(&option, "max_delay", "500000", 0);
//        av_dict_set(&option, "stimeout", "20000000", 0);  //设置超时断开连接时间
        av_dict_set(&option, "rtsp_transport", "tcp", 0); //以udp方式打开，如果以tcp方式打开将udp替换为tcp

        const char *rtspUrl = env->GetStringUTFChars(endpoint, JNI_FALSE);
        //打开网络流或文件
        if (int err = avformat_open_input(&context, rtspUrl, NULL, &option) != 0) {
            char errors[1024];
            av_strerror(err, errors, 1024);
            __android_log_print(ANDROID_LOG_ERROR, TAG, "Cannot open input %s, error code: %s",
                                rtspUrl,
                                errors);
            return JNI_ERR;
        }
        env->ReleaseStringUTFChars(endpoint, rtspUrl);

        av_dict_free(&option);

        //获取视频流信息
        if (avformat_find_stream_info(context, NULL) < 0) {
            __android_log_print(ANDROID_LOG_ERROR, TAG, "Cannot find stream info");
            return JNI_ERR;
        }

        //循环查找视频中包含的流信息，直到找到视频类型的流
        int video_stream_index = -1;
        //nb_streams代表有几路流，一般是2路：即音频和视频，顺序不一定
        for (int i = 0; i < context->nb_streams; i++) {
            if (context->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
                //这一路是视频流，标记一下，以后取视频流都从ifmt_ctx->streams[video_stream_index]取
                video_stream_index = i;
        }

        //如果video_stream_index为-1 说明没有找到视频流
        if (video_stream_index == -1) {
            __android_log_print(ANDROID_LOG_ERROR, TAG, "Video stream not found");
            return JNI_ERR;
        }

        // Open output file
        AVFormatContext *oc = avformat_alloc_context();
        AVStream *stream = NULL;

        // Start reading packets from stream and write them to file
        av_read_play(context);

        AVCodec *codec = NULL;
        //使用h264作为解码器
        codec = avcodec_find_decoder(AV_CODEC_ID_H264);
        if (!codec) {
            __android_log_print(ANDROID_LOG_ERROR, TAG, "Cannot find decoder H264");
            return JNI_ERR;
        }

        avcodec_get_context_defaults3(ccontext, codec);
        avcodec_copy_context(ccontext, context->streams[video_stream_index]->codec);

        //打开解码器
        if (avcodec_open2(ccontext, codec, NULL) < 0) {
            __android_log_print(ANDROID_LOG_ERROR, TAG, "Cannot open codec");
            return JNI_ERR;
        }

        //设置数据转换参数
        img_convert_ctx = sws_getContext(
                //源视频图像长宽以及数据格式
                ccontext->width, ccontext->height, ccontext->pix_fmt,
                //目标视频图像长宽以及数据格式
                ccontext->width, ccontext->height, AV_PIX_FMT_RGB24,
                //算法类型 AV_PIX_FMT_YUV420P AV_PIX_FMT_RGB24
                SWS_BICUBIC, NULL, NULL, NULL);

        //分配空间,一帧图像数据大小
        size_t size = (size_t) avpicture_get_size(AV_PIX_FMT_YUV420P, ccontext->width,
                                                  ccontext->height);
        uint8_t *picture_buf = (uint8_t *) (av_malloc(size));
        AVFrame *pic = av_frame_alloc();
        AVFrame *picrgb = av_frame_alloc();
        size_t size2 = (size_t) avpicture_get_size(AV_PIX_FMT_RGB24, ccontext->width,
                                                   ccontext->height);
        uint8_t *picture_buf2 = (uint8_t *) (av_malloc(size2));
        avpicture_fill((AVPicture *) pic, picture_buf, AV_PIX_FMT_YUV420P, ccontext->width,
                       ccontext->height);
        //将picrgb的数据按RGB格式自动"关联"到buffer
        //即picrgb中的数据改变了 picture_buf2中的数据也会相应的改变
        avpicture_fill((AVPicture *) picrgb, picture_buf2, AV_PIX_FMT_RGB24, ccontext->width,
                       ccontext->height);

        //分配AVPacket结构体
        AVPacket packet;
        //分配packet的数据
        av_init_packet(&packet);

        //从输入流中读取一帧视频,数据存入AVPacket的结构中
        while (!isStop && av_read_frame(context, &packet) >= 0) {
            if (packet.stream_index == video_stream_index) { // Packet is video
                if (stream == NULL) {
                    stream = avformat_new_stream(oc,
                                                 context->streams[video_stream_index]->codec->codec);
                    avcodec_copy_context(stream->codec,
                                         context->streams[video_stream_index]->codec);
                    stream->sample_aspect_ratio = context->streams[video_stream_index]->codec->sample_aspect_ratio;
                }

                int check = 0;
                packet.stream_index = stream->id;
                //视频解码函数,解码之后的数据存储在pic中
                avcodec_decode_video2(ccontext, pic, &check, &packet);
                //按长宽缩放图像,并转换格式yuv->rgb
                sws_scale(img_convert_ctx, (const uint8_t *const *) pic->data, pic->linesize, 0,
                          ccontext->height, picrgb->data, picrgb->linesize);
                LOGI("gCallback %p ", &gCallback);
                LOGI("gCallbackMethodId %p ", &gCallbackMethodId);
                if (gCallback != nullptr) {
                    //回调到java层
                    callback(env, picture_buf2, 3, ccontext->width, ccontext->height);
                }
            }
            av_free_packet(&packet);
            av_init_packet(&packet);
        }

        av_free(pic);
        av_free(picrgb);
        av_free(picture_buf);
        av_free(picture_buf2);

        av_read_pause(context);
        avio_close(oc->pb);
        avformat_free_context(oc);
        avformat_close_input(&context);

    } catch (std::exception &e) {
        jclass je = env->FindClass("java/lang/Exception");
        env->ThrowNew(je, e.what());
    }

    if (isStop) {
        LOGI("isStop");
    }

    return isStop ? JNI_OK : JNI_ERR;
}

extern "C"
void Java_com_gavinandre_rtsplibrary_RtspClient_stop(JNIEnv *env, jobject) {
    isStop = true;
}

extern "C"
void Java_com_gavinandre_rtsplibrary_RtspClient_dispose(JNIEnv *env, jobject) {
    try {
        LOGI("DeleteGlobalRef1");
        env->DeleteGlobalRef(gCallback);
        gCallback = nullptr;
        LOGI("DeleteGlobalRef2");
    } catch (std::exception &e) {
        jclass je = env->FindClass("java/lang/Exception");
        env->ThrowNew(je, e.what());
    }
}

void callback(JNIEnv *env, uint8_t *buf, int nChannel, int width, int height) {
    try {
        LOGI("callback1");
        int len = nChannel * width * height;
        jbyteArray gByteArray = env->NewByteArray(len);
        env->SetByteArrayRegion(gByteArray, 0, len, (jbyte *) buf);
        LOGI("callback2");
        env->CallVoidMethod(gCallback, gCallbackMethodId, gByteArray, nChannel, width, height);
        LOGI("callback3");
        env->DeleteLocalRef(gByteArray);
        LOGI("callback4");
    } catch (std::exception &e) {
        jclass je = env->FindClass("java/lang/Exception");
        env->ThrowNew(je, e.what());
    }
}

