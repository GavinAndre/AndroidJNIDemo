package com.gavinandre.jnidemo.activity

import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.util.Log
import android.view.View
import android.widget.Toast
import com.gavinandre.imageprocesslibrary.ImageProcess
import com.gavinandre.jnidemo.R
import com.gavinandre.jnidemo.utils.FileUtil
import kotlinx.android.synthetic.main.activity_mat_bitmap_convert.*
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.async
import kotlinx.coroutines.launch
import java.util.*


class MatBitmapConvertActivity : AppCompatActivity() {

    private val TAG = MatBitmapConvertActivity::class.java.simpleName

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_mat_bitmap_convert)
        btn1.setOnClickListener { click1() }
        btn2.setOnClickListener { click2() }
    }

    private fun click1() {
        GlobalScope.launch(Dispatchers.Main) {
            btn2.visibility = View.INVISIBLE
            val savePath = GlobalScope.async(Dispatchers.IO) {
                val savePath = FileUtil.getSDPath() + "/cat.png"
                val bitmap = FileUtil.getImageFromAssetsFile(applicationContext, "cat.png")
                val startDate = Date(System.currentTimeMillis())
                ImageProcess.bitmapToPng(savePath, bitmap)
                val endDate = Date(System.currentTimeMillis())
                val diff = endDate.time - startDate.time
                Log.i(TAG, "click1: diff $diff")
                bitmap.recycle()
                return@async savePath
            }.await()

            btn2.visibility = View.VISIBLE
            Toast.makeText(applicationContext, "保存路径 $savePath", Toast.LENGTH_SHORT).show()
        }
    }

    private fun click2() {
        GlobalScope.launch(Dispatchers.Main) {
            val bitmap = GlobalScope.async(Dispatchers.IO) {
                val savePath = FileUtil.getSDPath() + "/cat.png"
                val bitmap = ImageProcess.pngToBitmap(savePath)
                return@async bitmap
            }.await()

            IVCat.setImageBitmap(bitmap)
        }
    }

}
