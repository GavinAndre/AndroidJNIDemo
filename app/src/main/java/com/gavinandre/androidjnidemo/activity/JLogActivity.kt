package com.gavinandre.androidjnidemo.activity

import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import com.gavinandre.androidjnidemo.R
import com.gavinandre.androidjnidemo.utils.FileUtil
import com.gavinandre.nativeloglibrary.JLogLib
import kotlinx.android.synthetic.main.activity_jlog.*

class JLogActivity : AppCompatActivity() {

    private val TAG = JLogActivity::class.java.simpleName

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_jlog)
        JLogLib.initLog(FileUtil.getSDPath() + "/jlog.txt")
        btn1.setOnClickListener { click1() }
        btn2.setOnClickListener { click2() }
    }

    override fun onDestroy() {
        super.onDestroy()
        JLogLib.closeLog()
    }

    private fun click1() {
        JLogLib.logFromJava("log from java")
    }

    private fun click2() {
        JLogLib.logFromNative()
    }

}
