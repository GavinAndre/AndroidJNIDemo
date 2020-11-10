
cmake_minimum_required(VERSION 3.4.1)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(libs "${CMAKE_SOURCE_DIR}/jniLibs/${ANDROID_ABI}")

add_library(libusb100 SHARED IMPORTED)
set_target_properties(libusb100 PROPERTIES IMPORTED_LOCATION ${libs}/libusb100.so)

add_library(libuvc SHARED IMPORTED)
set_target_properties(libuvc PROPERTIES IMPORTED_LOCATION ${libs}/libuvc.so)

add_library(libjpeg-turbo1500 SHARED IMPORTED)
set_target_properties(libjpeg-turbo1500 PROPERTIES IMPORTED_LOCATION ${libs}/libjpeg-turbo1500.so)

add_library(uvccamera SHARED uvc_camera.cpp)

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

target_link_libraries( # Specifies the target library.
        uvccamera
        jnigraphics
        libusb100
        libuvc
        libjpeg-turbo1500
        android
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})