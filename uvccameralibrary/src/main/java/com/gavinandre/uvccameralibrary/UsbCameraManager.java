package com.gavinandre.uvccameralibrary;

import android.content.Context;
import android.hardware.usb.UsbDevice;
import android.support.annotation.IntDef;
import android.util.Log;

import java.util.Arrays;
import java.util.List;


public class UsbCameraManager {
    private static final String TAG = UsbCameraManager.class.getSimpleName();
    
    private static UsbCameraManager mUsbCameraManager;
    private USBMonitor mUsbMonitor;
    private Context mContext;
    private UsbDevice[] mDeviceArr = new UsbDevice[2];
    private USBMonitor.UsbControlBlock[] mCtrlBlockArr = new USBMonitor.UsbControlBlock[2];
    private Boolean[] mIsOpenArr = new Boolean[2];
    private CameraViewInterface[] mCamViewArr = new CameraViewInterface[2];
    private int streamMode;
    
    public final static int ASYNC_CALLBACK = 0;
    public final static int SYNC_POLLING = 1;
    
    @IntDef({ASYNC_CALLBACK, SYNC_POLLING})
    @interface StreamMode {
    }
    
    private UsbCameraManager() {
    }
    
    public static UsbCameraManager getInstance() {
        if (mUsbCameraManager == null) {
            mUsbCameraManager = new UsbCameraManager();
        }
        
        return mUsbCameraManager;
    }
    
    public void init(Context context, final OnUsbConnectListener listener) {
        this.mContext = context;
        
        Arrays.fill(mDeviceArr, null);
        Arrays.fill(mCtrlBlockArr, null);
        Arrays.fill(mCamViewArr, null);
        Arrays.fill(mIsOpenArr, null);
        
        mUsbMonitor = new USBMonitor(context, new USBMonitor.OnDeviceConnectListener() {
            @Override
            public void onAttach(UsbDevice device) {
                Log.i(TAG, "onAttach: ");
                if (listener != null) {
                    listener.onAttachDev(device);
                }
            }
            
            @Override
            public void onDetach(UsbDevice device) {
                Log.i(TAG, "onDetach: ");
                if (listener != null) {
                    listener.onDetachDev(device);
                }
            }
            
            @Override
            public void onConnect(UsbDevice device, USBMonitor.UsbControlBlock ctrlBlock, boolean createNew) {
                Log.i(TAG, "onConnect: ");
                if (listener != null) {
                    listener.onConnectDev(device, ctrlBlock, createNew);
                }
            }
            
            @Override
            public void onDisconnect(UsbDevice device, USBMonitor.UsbControlBlock ctrlBlock) {
                Log.i(TAG, "onDisconnect: ");
                if (listener != null) {
                    listener.onDisconnectDev(device, ctrlBlock);
                }
            }
            
            @Override
            public void onCancel(UsbDevice device) {
                Log.i(TAG, "onCancel: ");
            }
        });
    }
    
    public void register() {
        if (mUsbMonitor == null) {
            Log.e(TAG, "register: USBMonitor not init");
            return;
        }
        mUsbMonitor.register();
    }
    
    public void unregister() {
        if (mUsbMonitor == null) {
            Log.e(TAG, "unregister: USBMonitor not init");
            return;
        }
        mUsbMonitor.unregister();
    }
    
    public void release() {
        mContext = null;
        if (mUsbMonitor == null) {
            Log.e(TAG, "release: USBMonitor not init");
            return;
        }
        mUsbMonitor.destroy();
        mUsbMonitor = null;
    }
    
    public void setStreamMode(@StreamMode int streamMode) {
        this.streamMode = streamMode;
    }
    
    public boolean isRequestPermission(UsbDevice dev) {
        return findUsbDeviceIndex(dev) >= 0;
    }
    
    public void requestPermission(UsbDevice dev) {
        if (mUsbMonitor == null) {
            Log.e(TAG, "requestPermission: UsbMonitor not init");
            return;
        }
        for (int i = 0; i < mDeviceArr.length; i++) {
            if (mDeviceArr[i] == null) {
                mDeviceArr[i] = dev;
                Log.i(TAG, "requestPermission: index " + i);
                mUsbMonitor.requestPermission(dev);
                return;
            }
        }
    }
    
    public void requestPermission(UsbDevice dev, int index) {
        if (mUsbMonitor == null) {
            Log.e(TAG, "requestPermission: UsbMonitor not init");
            return;
        }
        
        mDeviceArr[index] = dev;
        Log.i(TAG, "requestPermission: index " + index);
        mUsbMonitor.requestPermission(dev);
    }
    
    public boolean isCameraOpened(USBMonitor.UsbControlBlock ctrlBlock) {
        return findUsbControlBlockIndex(ctrlBlock) >= 0;
    }
    
    public boolean isAllCameraOpened() {
        return mCtrlBlockArr[0] != null && mCtrlBlockArr[1] != null;
    }
    
    public boolean isCameraOpenFail() {
        return (mIsOpenArr[0] != null && !mIsOpenArr[0]) || (mIsOpenArr[1] != null && !mIsOpenArr[1]);
    }
    
