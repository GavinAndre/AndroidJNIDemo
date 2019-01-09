package com.gavinandre.cameraprocesslibrary.usbcamera

import android.content.Context
import android.graphics.*
import android.os.SystemClock
import android.util.AttributeSet
import android.util.Log
import android.view.TextureView
import android.widget.Toast
import com.gavinandre.cameraprocesslibrary.CameraProcessLib

class UsbCameraTextureView @JvmOverloads constructor(context: Context, attrs: AttributeSet? = null,
                                                     defStyleAttr: Int = 0) :
        TextureView(context, attrs, defStyleAttr), TextureView.SurfaceTextureListener {
    
    private val TAG = UsbCameraTextureView::class.java.simpleName
    
    companion object {
        var drawFrame: Int = 0
        private const val PREVIEW_WIDTH = 640
        private const val PREVIEW_HEIGHT = 480
    }
    
    private var stop: Boolean = false
    
    private val mBitmap: Bitmap by lazy {
        Bitmap.createBitmap(PREVIEW_WIDTH, PREVIEW_HEIGHT,
                Bitmap.Config.ARGB_8888)
    }
    
    private val mSrcRect: Rect by lazy { Rect(0, 0, mBitmap.width, mBitmap.height) }
    private val mDstRect: Rect by lazy { Rect(0, 0, width, height) }
    
    init {
        init()
    }
    
    private fun init() {
        //设置背景透明，记住这里是[是否不透明]
        isOpaque = false
        //设置监听
        surfaceTextureListener = this
        Log.i(TAG, "init: ")
    }
    
    override fun onSurfaceTextureAvailable(surface: SurfaceTexture, width: Int, height: Int) {
        val ret = CameraProcessLib.prepareUsbCamera(PREVIEW_WIDTH, PREVIEW_HEIGHT)
        Log.i(TAG, "onSurfaceTextureAvailable: ret $ret")
        if (ret == -1) {
            Toast.makeText(context, "初始化失败", Toast.LENGTH_SHORT).show()
            return
        }
        //开启线程
        val mPlayThread = PlayThread()
        mPlayThread.start()
    }
    
    override fun onSurfaceTextureSizeChanged(surface: SurfaceTexture, width: Int, height: Int) {
        Log.i(TAG, "onSurfaceTextureSizeChanged: ")
    }
    
    override fun onSurfaceTextureDestroyed(surface: SurfaceTexture): Boolean {
        Log.i(TAG, "onSurfaceTextureDestroyed: ")
        stop = true
        while (stop) {
            SystemClock.sleep(50)
        }
        CameraProcessLib.releaseUsbCamera()
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
    
    private inner class PlayThread : Thread() {
        override fun run() {
            while (true) {
                if (stop) {
                    stop = false
                    break
                }
                // SystemClock.sleep(2);
                CameraProcessLib.processUsbCamera()
                CameraProcessLib.pixelToBmp(mBitmap)
                drawBitmap(mBitmap)
            }
        }
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
