package com.gavinandre.androidjnidemo.activity

import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import com.gavinandre.androidjnidemo.R
import com.gavinandre.cameraprocesslibrary.systemcamera.SystemCameraFragment


class SystemCameraActivity : AppCompatActivity() {
    
    private val TAG = SystemCameraActivity::class.java.simpleName
    
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_system_camera)
        initView()
    }
    
    override fun onDestroy() {
        super.onDestroy()
    }
    
    private fun initView() {
        val systemCameraFragment = SystemCameraFragment.newInstance()
        supportFragmentManager.beginTransaction().replace(
                R.id.home_activity_frag_container, systemCameraFragment).commitAllowingStateLoss()
    }
    
}
