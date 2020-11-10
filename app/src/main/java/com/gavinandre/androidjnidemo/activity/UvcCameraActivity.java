package com.gavinandre.androidjnidemo.activity;

import android.app.Activity;
import android.hardware.usb.UsbDevice;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.WindowManager;
import android.widget.Toast;

import com.gavinandre.androidjnidemo.R;
import com.gavinandre.uvccameralibrary.CameraPreview;
import com.gavinandre.uvccameralibrary.USBMonitor;
import com.gavinandre.uvccameralibrary.UsbCameraLib;
import com.gavinandre.uvccameralibrary.UsbCameraManager;

import java.util.concurrent.ScheduledExecutorService;

public class UvcCameraActivity extends Activity implements UsbCameraManager.OnUsbConnectListener {
    
    private static final String TAG = UvcCameraActivity.class.getSimpleName();
    
    private CameraPreview cameraPreviewOne;
    private CameraPreview cameraPreviewTwo;
    private UsbCameraManager mManager;
    private ScheduledExecutorService executorService;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_uvc_dual_camera);
        cameraPreviewOne = findViewById(R.id.frontCameraView);
        cameraPreviewTwo = findViewById(R.id.backCameraView);
        findViewById(R.id.bt_take_photo).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                takePhoto();
            }
        });
        mManager = UsbCameraManager.getInstance();
        mManager.init(this.getApplicationContext(), this);
        // mManager.setStreamMode(UsbCameraManager.ASYNC_CALLBACK);
        mManager.setStreamMode(UsbCameraManager.SYNC_POLLING);
        // executorService = Executors.newScheduledThreadPool(1);
        // executorService.scheduleAtFixedRate(new Runnable() {
        //     @Override
        //     public void run() {
        //         Log.i(TAG, "run: frame " + cameraPreviewOne.getFrame());
        //         Log.i(TAG, "run: frame " + cameraPreviewTwo.getFrame());
        //         cameraPreviewOne.setFrame(0);
        //         cameraPreviewTwo.setFrame(0);
        //     }
        // }, 1, 1, TimeUnit.SECONDS);
    }
    
    @Override
    protected void onStart() {
        super.onStart();
        mManager.register();
    }
    
    @Override
    protected void onStop() {
        super.onStop();
        mManager.unregister();
    }
    
    @Override
    protected void onDestroy() {
        super.onDestroy();
        mManager.release();
        // executorService.shutdownNow();
    }
    
    @Override
    public void onAttachDev(UsbDevice usbDevice) {
        Log.i(TAG, "onAttachDev: USB_DEVICE_ATTACHED " + usbDevice.toString());
        runOnUiThread(() -> Toast.makeText(this, "USB_DEVICE_ATTACHED", Toast.LENGTH_SHORT).show());
        if (!mManager.isRequestPermission(usbDevice)) {
            mManager.requestPermission(usbDevice);
        }
    }
    
    @Override
    public void onDetachDev(UsbDevice usbDevice) {
        Log.i(TAG, "onDetachDev: USB_DEVICE_DETACHED " + usbDevice.toString());
        runOnUiThread(() -> Toast.makeText(this, "USB_DEVICE_DETACHED", Toast.LENGTH_SHORT).show());
        mManager.closeCamera(usbDevice);
    }
    
    @Override
    public void onConnectDev(UsbDevice usbDevice, USBMonitor.UsbControlBlock usbControlBlock, boolean createNew) {
        Log.i(TAG, "onConnectDev: USB_DEVICE_CONNECT " + usbDevice.toString());
        if (!mManager.isCameraOpened(usbControlBlock)) {
            boolean ok = false;
            if (mManager.findCameraViewIndex(cameraPreviewOne) < 0) {
                ok = mManager.openCamera(usbDevice, usbControlBlock, cameraPreviewOne);
            } else if (mManager.findCameraViewIndex(cameraPreviewTwo) < 0) {
                ok = mManager.openCamera(usbDevice, usbControlBlock, cameraPreviewTwo);
            }
            if (ok) setParam(usbDevice);
            if (mManager.isCameraOpenFail()) resetUsb();
        }
    }
    
    @Override
    public void onDisconnectDev(UsbDevice usbDevice, USBMonitor.UsbControlBlock usbControlBlock) {
        Log.i(TAG, "onDisconnectDev: USB_DEVICE_DISCONNECT " + usbDevice.toString());
        mManager.closeCamera(usbDevice);
    }
    
    private void takePhoto() {
        String jpg1 = "/sdcard/DCIM/0_" + System.currentTimeMillis() + ".jpg";
        boolean ok1 = UsbCameraLib.takePhoto(0, jpg1);
        String jpg2 = "/sdcard/DCIM/1_" + System.currentTimeMillis() + ".jpg";
        boolean ok2 = UsbCameraLib.takePhoto(1, jpg2);
        Log.e(TAG, "takePhoto: cam0 " + ok1 + ", cam1 " + ok2);
    }
    
    private void setParam(UsbDevice usbDevice) {
        mManager.setParam(usbDevice, 0, 0.0157f, 0, 50);
        // mManager.setParam(usbDevice, 1, 0.015f, 15, 50);
    }
    
    private void resetUsb() {
        // 调用重置USB接口，比如MTK平台的切换OTG
    }

}
