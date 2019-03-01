package com.gavinandre.jnidemo.activity

import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import com.gavinandre.jnidemo.R

class RtspActivity : AppCompatActivity() {

    private val TAG = RtspActivity::class.java.simpleName

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_rtsp)
    }

}
