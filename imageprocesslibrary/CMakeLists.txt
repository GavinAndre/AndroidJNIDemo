cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -stdlib=libc++")

set(CMAKE_VERBOSE_MAKEFILE on)
set(libs "${CMAKE_SOURCE_DIR}/src/main/jniLibs")
set(jniLibs "${libs}/${ANDROID_ABI}")

#include
include_directories(${CMAKE_SOURCE_DIR}/src/main/cpp/include)

#opencv
add_library(libopencv_java3 SHARED IMPORTED )
set_target_properties(libopencv_java3 PROPERTIES
    IMPORTED_LOCATION "${jniLibs}/libopencv_world.so")

add_library(image_process-lib
            SHARED
            src/main/cpp/image_process.cpp
            src/main/cpp/base64.cpp
            )

find_library(log-lib log)

target_link_libraries(image_process-lib
                      jnigraphics
                      libopencv_java3
                      ${log-lib})
