package com.gavinandre.jnidemo.activity

import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.util.Log
import com.gavinandre.jnidemo.NativeLib
import com.gavinandre.jnidemo.R
import kotlinx.android.synthetic.main.activity_native.*

class NativeActivity : AppCompatActivity() {

    private val TAG = NativeActivity::class.java.simpleName

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_native)
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

        val sdPath = NativeLib.getSDPath()

        Log.i(TAG, "onCreate: getSDPath: $sdPath")

        NativeLib.disableCout()
    }
}
