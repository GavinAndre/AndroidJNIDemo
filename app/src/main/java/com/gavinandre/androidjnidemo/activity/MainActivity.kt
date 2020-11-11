package com.gavinandre.androidjnidemo.activity

import android.Manifest
import android.content.Intent
import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import com.gavinandre.androidjnidemo.R
import kotlinx.android.synthetic.main.activity_main.*
import pub.devrel.easypermissions.AfterPermissionGranted
import pub.devrel.easypermissions.AppSettingsDialog
import pub.devrel.easypermissions.EasyPermissions

class MainActivity : AppCompatActivity(), EasyPermissions.PermissionCallbacks {

    companion object {
        private val TAG = MainActivity::class.java.simpleName
        private const val RC_CAMERA_PERM = 123
        private const val RC_STORAGE_PERM1 = 124
        private const val RC_STORAGE_PERM2 = 125
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        initView()
    }

    private fun initView() {
        btnNative.setOnClickListener { startActivity(Intent(this, NativeActivity::class.java)) }
        btnSystemCamera.setOnClickListener { startSystemCameraActivity() }
        btnUsbCamera.setOnClickListener { startActivity(Intent(this, UsbCameraActivity::class.java)) }
        btnMatBitmapConvert.setOnClickListener { startMatBitmapConvertActivity() }
        btnRtsp.setOnClickListener { startActivity(Intent(this, RtspActivity::class.java)) }
        btnUvcCamera.setOnClickListener { startActivity(Intent(this, UvcCameraActivity::class.java)) }
        btnJlog.setOnClickListener { startJLogActivity() }
    }

    @AfterPermissionGranted(RC_CAMERA_PERM)
    private fun startSystemCameraActivity() {
        if (hasPermission(Manifest.permission.CAMERA)) {
            startActivity(Intent(this, SystemCameraActivity::class.java))
        } else {
            EasyPermissions.requestPermissions(
                this,
                "相机权限(用于拍照，视频聊天)",
                RC_CAMERA_PERM,
                Manifest.permission.CAMERA
            )
        }
    }

    @AfterPermissionGranted(RC_STORAGE_PERM1)
    private fun startMatBitmapConvertActivity() {
        if (hasPermission(Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.WRITE_EXTERNAL_STORAGE)) {
            startActivity(Intent(this, MatBitmapConvertActivity::class.java))
        } else {
            EasyPermissions.requestPermissions(
                this,
                "读写权限(用于读取和保存图片)",
                    RC_STORAGE_PERM1,
                Manifest.permission.READ_EXTERNAL_STORAGE,
                Manifest.permission.WRITE_EXTERNAL_STORAGE
            )
        }
    }

    @AfterPermissionGranted(RC_STORAGE_PERM2)
    private fun startJLogActivity() {
        if (hasPermission(Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.WRITE_EXTERNAL_STORAGE)) {
            startActivity(Intent(this, JLogActivity::class.java))
        } else {
            EasyPermissions.requestPermissions(
                this,
                "读写权限(用于读取和保存图片)",
                RC_STORAGE_PERM2,
                Manifest.permission.READ_EXTERNAL_STORAGE,
                Manifest.permission.WRITE_EXTERNAL_STORAGE
            )
        }
    }

    override fun onPermissionsGranted(requestCode: Int, perms: MutableList<String>) {
    }

    override fun onPermissionsDenied(requestCode: Int, perms: MutableList<String>) {
        if (EasyPermissions.somePermissionPermanentlyDenied(this, perms)) {
            AppSettingsDialog.Builder(this)
                .setTitle("申请权限")
                .setRationale("没有相关权限应用将无法正常运行，点击确定进入权限设置界面来进行更改")
                .build().show()
        }
    }

    override fun onDestroy() {
        super.onDestroy()
    }

    private fun hasPermission(vararg permission: String): Boolean {
        return EasyPermissions.hasPermissions(this, *permission)
    }

    override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<String>, grantResults: IntArray) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)

        // Forward results to EasyPermissions
        EasyPermissions.onRequestPermissionsResult(requestCode, permissions, grantResults, this)
    }

}
