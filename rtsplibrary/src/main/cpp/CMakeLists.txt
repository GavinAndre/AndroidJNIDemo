cmake_minimum_required(VERSION 3.4.1)

#设置头文件目录
include_directories(${CMAKE_SOURCE_DIR}/include)

#设置jniLibs目录
set(jniLibs "${CMAKE_SOURCE_DIR}/jniLibs/${ANDROID_ABI}")

#ffmpeg
add_library(avcodec-lib SHARED IMPORTED)
set_target_properties(avcodec-lib PROPERTIES IMPORTED_LOCATION
        ${jniLibs}/libavcodec-57.so)

add_library(avformat-lib SHARED IMPORTED)
set_target_properties(avformat-lib PROPERTIES IMPORTED_LOCATION
        ${jniLibs}/libavformat-57.so)

add_library(avutil-lib SHARED IMPORTED)
set_target_properties(avutil-lib PROPERTIES IMPORTED_LOCATION
        ${jniLibs}/libavutil-55.so)

add_library(swresample-lib SHARED IMPORTED)
set_target_properties(swresample-lib PROPERTIES IMPORTED_LOCATION
        ${jniLibs}/libswresample-2.so)

add_library(swscale-lib SHARED IMPORTED)
set_target_properties(swscale-lib PROPERTIES IMPORTED_LOCATION
        ${jniLibs}/libswscale-4.so)

add_library(rtsp_lib
        SHARED
        native-lib.cpp)

find_library(log-lib
        log)

target_link_libraries(
        rtsp_lib
        ${log-lib}
        avcodec-lib
        avformat-lib
        avutil-lib
        swresample-lib
        swscale-lib)
