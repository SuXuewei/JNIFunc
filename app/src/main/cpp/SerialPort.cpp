//
// Created by Administrator on 2021/1/20.
//

#include <stdio.h>      //标准输入输出定义
#include <stdlib.h>     //标准函数定义
#include <unistd.h>     //unix标准函数定义
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>      //文件控制定义
#include <termios.h>    //PPSIX 终端控制定义
#include <error.h>      //错误号定义
#include <string.h>
#include <jni.h>
#include "SerialPortUtil.h"

void setSpeed(int fd, int speed);
int setParity(int fd, int numOfDataBits, int numOfStopBits, int parity);
int openSerialPort(char *pcDevName);

extern "C" {
    JNIEXPORT jstring JNICALL Java_com_suxw_ndkfunc_util_SerialPort_getStringFromCpp(
            JNIEnv *env, jobject obj, jstring input)
    {
        return (*env).NewStringUTF("Hello JNI From C++");
    }

    JNIEXPORT void JNICALL Java_com_suxw_ndkfunc_util_SerialPort_testSerialPort(
            JNIEnv *env, jobject obj)
    {
        SerialPortUtil::mainTest();
    }
};
