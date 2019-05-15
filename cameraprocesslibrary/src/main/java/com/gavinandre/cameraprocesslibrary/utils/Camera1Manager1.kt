package com.gavinandre.cameraprocesslibrary.utils

import android.graphics.ImageFormat
import android.graphics.SurfaceTexture
import android.hardware.Camera
import android.util.Log

object Camera1Manager1 {

    private val TAG = Camera1Manager1::class.java.simpleName

    private const val MAGIC_TEXTURE_ID = 10

    const val PREVIEW_WIDTH = 480
    const val PREVIEW_HEIGHT = 640
    const val RAW_PREVIEW_WIDTH = 640
    const val RAW_PREVIEW_HEIGHT = 480

    private var isProperResolution = false
    // 定义系统所用的照相机
    private var camera: Camera? = null
    // 是否在预览中
    private var isPreview = false

    var preBuffer: ByteArray? = null
        private set

    private var emptySurfaceTexture: SurfaceTexture? = null

    var systemCameraFrame: Int = 0

    /**
     * 获取Camera实例
     *
     * @param id
     * @return
     */
    fun openCamera(id: Int) {
        if (camera == null) {
            camera = Camera.open(id)
        }
    }

    /**
     * 摄像机参数设置
     */
    fun setupCamera(): Int {
        Log.i(TAG, "setupCamera: isPreview $isPreview")

        if (isPreview) {
            return -1
        }

        camera?.let { camera ->
            emptySurfaceTexture = SurfaceTexture(MAGIC_TEXTURE_ID)
            camera.setPreviewTexture(emptySurfaceTexture)

            Log.i(TAG, "camera.open OK")
            val parameters = camera.parameters
            val supportedPreviewSizes = parameters.supportedPreviewSizes
            val supportedPictureSizes = parameters.supportedPictureSizes
            val size = supportedPreviewSizes.size
            for (i in 0 until size) {
                Log.i(
                    TAG, "setupCamera : supportedPreviewSizes $i =" +
                            " ${supportedPreviewSizes[i].width} x ${supportedPreviewSizes[i].height}"
                )
                if (supportedPreviewSizes[i].width == RAW_PREVIEW_WIDTH && supportedPreviewSizes[i].height == RAW_PREVIEW_HEIGHT) {
                    isProperResolution = true
                }
            }

            if (!isProperResolution) {
                Log.e(TAG, "no proper resolution")
                return -1
            }

            Log.i(TAG, "setupCamera: IMG_WIDTH$RAW_PREVIEW_WIDTH")
            Log.i(TAG, "setupCamera: IMG_HEIGHT$RAW_PREVIEW_HEIGHT")

            parameters.setPreviewSize(RAW_PREVIEW_WIDTH, RAW_PREVIEW_HEIGHT)
            parameters.setPictureSize(RAW_PREVIEW_WIDTH, RAW_PREVIEW_HEIGHT)
            parameters.setRecordingHint(true)
            camera.parameters = parameters

            //跟setPreviewCallback的工作方式一样，但是要求指定一个字节数组作为缓冲区，用于预览帧数据，
            //这样能够更好的管理预览帧数据时使用的内存。它一般搭配addCallbackBuffer方法使用
            //需要在startPreview()之前调用
            //camera.setPreviewCallback(new StreamIt(""));
            camera.setPreviewCallbackWithBuffer(StreamIt())
            val byteSize = parameters.previewSize.width * parameters.previewSize.height *
                    ImageFormat.getBitsPerPixel(ImageFormat.NV21) / 8
            camera.addCallbackBuffer(ByteArray(byteSize))
            // 设置回调的类

            camera.startPreview() // 开始预览
            // camera.autoFocus(null); // 自动对焦
            Log.i(TAG, "startPreview OK")

            isPreview = true

            return 0
        } ?: let {
            Log.e(TAG, "camera.open fail")
            return -1
        }
    }

    /**
     * 释放相机资源
     */
    fun releaseCamera() {
        if (isPreview) {
            camera?.apply {
                setPreviewCallbackWithBuffer(null)
                setPreviewTexture(null)
                //setPreviewCallback(null);
                stopPreview()
                release()
            }
            preBuffer = null
            camera = null
        }
        isPreview = false
        emptySurfaceTexture?.release()
        Log.i(TAG, "releaseCamera: isPreview $isPreview")
    }

    /**
     * 原始帧回调
     */
    internal class StreamIt : Camera.PreviewCallback {

        override fun onPreviewFrame(data: ByteArray, camera: Camera) {
            preBuffer = data
            systemCameraFrame++
            camera.addCallbackBuffer(data)
        }

    }

}
