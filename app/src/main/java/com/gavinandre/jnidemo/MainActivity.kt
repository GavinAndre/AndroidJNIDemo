package com.gavinandre.jnidemo

import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        NativeLib.enableCout()
        // Example of a call to a native method
        sample_text.text = NativeLib.stringFromJNI()
        NativeLib.stringToJNI("Hello World!")
        NativeLib.disableCout()
    }
}
