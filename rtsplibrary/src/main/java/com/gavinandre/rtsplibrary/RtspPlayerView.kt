package com.gavinandre.rtsplibrary

import android.content.Context
import android.graphics.*
import android.os.SystemClock
import android.util.AttributeSet
import android.util.Log
import android.view.TextureView
import kotlinx.coroutines.*

class RtspPlayerView : TextureView, TextureView.SurfaceTextureListener, NativeCallback {

    private val TAG = RtspPlayerView::class.java.simpleName

    private val endpoint = "rtsp://192.168.3.5:554/user=admin&password=&channel=1&stream=1.sdp?real_stream"
    // private String endpoint = "rtsp://192.168.1.123:8554/test";

    private lateinit var rtspClient: RtspClient
    private var mDstRect: Rect
    private var bmp: Bitmap? = null
    private var isStop: Boolean = false
    private lateinit var job: Job

    constructor(context: Context?) : super(context)
    constructor(context: Context?, attrs: AttributeSet?) : super(context, attrs)
    constructor(context: Context?, attrs: AttributeSet?, defStyleAttr: Int) : super(context, attrs, defStyleAttr)

    init {
        //设置背景透明，记住这里是[是否不透明]
        isOpaque = false

        //设置监听
        surfaceTextureListener = this

        mDstRect = Rect()
        Log.i(TAG, "init: ")
    }

    private fun initRtsp(): Job {
        // 启动一个新协程并保持对这个作业的引用
        return GlobalScope.launch {
            initRtspClient()
            while (isActive) {
                if (rtspClient.play(endpoint) == 0) {
                    break
                }
                delay(5000L)
            }
            Log.i(TAG, "initRtsp: coroutine finish")
        }
    }

    private fun initRtspClient() {
        Log.i(TAG, "initRtspClient: ")
        rtspClient = RtspClient(this)
    }

    override fun onFrame(frame: ByteArray, nChannel: Int, width: Int, height: Int) {
        if (isStop) {
            return
        }
        val area = width * height
        val pixels = IntArray(area)
        for (i in 0 until area) {
            var r = frame[3 * i].toInt()
            var g = frame[3 * i + 1].toInt()
            var b = frame[3 * i + 2].toInt()
            if (r < 0) r += 255
            if (g < 0) g += 255
            if (b < 0) b += 255
            pixels[i] = Color.rgb(r, g, b)
        }
        bmp?.apply {
            setPixels(pixels, 0, width, 0, 0, width, height)
            drawBitmap(this)
        } ?: apply {
            bmp = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888)
        }
    }

    private fun drawBitmap(bitmap: Bitmap) {
        //锁定画布
        val canvas = lockCanvas()
        if (canvas != null) {
            //清空画布
            canvas.drawColor(Color.TRANSPARENT, PorterDuff.Mode.CLEAR)
            //将bitmap画到画布上
            canvas.drawBitmap(bitmap, null, mDstRect, null)
            //解锁画布同时提交
            unlockCanvasAndPost(canvas)
        }
    }

    override fun onSurfaceTextureAvailable(surface: SurfaceTexture, width: Int, height: Int) {
        Log.i(TAG, "onSurfaceTextureAvailable: ")
        isStop = false
        mDstRect.set(0, 0, getWidth(), getWidth())
        job = initRtsp()
    }

    override fun onSurfaceTextureSizeChanged(surface: SurfaceTexture, width: Int, height: Int) {
        Log.i(TAG, "onSurfaceTextureSizeChanged: ")
    }

    override fun onSurfaceTextureDestroyed(surface: SurfaceTexture): Boolean {
        Log.i(TAG, "onSurfaceTextureDestroyed: ")
        isStop = true
        job.cancel()
        rtspClient.stop()
        SystemClock.sleep(200)
        rtspClient.dispose()
        return true
    }

    override fun onSurfaceTextureUpdated(surface: SurfaceTexture) {
        // Log.i(TAG, "onSurfaceTextureUpdated: ");
    }

}
