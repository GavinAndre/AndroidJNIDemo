package com.gavinandre.jnidemo

import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.util.Log
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

        NativeLib.disableCout()
    }
}
