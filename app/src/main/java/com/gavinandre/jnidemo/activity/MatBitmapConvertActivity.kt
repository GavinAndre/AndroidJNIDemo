package com.gavinandre.jnidemo.activity

import android.graphics.BitmapFactory
import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.util.Log
import com.gavinandre.imageprocesslibrary.ImageProcess
import com.gavinandre.jnidemo.NativeLib
import com.gavinandre.jnidemo.R
import com.gavinandre.jnidemo.utils.FileUtil
import kotlinx.android.synthetic.main.activity_native.*

class MatBitmapConvertActivity : AppCompatActivity() {

    private val TAG = MatBitmapConvertActivity::class.java.simpleName

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_mat_bitmap_convert)
        NativeLib.enableCout()

        val s = FileUtil.getSDPath() + "/cat.png"
        Log.i(TAG, "onCreate: $s")
        val encodeImageData = ImageProcess.encode(s)
        Log.i(TAG, "onCreate: $encodeImageData")
//        val bitmap1 = ImageProcess.decode(encodeImageData)
        val bitmap1 = BitmapFactory.decodeResource(application.resources, R.drawable.cat)

        val bitmap2 = ImageProcess.processBitmap(bitmap1)
//        bitmap1.recycle()
        IVCat.setImageBitmap(bitmap2)

        NativeLib.disableCout()
    }
}
