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

#file(GLOB native_srcs "src/main/util/v4l2/*.cpp")

add_library(camera_process_lib SHARED
        camera_process_jni.cpp
        util/v4l2/v4l_achieve.cpp
        #${native_srcs}
        )

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

target_link_libraries( # Specifies the target library.
        camera_process_lib
        jnigraphics
        android
        libopencv_core
        libopencv_imgcodecs
        libopencv_imgproc
        ${log-lib})
