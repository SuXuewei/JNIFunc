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
#include "LogUtil.h"

static const char* const TAG = "SerialPort";

void setSpeed(int fd, int speed);
int setParity(int fd, int numOfDataBits, int numOfStopBits, int parity);
int openSerialPort(char *pcDevName);

extern "C" {
    JNIEXPORT jstring JNICALL Java_com_suxw_ndkfunc_util_SerialPort_translate(
            JNIEnv *env, jobject obj, jstring input)
    {
        const char *pcInputData = (*env).GetStringUTFChars(input, NULL);
        int nSendLen = (int)((*env).GetStringUTFLength(input));
        char *pcSendData = new char[nSendLen];
        char readData[512];
        const char *pcDevName = "/dev/ttyGS0";
        int nReadLen = 0;

        memset(readData, 0, sizeof(readData));
        //存储输入数据
        memset(pcSendData, 0, nSendLen);
        (*env).ReleaseStringUTFChars(input, pcInputData);

        //准备串口收发
        SerialPortUtil serialPortUtil;
        serialPortUtil.open(pcDevName);
        if (!serialPortUtil.isValued()) {
            LOGI(TAG, "fail to open %s", pcDevName);
        }
        serialPortUtil.setSpeed(9600);
        serialPortUtil.setParity(SerialPortUtil::DATA_BITS_8,
                SerialPortUtil::STOP_BITS_1, SerialPortUtil::PARITY_N);
        serialPortUtil.write(pcSendData, nSendLen);
        while(true)
        {
            nReadLen = serialPortUtil.read(readData, sizeof(readData));
            if(nReadLen > 0)
            {
                break;
            }
        }
        serialPortUtil.close();

        return (*env).NewStringUTF(readData);
    }

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
