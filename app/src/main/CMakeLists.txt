cmake_minimum_required(VERSION 3.4.1)

add_library(StringUtil
            SHARED
            ./cpp/StringUtil.c)

target_link_libraries(StringUtil
                      android
                      log)
