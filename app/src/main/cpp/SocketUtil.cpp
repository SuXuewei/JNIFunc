//
// Created by Administrator on 2021/1/28.
//

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <errno.h>
#include "SocketUtil.h"
#include "LogUtil.h"

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

int SocketUtil::recv(char *pcReadBuf, int maxSize)
{
    int bytesCount = 0;
    if((bytesCount = ::read(m_socket, pcReadBuf, maxSize)) < 0) {
        LOGI(TAG, "read error: %s errno:%d",
             strerror(errno), errno);
        return 0;
    }

    return bytesCount;
}

void SocketUtil::close() {
    if (isValid())
    {
        shutdown(m_socket, SHUT_RDWR);
        ::close(m_socket);
        m_socket = -1;
    }
}

bool SocketUtil::isValid() {
    return (m_socket > 0);
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
    socketUtil.open("10.73.4.130", 1988);
    if (!socketUtil.isValid()) {
        LOGI(TAG, "testSelf socket invalid!");
        return;
    }
    socketUtil.send((unsigned char*)pcSendInfo, strlen(pcSendInfo));
    LOGI(TAG, "send data: %s", pcSendInfo);
    nTryCount = 0;
    while((nRecvLen = socketUtil.recv(recvBuff, sizeof(recvBuff))) <= 0) {
        sleep(1);
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
