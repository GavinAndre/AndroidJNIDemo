package com.gavinandre.rtsplibrary;

import android.content.Context;
import android.graphics.*;
import android.util.AttributeSet;
import android.util.Log;
import android.view.TextureView;

public class RtspPlayerView extends TextureView implements TextureView.SurfaceTextureListener {

    private static final String TAG = RtspPlayerView.class.getSimpleName();

    private Rect mSrcRect;
    private Rect mDstRect;
    private RtspClient rtspClient;
    private String endpoint = "rtsp://192.168.3.5:554/user=admin&password=&channel=1&stream=1.sdp?real_stream";
    // private String endpoint = "rtsp://192.168.1.123:8554/test";
    private boolean isStop;

    public RtspPlayerView(Context context) {
        this(context, null);
    }

    public RtspPlayerView(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public RtspPlayerView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    private void init() {
        //设置背景透明，记住这里是[是否不透明]
        setOpaque(false);

        //设置监听
        setSurfaceTextureListener(this);

        mSrcRect = new Rect();
        mDstRect = new Rect();

        Log.i(TAG, "init: ");
    }

    private class PlayThread extends Thread {
        @Override
        public void run() {
            try {
                initRtspClient();
                while (true) {
                    if (rtspClient.play(endpoint) == 0) {
                        break;
                    }
                    RtspPlayerView.this.sleep(5000);
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    private void initRtspClient() {
        Log.i(TAG, "initRtspClient: ");
        rtspClient = new RtspClient(new NativeCallback() {
            @Override
            public void onFrame(final byte[] frame, final int nChannel, final int width, final int height) {
                if (isStop) {
                    return;
                }
                int area = width * height;
                int pixels[] = new int[area];
                for (int i = 0; i < area; i++) {
                    int r = frame[3 * i];
                    int g = frame[3 * i + 1];
                    int b = frame[3 * i + 2];
                    if (r < 0) r += 255;
                    if (g < 0) g += 255;
                    if (b < 0) b += 255;
                    pixels[i] = Color.rgb(r, g, b);
                }
                Bitmap bmp = Bitmap.createBitmap(pixels, width, height, Bitmap.Config.ARGB_8888);
                drawBitmap(bmp);
            }
        });
    }

    @Override
    public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
        Log.i(TAG, "onSurfaceTextureAvailable: ");
        isStop = false;
        //开启线程
        PlayThread mPlayThread = new PlayThread();
        mPlayThread.start();
    }

    @Override
    public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {
        Log.i(TAG, "onSurfaceTextureSizeChanged: ");
    }

    @Override
    public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
        Log.i(TAG, "onSurfaceTextureDestroyed: ");
        isStop = true;
        Log.i(TAG, "run: 1");
        rtspClient.stop();
        Log.i(TAG, "run: 2");
        sleep(100);
        Log.i(TAG, "run: 3");
        rtspClient.dispose();
        Log.i(TAG, "run: 4");
        return true;
    }

    private void sleep(int i) {
        try {
            Thread.sleep(i);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void onSurfaceTextureUpdated(SurfaceTexture surface) {
        // Log.i(TAG, "onSurfaceTextureUpdated: ");
    }

    private void drawBitmap(Bitmap bitmap) {
        // Log.i(TAG, "drawBitmap: ");
        //锁定画布
        Canvas canvas = lockCanvas();
        if (canvas != null) {
            // 清空画布
            canvas.drawColor(Color.TRANSPARENT, PorterDuff.Mode.CLEAR);
            //这里我将2个rect抽离出去，防止重复创建
            mSrcRect.set(0, 0, bitmap.getWidth(), bitmap.getHeight());
            mDstRect.set(0, 0, getWidth(), bitmap.getHeight() * getWidth() / bitmap.getWidth());
            //将bitmap画到画布上
            canvas.drawBitmap(bitmap, mSrcRect, mDstRect, null);
            //解锁画布同时提交
            unlockCanvasAndPost(canvas);
        }
    }
}
