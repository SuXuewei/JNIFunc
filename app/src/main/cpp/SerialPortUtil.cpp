//
// Created by Administrator on 2021/1/25.
//

#include "SerialPortUtil.h"
#include "LogUtil.h"
#include "ErrorCode.h"
#include <string.h>

//const char* 指针本身数值不能变 const SerialPortUtil::TAG指向的字符不可以改变
const char* const SerialPortUtil::TAG = "SerialPortUtil";

SerialPortUtil::SerialPortUtil() {
    m_fd = -1;
}

SerialPortUtil::~SerialPortUtil() {
    if(isValued()) {
        close();
    }
}

/************************************************************************
 * speed_t getBaudrate(int baudrate)
 * 功能：获取波特率数值
 * int baudrate - 波特率数值
 * speed_t - 串口底层使用的波特率数值
************************************************************************/
speed_t SerialPortUtil::getBaudrate(int baudrate) {
    switch (baudrate) {
        case 0: return B0;
        case 50: return B50;
        case 75: return B75;
        case 110: return B110;
        case 134: return B134;
        case 150: return B150;
        case 200: return B200;
        case 300: return B300;
        case 600: return B600;
        case 1200: return B1200;
        case 1800: return B1800;
        case 2400: return B2400;
        case 4800: return B4800;
        case 9600: return B9600;
        case 19200: return B19200;
        case 38400: return B38400;
        case 57600: return B57600;
        case 115200: return B115200;
        case 230400: return B230400;
        case 460800: return B460800;
        case 500000: return B500000;
        case 576000: return B576000;
        case 921600: return B921600;
        case 1000000: return B1000000;
        case 1152000: return B1152000;
        case 1500000: return B1500000;
        case 2000000: return B2000000;
        case 2500000: return B2500000;
        case 3000000: return B3000000;
        case 3500000: return B3500000;
        case 4000000: return B4000000;
        default: return -1;
    }
}

//打开串口
bool SerialPortUtil::open(const char *pcDevName) {
    m_fd = ::open(pcDevName, O_RDWR);
    LOGI(TAG, "open m_fd=%d", m_fd);
    if(-1 == m_fd)
    {
        return false;
    }

    return true;
}

//关闭串口
void SerialPortUtil::close() {
    ::close(m_fd);
    m_fd = -1;
}

//当前实体对象控制的串口是否有效
bool SerialPortUtil::isValued() {
    return (m_fd == -1) ? false : true;
}

/************************************************************************
 * void setSpeed(int fd, int speed)
 * 功能: 设置串口波特率
 * int fd - 串口设备句柄
 * int speed - 波特率
************************************************************************/
void SerialPortUtil::setSpeed(int speed) {
    int i = 0;
    int status = -1;
    struct termios opt;

    //获取串口设备属性
    tcgetattr(m_fd, &opt);
    //清空串口输入输出缓存
    tcflush(m_fd, TCIOFLUSH);
    //设置串口数据输入的速率
    cfsetispeed(&opt, getBaudrate(speed));
    //设置串口数据输出的速率
    cfsetospeed(&opt, getBaudrate(speed));
    //设置串口属性,之前是修改属性结构体，此处是真正修改串口属性
    status = tcsetattr(m_fd, TCSANOW, &opt);
    if(status != 0)
    {
        perror("tcsettattr fail!");
        LOGI(TAG, "tcsettattr fail!");
        return;
    }
    //清空串口输入输出缓存
    tcflush(m_fd, TCIOFLUSH);
}

