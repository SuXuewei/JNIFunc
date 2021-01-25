cmake_minimum_required(VERSION 3.4.1)

add_library(StringUtil
            SHARED
            ./cpp/StringUtil.c)

add_library(SerialPort
        SHARED
        cpp/SerialPortUtil.cpp
        cpp/SerialPort.cpp)

target_link_libraries(SerialPort
        android
        log)

target_link_libraries(StringUtil
                      android
                      log)
