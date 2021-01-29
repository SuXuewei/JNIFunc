//
// Created by Administrator on 2021/1/20.
//

#include <cstdio>      //标准输入输出定义
#include <cstdlib>     //标准函数定义
#include <unistd.h>     //unix标准函数定义
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>      //文件控制定义
#include <termios.h>    //PPSIX 终端控制定义
#include <error.h>      //错误号定义
#include <cstring>
#include <jni.h>
#include "SerialPortUtil.h"
#include "StringUtil.h"
#include "LogUtil.h"
#include "SocketUtil.h"

static const char* const TAG = "SerialPort";

void setSpeed(int fd, int speed);
int setParity(int fd, int numOfDataBits, int numOfStopBits, int parity);
int openSerialPort(char *pcDevName);

extern "C" {
    JNIEXPORT jstring JNICALL Java_com_suxw_ndkfunc_util_SerialPort_translate(
            JNIEnv *env, jobject obj, jstring input)
    {
        int nSendLen = (int)((*env).GetStringUTFLength(input));
        char *pcSendData = new char[nSendLen + 1];
        char readData[512];
        const char *pcDevName = "/dev/ttyS0";
        int nReadLen = 0;

        StringUtil::init(env);
        memset(readData, 0, sizeof(readData));

        //存储输入数据
        nSendLen += 1;
        memset(pcSendData, 0, nSendLen);
        StringUtil::convertJStringToBytes(input,
                StringUtil::ENCODE_GB2312, pcSendData, &nSendLen);
        LogUtil::log(LogUtil::LOG_LEVAL_INFO, "SerialPort Test",
                (unsigned char*)pcSendData, nSendLen);

        //准备串口收发
        SerialPortUtil serialPortUtil;
        serialPortUtil.open(pcDevName);
        if (!serialPortUtil.isValued()) {
            delete pcSendData;
            pcSendData = NULL;
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
        delete pcSendData;
        pcSendData = NULL;
        serialPortUtil.close();

        sleep(1);
        SocketUtil::testSelf(0, 0);

        //return (*env).NewStringUTF(readData);
        return StringUtil::ConvBytesToJstring(readData,
                StringUtil::ENCODE_GB2312);
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
