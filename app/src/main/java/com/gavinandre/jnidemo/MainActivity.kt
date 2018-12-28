package com.gavinandre.jnidemo

import android.graphics.BitmapFactory
import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.util.Log
import com.lingzhi.imageprocesslibrary.ImageProcess
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    private val TAG = MainActivity::class.java.simpleName

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        NativeLib.enableCout()
        // Example of a call to a native method
        sample_text.text = NativeLib.stringFromJNI()
        NativeLib.stringToJNI("Hello World!")

        val list = arrayListOf("1", "2", "3")
        NativeLib.listToJNI(list)

        val list2 = NativeLib.listFromJNI()

        Log.i(TAG, "onCreate: listFromJni: $list2")

        val uuid = NativeLib.uuidFromJNI()

        Log.i(TAG, "onCreate: uuidFromJNI: $uuid")

        val s = FileUtil.getInstance().sdPath + "/cat.png"
        Log.i(TAG, "onCreate: $s")
        val encodeImageData = ImageProcess.Encode(s)
        Log.i(TAG, "onCreate: $encodeImageData")
//        val bitmap1 = ImageProcess.Decode(encodeImageData)
        val bitmap1 = BitmapFactory.decodeResource(application.resources, R.drawable.cat)

        val bitmap2 = ImageProcess.processBitmap(bitmap1)
//        bitmap1.recycle()
        IVCat.setImageBitmap(bitmap2)

        NativeLib.disableCout()
    }
}
