package com.gavinandre.jnidemo.activity

import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.util.Log
import com.gavinandre.jnidemo.NativeLib
import com.gavinandre.jnidemo.R
import com.gavinandre.jnidemo.bean.ComplexBean
import com.gavinandre.jnidemo.bean.SimpleBean
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

        val intList = arrayListOf(1, 2, 3)
        NativeLib.intListToJNI(intList)

        val doubleList = arrayListOf(1.1, 2.2, 3.3)
        NativeLib.doubleListToJNI(doubleList)

        val stringList = arrayListOf("x", "y", "z")
        NativeLib.stringListToJNI(stringList)

        val simpleBeanList = arrayListOf(
            SimpleBean(11), SimpleBean(22), SimpleBean(33)
        )
        NativeLib.simpleObjectListToJNI(simpleBeanList)

        val complexBeanList = arrayListOf(
            ComplexBean(11, arrayListOf(1.1f, 1.2f, 1.3f)),
            ComplexBean(22, arrayListOf(2.1f, 2.2f, 2.3f)),
            ComplexBean(33, arrayListOf(3.1f, 3.2f, 3.3f))
        )
        NativeLib.complexObjectListToJNI(complexBeanList)

        val intList2 = NativeLib.intListFromJNI()
        Log.i(TAG, "onCreate: intListFromJNI: $intList2")

        val doubleList2 = NativeLib.doubleListFromJNI()
        Log.i(TAG, "onCreate: doubleListFromJNI: $doubleList2")

        val stringList2 = NativeLib.stringListFromJNI()
        Log.i(TAG, "onCreate: stringListFromJNI: $stringList2")

        val simpleObjectList2 = NativeLib.simpleObjectListFromJNI()
        Log.i(TAG, "onCreate: simpleObjectListFromJNI: $simpleObjectList2")

        val complexObjectList2 = NativeLib.complexObjectListFromJNI()
        Log.i(TAG, "onCreate: complexObjectListFromJNI: $complexObjectList2")

        val uuid = NativeLib.uuidFromJNI()
        Log.i(TAG, "onCreate: uuidFromJNI: $uuid")

        val sdPath = NativeLib.getSDPath()
        Log.i(TAG, "onCreate: getSDPath: $sdPath")

        val baseStr = NativeLib.base64Encode()
        Log.i(TAG, "onCreate: base64Encode: $baseStr")

        NativeLib.base64Decode(baseStr)

        NativeLib.disableCout()
    }
}
