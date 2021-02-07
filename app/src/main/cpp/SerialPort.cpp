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
#include "Config.h"
#include "ErrorCode.h"
#include "Viewer.h"

static const char* const TAG = "SerialPort";

void setSpeed(int fd, int speed);
int setParity(int fd, int numOfDataBits, int numOfStopBits, int parity);
int openSerialPort(char *pcDevName);

extern "C" {
//    JNIEXPORT jstring JNICALL Java_com_suxw_ndkfunc_util_SerialPort_translate(
    JNIEXPORT jstring JNICALL Java_com_suxw_ndkfunc_SerialPortActivity_translate(
            JNIEnv *env, jobject obj, jstring input)
    {
        int nSendLen = (int)((*env).GetStringUTFLength(input));
        char *pcSendData = new char[nSendLen + 1];
        char readData[512];
        const char *pcDevName = "/dev/ttyS0";
        int nReadLen = 0;
        Config config;
        jstring jstrInfo;
        Viewer::init(env, obj, "displayInfo", "(Ljava/lang/String;)V");

        StringUtil::init(env);
        memset(readData, 0, sizeof(readData));
        jclass clazz = (*env).GetObjectClass(obj);
        Viewer::display("JNI status update!");

        //测试socket通讯
        Viewer::display("JNI test socket start!");
        SocketUtil::testSelf(0, 0);
        Viewer::display("JNI test socket end!");

        //存储输入数据
        Viewer::display("JNI test jstring translation start!");
        nSendLen += 1;
        memset(pcSendData, 0, nSendLen);
        StringUtil::convertJStringToBytes(input,
                StringUtil::ENCODE_GB2312, pcSendData, &nSendLen);
        LogUtil::log(LogUtil::LOG_LEVAL_INFO, "SerialPort Test",
                (unsigned char*)pcSendData, nSendLen);
        Viewer::display("JNI get input data:", (const unsigned char*)pcSendData, nSendLen);
        Viewer::display("JNI test jstring translation end!");
        Viewer::display("JNI test read config file start!");
        config.parseConfigFile(Config::FILE_PATH);
        Viewer::display("JNI test read config file end!");

        //准备串口收发
        Viewer::display("JNI test serial port start!");
        SerialPortUtil serialPortUtil;
        serialPortUtil.open(pcDevName);
        if (!serialPortUtil.isValued()) {
            delete[] pcSendData;
            pcSendData = nullptr;
            Viewer::display("JNI test serial port: open fail!");
            LOGI(TAG, "fail to open %s", pcDevName);
        }
        serialPortUtil.setSpeed(9600);
        serialPortUtil.setParity(SerialPortUtil::DATA_BITS_8,
                SerialPortUtil::STOP_BITS_1, SerialPortUtil::PARITY_N);
        serialPortUtil.write(pcSendData, nSendLen);
        while(true)
        {
            nReadLen = serialPortUtil.read(readData, 2, 5);
//            nReadLen = serialPortUtil.read(readData, 6);
            if(nReadLen == EC_TIMEOUT) {
                LOGI(TAG, "time out", NULL, 0);
                Viewer::display("JNI test serial port: read timeout!");
            }
            if(nReadLen > 0)
            {
                LogUtil::logcat(LogUtil::LOG_LEVAL_INFO, "serial read data",
                        (unsigned char*)readData, nReadLen);
                Viewer::display("JNI test serial port: read data succ!");
                break;
            }
        }
        delete[] pcSendData;
        pcSendData = nullptr;
        serialPortUtil.close();
        Viewer::display("JNI test serial port end!");

        //根据gb2312编码的字符数据生成jstring对象返回给接口调用者
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

