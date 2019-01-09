package com.gavinandre.cameraprocesslibrary.usbcamera

import android.os.Bundle
import android.support.v4.app.Fragment
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import com.gavinandre.cameraprocesslibrary.R
import com.gavinandre.cameraprocesslibrary.utils.Camera1Manager
import kotlinx.android.synthetic.main.fragment_usb_camera.*
import java.util.concurrent.Executors
import java.util.concurrent.ScheduledExecutorService
import java.util.concurrent.TimeUnit

class UsbCameraFragment : Fragment() {
    
    private val TAG = UsbCameraFragment::class.java.simpleName
    
    private var param1: String? = null
    private var param2: String? = null
    
    private val executorService: ScheduledExecutorService
            by lazy { Executors.newScheduledThreadPool(1) }
    
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        arguments?.let {
            param1 = it.getString(ARG_PARAM1)
            param2 = it.getString(ARG_PARAM2)
        }
    }
    
    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?,
                              savedInstanceState: Bundle?): View? {
        // Inflate the layout for this fragment
        return inflater.inflate(R.layout.fragment_usb_camera, container, false)
    }
    
    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        initView()
        initData()
    }
    
    override fun onDestroyView() {
        super.onDestroyView()
        executorService.shutdown()
    }
    
    private fun initData() {
        executorService.scheduleAtFixedRate({
            Log.i(TAG, "run: drawFrame" + UsbCameraTextureView.drawFrame)
            Log.i(TAG, "run: cameraFrame " + Camera1Manager.systemCameraFrame)
            UsbCameraTextureView.drawFrame = 0
            Camera1Manager.systemCameraFrame = 0
        }, 1, 1, TimeUnit.SECONDS)
    }
    
    private fun initView() {
        usbCameraTextureView.visibility = View.VISIBLE
    }
    
    companion object {
        
        private const val ARG_PARAM1 = "param1"
        private const val ARG_PARAM2 = "param2"
        
        @JvmStatic
        fun newInstance() = UsbCameraFragment()
        
        @JvmStatic
        fun newInstance(param1: String, param2: String) =
                UsbCameraFragment().apply {
                    arguments = Bundle().apply {
                        putString(Companion.ARG_PARAM1, param1)
                        putString(Companion.ARG_PARAM2, param2)
                    }
                }
    }
}