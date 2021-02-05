//
// Created by Administrator on 2021/1/25.
//

#ifndef NDKFUNC_SERIALPORTUTIL_H
#define NDKFUNC_SERIALPORTUTIL_H

#include <stdio.h>      //标准输入输出定义
#include <stdlib.h>     //标准函数定义
#include <unistd.h>     //unix标准函数定义
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>      //文件控制定义
#include <termios.h>    //PPSIX 终端控制定义
#include <error.h>      //错误号定义

class SerialPortUtil {

public:
    SerialPortUtil();
    ~SerialPortUtil();
    //打开串口
    bool open(const char *pcDevName);
    //关闭串口
    void close();
    //设置波特率
    void setSpeed(int speed);
    //设置通讯参数
    bool setParity(int numOfDataBits, int numOfStopBits, int parity);
    //写串口
    int write(char *pcSendBytes, int nSendLen);
    //读串口
    int read(char *pcReadBytes, int nNeedReadLen, int nTimeOut = TIME_OUT_DISABLE);
    //判定文件句柄是否有效
    inline bool isValued() const;

    //自测函数
    static void mainTest();

private:
    //获取波特率
    speed_t getBaudrate(int baudrate);
    //计算超时时间，以秒为单位
    int calcTimeOut(struct timeval tvSta);

public:
    //data bits
    static const int DATA_BITS_8 = 8;
    static const int DATA_BITS_7 = 7;
    //stop bits
    static const int STOP_BITS_1 = 1;
    static const int STOP_BITS_2 = 2;
    //parity
    static const int PARITY_N = 'n';
    static const int PARITY_E = 'e';
    static const int PARITY_S = 's';
    static const int PARITY_O = 'o';

private:
    static const char* const TAG;

    static const int TIME_OUT_DISABLE = -1;   //秒

    //串口句柄
    int m_fd;
};

#endif //NDKFUNC_SERIALPORTUTIL_H
