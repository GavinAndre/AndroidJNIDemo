package com.gavinandre.androidjnidemo.activity

import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import com.gavinandre.androidjnidemo.R
import com.gavinandre.cameraprocesslibrary.usbcamera.UsbCameraFragment

class UsbCameraActivity : AppCompatActivity() {
    
    private val TAG = UsbCameraActivity::class.java.simpleName
    
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_usb_camera)
        initView()
    }
    
    override fun onDestroy() {
        super.onDestroy()
    }
    
    private fun initView() {
        val usbCameraFragment = UsbCameraFragment.newInstance()
        supportFragmentManager.beginTransaction().replace(
                R.id.home_activity_frag_container, usbCameraFragment).commitAllowingStateLoss()
    }
    
}
