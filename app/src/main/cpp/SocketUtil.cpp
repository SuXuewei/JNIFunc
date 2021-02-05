//
// Created by Administrator on 2021/1/28.
//

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <cerrno>
#include "SocketUtil.h"
#include "LogUtil.h"
#include "TimerUtil.h"
#include "ErrorCode.h"

const char* const SocketUtil::TAG = "SocketUtil";

SocketUtil::SocketUtil()
{
    m_socket = -1;
    memset((char *)&m_addr, 0, sizeof(m_addr));
}

SocketUtil::~SocketUtil()
{
    close();
}

inline bool SocketUtil::isValid() const {
    return m_socket > 0;
}

//create a socket connect to host
bool SocketUtil::open(const char *pcIP, int port)
{
    close();

    memset((char *)&m_addr, 0, sizeof(m_addr));
    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons(port);
    m_addr.sin_addr.s_addr = inet_addr(pcIP);
    bzero(&(m_addr.sin_zero), 8);

    //创建套接字
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(m_socket < 0) {
        LOGI(TAG, "create socket error: %s errno:%d",
                strerror(errno), errno);
        return false;
    }

    //连接服务器
    int nTryCount = 0;
    while(connect(m_socket, (struct sockaddr*)&m_addr, sizeof(m_addr)) < 0)
    {
        if(EINTR == errno)
        {
            if(nTryCount < SocketUtil::CONNECT_MAX_COUNT)
            {
                nTryCount++;
                usleep(100000);
                continue;
            }
        }

        LOGI(TAG, "connect error: %s errno:%d",
             strerror(errno), errno);
        close();
        return false;
    }

    LOGI(TAG, "socket open success!");
    return true;
}

int SocketUtil::send(unsigned char *pcSendBuf, int count)
{
    int bytesCount = 0;
    if ((bytesCount = ::write(m_socket, pcSendBuf, count)) < 0) {
        LOGI(TAG, "send error: %s errno:%d",
             strerror(errno), errno);
        return 0;
    }

    return bytesCount;
}

//以秒为单位
int SocketUtil::recv(char *pcReadBuf, int nNeedReadLen, int nTimeOut)
{
    TimerUtil timerUtil;
    int nReadLen = 0;
    int nReceivedLen = 0;
    int nStatus = EC_ERROR;

    nStatus = checkStatus(nTimeOut);
    if (nStatus != EC_SUCCESS) {
        return nStatus;
    }

    // checkStatus确定有数据可以读取，此处计算读取数据的总时长不能超过timeOut
    timerUtil.startTimer();
    while(true) {
        //此处从串口处读取指定个数的字节数据,即使接收的数据缓存比这个大
        //与从文件中读取指定个数的字节数据是一样的,即使文件本身有大量数据
        nReadLen = ::read(m_socket, pcReadBuf + nReceivedLen,
                nNeedReadLen - nReceivedLen);
        nReceivedLen += nReadLen;

        if(nReadLen < 0) {
            LOGI(TAG, "read error: %s errno:%d",
                 strerror(errno), errno);
            return 0;
        }
        if(nReceivedLen >= nNeedReadLen) {
            break;
        }
        if((nTimeOut != TIME_OUT_DISABLE) && (timerUtil.getTimeOut() > nTimeOut)) {
            return EC_TIMEOUT;
        }
    }

    return nReceivedLen;
}

void SocketUtil::close() {
    if (isValid())
    {
        shutdown(m_socket, SHUT_RDWR);
        ::close(m_socket);
        m_socket = -1;
    }
}

//监测超时时间以秒为单位
int SocketUtil::checkStatus(int timeOut) {
    int iRet = -1;
    fd_set fds; //文件描述符结合
    struct timeval tvWaitTime;

    if (!isValid()) {
        return EC_EXCEPTION;
    }

    //清空描述符
    FD_ZERO(&fds);
    //添加文件句柄，设置标志位
    FD_SET(m_socket, &fds);
    //设置超时时间
    tvWaitTime.tv_sec = timeOut;
    tvWaitTime.tv_usec = 0;

    //开始监测socket读取状态
    if (timeOut == TIME_OUT_DISABLE) {
        iRet = select(m_socket + 1, &fds, NULL, NULL, NULL);
    } else {
        iRet = select(m_socket + 1, &fds, NULL, NULL, &tvWaitTime);
    }
    if(iRet == -1) {
        return EC_EXCEPTION;
    }
    if(iRet == 0) {
        return EC_TIMEOUT;
    }

    return EC_SUCCESS;
}

void SocketUtil::testSelf(int argv, char **argc)
{
    char recvBuff[1024];
    char* pcSendInfo = "The information from client!";
    SocketUtil socketUtil;
    int nTryCount = 0;
    int nRecvLen = 0;

    LOGI(TAG, "testSelf");
    memset(recvBuff, 0, sizeof(recvBuff));
    socketUtil.open("10.73.4.56", 1988);
    if (!socketUtil.isValid()) {
        LOGI(TAG, "testSelf socket invalid!");
        return;
    }
    socketUtil.send((unsigned char*)pcSendInfo, strlen(pcSendInfo));
    LOGI(TAG, "send data: %s", pcSendInfo);
    nTryCount = 0;
    while((nRecvLen = socketUtil.recv(recvBuff, 10, 10)) <= 0) {
        if (nRecvLen == EC_TIMEOUT) {
            LOGI(TAG, "socket recv time out!");
        }
        if(nTryCount < 15) {
            continue;
        }
        break;
    }
    LogUtil::log(LogUtil::LOG_LEVAL_INFO, "socket read data",
            (unsigned char*)recvBuff, nRecvLen);
    LOGI(TAG, "read data: %s", recvBuff);
    socketUtil.close();
}
