cmake_minimum_required(VERSION 3.4.1)

#设置头文件目录
include_directories(${CMAKE_SOURCE_DIR}/include)

#设置jniLibs目录
set(jniLibs "${CMAKE_SOURCE_DIR}/jniLibs/${ANDROID_ABI}")

#opencv
add_library(libopencv_core SHARED IMPORTED)
set_target_properties(libopencv_core PROPERTIES
        IMPORTED_LOCATION "${jniLibs}/libopencv_core.so")

add_library(libopencv_imgcodecs SHARED IMPORTED)
set_target_properties(libopencv_imgcodecs PROPERTIES
        IMPORTED_LOCATION "${jniLibs}/libopencv_imgcodecs.so")

add_library(libopencv_imgproc SHARED IMPORTED)
set_target_properties(libopencv_imgproc PROPERTIES
        IMPORTED_LOCATION "${jniLibs}/libopencv_imgproc.so")

#jni library
add_library(image_process_lib
        SHARED
        image_process_jni.cpp
        )

find_library(log-lib log)

target_link_libraries(
        image_process_lib
        jnigraphics
        libopencv_core
        libopencv_imgcodecs
        libopencv_imgproc
        ${log-lib})
