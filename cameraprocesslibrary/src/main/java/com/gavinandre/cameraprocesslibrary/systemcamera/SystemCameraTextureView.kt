package com.gavinandre.cameraprocesslibrary.systemcamera

import android.content.Context
import android.graphics.*
import android.util.AttributeSet
import android.util.Log
import android.view.TextureView
import android.widget.Toast
import com.gavinandre.cameraprocesslibrary.CameraProcessLib
import com.gavinandre.cameraprocesslibrary.utils.Camera1Manager
import kotlinx.coroutines.*

class SystemCameraTextureView : TextureView, TextureView.SurfaceTextureListener {

    private val TAG = SystemCameraTextureView::class.java.simpleName

    companion object {
        var drawFrame: Int = 0
    }

    private var job: Job? = null

    private val mBitmap: Bitmap by lazy {
        Bitmap.createBitmap(
            Camera1Manager.PREVIEW_WIDTH, Camera1Manager.PREVIEW_HEIGHT,
            Bitmap.Config.ARGB_8888
        )
    }

    private val mSrcRect: Rect by lazy { Rect(0, 0, mBitmap.width, mBitmap.height) }
    private val mDstRect: Rect by lazy { Rect(0, 0, width, height) }

    constructor(context: Context) : super(context)
    constructor(context: Context, attributeSet: AttributeSet? = null) : super(context, attributeSet)
    constructor(context: Context, attributeSet: AttributeSet? = null, defStyleAttr: Int = 0)
            : super(context, attributeSet, defStyleAttr)

    init {
        //设置背景透明，记住这里是[是否不透明]
        isOpaque = false
        //设置监听
        surfaceTextureListener = this
        Log.i(TAG, "init: ")
    }

    override fun onSurfaceTextureAvailable(surface: SurfaceTexture, width: Int, height: Int) {
        //打开相机
        Camera1Manager.openCamera(1)
        var ret = Camera1Manager.setupCamera()
        if (ret == -1) {
            Toast.makeText(context, "摄像头初始化失败", Toast.LENGTH_SHORT).show()
            return
        }
        ret = CameraProcessLib.prepareSystemCamera(Camera1Manager.RAW_PREVIEW_WIDTH, Camera1Manager.RAW_PREVIEW_HEIGHT)
        Log.i(TAG, "onSurfaceTextureAvailable: ret $ret")
        if (ret == -1) {
            Toast.makeText(context, "算法初始化失败", Toast.LENGTH_SHORT).show()
            return
        }
        //开启协程
        job = processSystemCameraJob()
    }

    override fun onSurfaceTextureSizeChanged(surface: SurfaceTexture, width: Int, height: Int) {
        Log.i(TAG, "onSurfaceTextureSizeChanged: ")
    }

    override fun onSurfaceTextureDestroyed(surface: SurfaceTexture): Boolean {
        Log.i(TAG, "onSurfaceTextureDestroyed: ")
        stopSystemCameraJob()
        Camera1Manager.releaseCamera()
        CameraProcessLib.releaseSystemCamera()
        releaseBitmap()
        return true
    }

    override fun onSurfaceTextureUpdated(surface: SurfaceTexture) {
        // Log.i(TAG, "onSurfaceTextureUpdated: ");
    }

    private fun releaseBitmap() {
        if (!mBitmap.isRecycled) {
            mBitmap.recycle()
        }
    }

    private fun processSystemCameraJob(): Job = GlobalScope.launch {
        // 启动一个新协程并保持对这个作业的引用
        while (isActive) {
            val frame = Camera1Manager.preBuffer
            CameraProcessLib.processSystemCamera(frame)
            CameraProcessLib.pixelToBmp(mBitmap)
            drawBitmap(mBitmap)
            delay(5L)
        }
    }

    private fun stopSystemCameraJob() = runBlocking {
        job?.cancelAndJoin()
    }

    private fun drawBitmap(bitmap: Bitmap) {
        // Log.i(TAG, "drawBitmap: ");
        //锁定画布
        val canvas = lockCanvas()
        if (canvas != null) {
            //清空画布
            canvas.drawColor(Color.TRANSPARENT, PorterDuff.Mode.CLEAR)
            //将bitmap画到画布上
            canvas.drawBitmap(bitmap, mSrcRect, mDstRect, null)
            //解锁画布同时提交
            unlockCanvasAndPost(canvas)
        }
        drawFrame++
    }

}
