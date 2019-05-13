package com.gavinandre.cameraprocesslibrary.systemcamera

import android.os.Bundle
import android.support.v4.app.Fragment
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import com.gavinandre.cameraprocesslibrary.R
import com.gavinandre.cameraprocesslibrary.utils.Camera1Manager1
import kotlinx.android.synthetic.main.fragment_system_camera1.*
import kotlinx.coroutines.*

class SystemCameraFragment1 : Fragment() {

    private val TAG = SystemCameraFragment1::class.java.simpleName

    private var param1: String? = null
    private var param2: String? = null

    private lateinit var job: Job

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        arguments?.let {
            param1 = it.getString(ARG_PARAM1)
            param2 = it.getString(ARG_PARAM2)
        }
    }

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View? {
        return inflater.inflate(R.layout.fragment_system_camera1, container, false)
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        initView()
        initData()
    }

    override fun onDestroyView() {
        super.onDestroyView()
        job.cancel()
    }

    private fun initData() {
        job = GlobalScope.launch {
            while (isActive) {
                Log.i(TAG, "run: drawFrame" + SystemCameraTextureView1.drawFrame)
                Log.i(TAG, "run: cameraFrame " + Camera1Manager1.systemCameraFrame)
                SystemCameraTextureView1.drawFrame = 0
                Camera1Manager1.systemCameraFrame = 0
                delay(1000L)
            }
        }
    }

    private fun initView() {
        systemCameraTextureView1.visibility = View.VISIBLE
    }

    companion object {

        private const val ARG_PARAM1 = "param1"
        private const val ARG_PARAM2 = "param2"

        @JvmStatic
        fun newInstance() = SystemCameraFragment1()

        @JvmStatic
        fun newInstance(param1: String, param2: String) =
            SystemCameraFragment1().apply {
                arguments = Bundle().apply {
                    putString(ARG_PARAM1, param1)
                    putString(ARG_PARAM2, param2)
                }
            }
    }
}