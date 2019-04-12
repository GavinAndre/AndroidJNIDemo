package com.gavinandre.androidjnidemo.activity

import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import com.gavinandre.androidjnidemo.R
import com.gavinandre.rtsplibrary.RtspEndpointInterface
import kotlinx.android.synthetic.main.activity_rtsp.*

class RtspActivity : AppCompatActivity() {

    private val TAG = RtspActivity::class.java.simpleName

    private val endpoint = "rtsp://192.168.3.5:554/user=admin&password=&channel=1&stream=1.sdp?real_stream"

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_rtsp)
        mRtspPlayerView.setRtspEndpointListener(RtspEndpointInterface { endpoint })
    }

}
