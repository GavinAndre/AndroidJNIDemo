package com.gavinandre.jnidemo.activity

import android.graphics.BitmapFactory
import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.util.Log
import com.gavinandre.imageprocesslibrary.ImageProcess
import com.gavinandre.jnidemo.R
import com.gavinandre.jnidemo.utils.FileUtil
import kotlinx.android.synthetic.main.activity_mat_bitmap_convert.*
import kotlinx.coroutines.*

class MatBitmapConvertActivity : AppCompatActivity() {

    private val TAG = MatBitmapConvertActivity::class.java.simpleName

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_mat_bitmap_convert)
        btn1.setOnClickListener { click1() }
        btn2.setOnClickListener { click2() }
    }

    private fun click1() = runBlocking {
        GlobalScope.launch(Dispatchers.Main) {
            val bitmap = GlobalScope.async(Dispatchers.IO) {
                val s = FileUtil.getSDPath() + "/cat.png"
                Log.i(TAG, "onCreate: $s")
                val encodeImageData = ImageProcess.encode(s)
                Log.i(TAG, "onCreate: $encodeImageData")
                val bitmap = ImageProcess.decode(encodeImageData)
                return@async bitmap
            }.await()
            IVCat.setImageBitmap(bitmap)
        }
    }

    private fun click2() {
        GlobalScope.launch(Dispatchers.Main) {
            val bitmap = GlobalScope.async(Dispatchers.IO) {
                val bitmap1 = BitmapFactory.decodeResource(application.resources, R.drawable.cat)
                val bitmap2 = ImageProcess.processBitmap(bitmap1)
                bitmap1.recycle()
                return@async bitmap2
            }.await()
            IVCat.setImageBitmap(bitmap)
        }
    }

}
