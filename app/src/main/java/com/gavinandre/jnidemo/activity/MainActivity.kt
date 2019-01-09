package com.gavinandre.jnidemo.activity

import android.Manifest
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Build
import android.os.Bundle
import android.support.v4.content.ContextCompat
import android.support.v7.app.AppCompatActivity
import com.gavinandre.jnidemo.R
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    private val TAG = MainActivity::class.java.simpleName

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        requestPermission()
        initView()
    }

    private fun initView() {
        btnNative.setOnClickListener { startActivity(Intent(this,NativeActivity::class.java)) }
        btnSystemCamera.setOnClickListener { startActivity(Intent(this, SystemCameraActivity::class.java)) }
        btnUsbCamera.setOnClickListener { startActivity(Intent(this, UsbCameraActivity::class.java)) }
        btnMatBitmapConvert.setOnClickListener { startActivity(Intent(this, MatBitmapConvertActivity::class.java)) }
    }

    override fun onDestroy() {
        super.onDestroy()
    }

    private fun requestPermission() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED ||
                ContextCompat.checkSelfPermission(this, Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED ||
                ContextCompat.checkSelfPermission(this, Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED) {
                requestPermissions(arrayOf(Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.CAMERA), 1000)
            }
        }
    }
}
