cmake_minimum_required(VERSION 3.4.1)

add_library(StringTools
            SHARED
        cpp/StringTools.c)

add_library(SerialPort
        SHARED
        cpp/Viewer.cpp
        cpp/Controller.cpp
        cpp/BytesUtil.cpp
        cpp/TimerUtil.cpp
        cpp/Config.cpp
        cpp/LogUtil.cpp
        cpp/StringUtil.cpp
        cpp/SocketUtil.cpp
        cpp/SerialPortUtil.cpp
        cpp/SerialPort.cpp)

target_link_libraries(SerialPort
        android
        log)

target_link_libraries(StringTools
                      android
                      log)