    public boolean openCamera(UsbDevice dev, USBMonitor.UsbControlBlock ctrlBlock, CameraViewInterface cameraView) {
        int index = findUsbDeviceIndex(dev);
        if (index >= 0) {
            Log.i(TAG, "openCamera: index " + index);
            Log.i(TAG, "openCamera: cameraView " + cameraView);
            int result = openCamera(index, ctrlBlock);
            if (result == 0) {
                mCtrlBlockArr[index] = ctrlBlock;
                mIsOpenArr[index] = true;
                if (cameraView != null) {
                    cameraView.openCamera(index);
                    mCamViewArr[index] = cameraView;
                }
                return true;
            } else {
                mIsOpenArr[index] = false;
                Log.e(TAG, "openCamera: error " + result);
            }
        } else {
            Log.e(TAG, "openCamera: usbDevice not found");
        }
        return false;
    }
    
    public int openCamera(int index, USBMonitor.UsbControlBlock ctrlBlock) {
        Log.i(TAG, "openCamera: ");
        Log.i(TAG, "getVendorId: " + ctrlBlock.getVendorId());
        Log.i(TAG, "getProductId: " + ctrlBlock.getProductId());
        Log.i(TAG, "getFileDescriptor: " + ctrlBlock.getFileDescriptor());
        Log.i(TAG, "getBusNum: " + ctrlBlock.getBusNum());
        Log.i(TAG, "getDevNum: " + ctrlBlock.getDevNum());
        Log.i(TAG, "getUSBFSName: " + ctrlBlock.getUSBFSName());
        UsbCameraLib.setStreamMode(streamMode);
        return UsbCameraLib.connect(index, ctrlBlock.getVendorId(), ctrlBlock.getProductId(),
                ctrlBlock.getFileDescriptor(), ctrlBlock.getBusNum(), ctrlBlock.getDevNum(),
                ctrlBlock.getUSBFSName());
    }
    
    public void closeCamera() {
        for (int i = 0; i < mDeviceArr.length; i++) {
            closeCamera(mDeviceArr[i]);
        }
    }
    
    public void closeCamera(UsbDevice dev) {
        int index = findUsbDeviceIndex(dev);
        if (index >= 0) {
            CameraViewInterface cameraView = mCamViewArr[index];
            if (cameraView != null) {
                cameraView.closeCamera();
            }
            Log.i(TAG, "closeCamera: UsbCameraLib.release start");
            UsbCameraLib.release(index);
            Log.i(TAG, "closeCamera: UsbCameraLib release finish");
            mCamViewArr[index] = null;
            mDeviceArr[index] = null;
            mCtrlBlockArr[index] = null;
            mIsOpenArr[index] = null;
        } else {
            Log.e(TAG, "closeCamera: usbDevice not found");
        }
    }
    
    public void setParam(int autoExpo, float expoTime, float gain, int brightness) {
        for (int i = 0; i < mDeviceArr.length; i++) {
            setParam(mDeviceArr[i], autoExpo, expoTime, gain, brightness);
        }
    }
    
    public boolean setParam(UsbDevice dev, int autoExpo, float expoTime, float gain, int brightness) {
        int index = findUsbDeviceIndex(dev);
        if (index >= 0) {
            return UsbCameraLib.setPara(index, autoExpo, expoTime, gain, brightness);
        }
        return false;
    }
    
    public boolean setHeatCoil(UsbDevice dev, boolean on) {
        int index = findUsbDeviceIndex(dev);
        if (index >= 0) {
            return UsbCameraLib.setHeatCoil(index, on);
        }
        return false;
    }
    
    public boolean getHeatCoil(int index) {
        return UsbCameraLib.getHeatCoil(index);
    }
    
    public int findUsbDeviceIndex(UsbDevice dev) {
        for (int i = 0; i < mDeviceArr.length; ++i) {
            UsbDevice usbDevice = mDeviceArr[i];
            if (dev.equals(usbDevice)) {
                return i;
            }
        }
        return -1;
    }
    
    public int findUsbControlBlockIndex(USBMonitor.UsbControlBlock ctrlBlock) {
        for (int i = 0; i < mCtrlBlockArr.length; ++i) {
            if (ctrlBlock.equals(mCtrlBlockArr[i])) {
                return i;
            }
        }
        return -1;
    }
    
    public int findCameraViewIndex(CameraViewInterface cameraView) {
        for (int i = 0; i < mCamViewArr.length; ++i) {
            if (cameraView.equals(mCamViewArr[i])) {
                return i;
            }
        }
        return -1;
    }
    
    public List<UsbDevice> getUsbDeviceList() {
        List<DeviceFilter> deviceFilters = DeviceFilter.getDeviceFilters(mContext, R.xml.device_filter);
        return this.mUsbMonitor != null ? this.mUsbMonitor.getDeviceList(deviceFilters) : null;
    }
    
    public interface OnUsbConnectListener {
        void onAttachDev(UsbDevice usbDevice);
        
        void onDetachDev(UsbDevice usbDevice);
        
        void onConnectDev(UsbDevice usbDevice, USBMonitor.UsbControlBlock usbControlBlock, boolean createNew);
        
        void onDisconnectDev(UsbDevice usbDevice, USBMonitor.UsbControlBlock usbControlBlock);
    }
    
}