/************************************************************************
 * int setParity(int fd, int numOfDataBits, int numOfStopBits, int parity)
 * 功能：设置串口数据为，停止位和校验位
 * int fd - 串口设备句柄
 * int numOfDataBits - 设置数据位个数, {7,8}
 * int numOfStopBits - 设置停止位个数, {1,2}
 * int parity - 设置校验类型, {N,E,O,S}
 * int - success:0 fail:-1
************************************************************************/
bool SerialPortUtil::setParity(int numOfDataBits, int numOfStopBits, int parity) {
    struct termios options;

    if(tcgetattr(m_fd, &options) != 0)
    {
        LOGI(TAG, "setParity tcgetattr fail!");
        return false;
    }

    //设置数据位数,实际数据占几位
    options.c_cflag &= ~CSIZE;
    switch (numOfDataBits) {
        case DATA_BITS_7:
            options.c_cflag |= CS7;
            break;
        case DATA_BITS_8:
            options.c_cflag |= CS8;
            break;
        default:
            LOGI(TAG, "Unsupport data bits size\n");
            return false;
    }

    //设置停止位数,实际数据占几位
    switch (numOfStopBits) {
        case STOP_BITS_1:
            options.c_cflag &= ~CSTOPB;
            break;
        case STOP_BITS_2:
            options.c_cflag |= CSTOPB;
            break;
        default:
            LOGI(TAG, "Unsupport stop bits size\n");
            return false;
    }

    //设置校验类型
    switch (parity) {
        case PARITY_N:
            options.c_cflag &= ~PARENB; //clear parity enable
            options.c_cflag &= ~INPCK;  //enable parity checking
            break;
        case PARITY_O:
            options.c_cflag |= (PARODD | PARENB); //设置奇校验
            options.c_cflag |= INPCK;  //disable parity checking
        case PARITY_E:
            options.c_cflag |= PARENB; //enable parity
            options.c_cflag &= ~PARODD; //转换为偶校验
            options.c_cflag |= INPCK;  //disable parity checking
        case PARITY_S:
            //as no parity
            options.c_cflag &= ~PARENB;
            options.c_cflag &= ~CSTOPB;
            break;
        default:
            LOGI(TAG, "Unsupport parity!");
            return false;
    }

    //set input parity
    if(parity != PARITY_N) {
        options.c_iflag |= INPCK;
    }
    tcflush(m_fd, TCIFLUSH);
    options.c_cc[VTIME] = 150;  //设置超时15秒
    options.c_cc[VMIN] = 0;     //update option and do it now
    if(tcsetattr(m_fd, TCSANOW, &options) != 0)
    {
        LOGI(TAG, "Setup Serial fail at last!");
        return false;
    }

    return true;
}

int SerialPortUtil::write(char *pcSendBytes, int nSendLen) {
    return ::write(m_fd, pcSendBytes, nSendLen);
}

int SerialPortUtil::read(char *pcReadBytes, int nMaxLen) {
    return ::read(m_fd, pcReadBytes, nMaxLen);
}

//测试函数
void SerialPortUtil::mainTest() {
    int nReadBytes;
    int nSendBytes;
    char sendBytes[256];
    char readBytes[256];
    char *pcDeviceName = "/dev/ttyS0";
    SerialPortUtil serialPortUtil;

    LOGI(TAG, "mainTest deviceName=%s", pcDeviceName);
    serialPortUtil.open(pcDeviceName);
    if(!serialPortUtil.isValued()) {
        LOGI(TAG, "isValued return false!");
        return;
    }
    serialPortUtil.setSpeed(9600);
    serialPortUtil.setParity(SerialPortUtil::DATA_BITS_8,
            SerialPortUtil::STOP_BITS_1, SerialPortUtil::PARITY_N);
    strcpy(sendBytes, "JNI send serial data");
    serialPortUtil.write(sendBytes, strlen(sendBytes));
    while (1)
    {
        while((nReadBytes = serialPortUtil.read(readBytes, sizeof(readBytes))) > 0)
        {
            LOGI(TAG, "\nreand len = %d\n", nReadBytes);
            readBytes[nReadBytes] = 0;
            LOGI(TAG, "read buff: %s", readBytes);
            //if(strcmp(readBytes, "OK") == 0) {
            if(strcmp(readBytes, "\x4f\x4B") == 0) {
                LOGI(TAG, "call serialPortUtil.close!");
                serialPortUtil.close();
                return;
            }
        }
    }
}
