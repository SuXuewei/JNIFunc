//
// Created by Administrator on 2021/1/28.
//

#ifndef NDKFUNC_SOCKETUTIL_H
#define NDKFUNC_SOCKETUTIL_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class SocketUtil {

public:
    SocketUtil();
    ~SocketUtil();
    //is the socket object valid
    bool isValid();
    //open a socket connect to host
    bool open(const char* pcIP, int port);
    //send data
    int send(unsigned char* pcSendBuf, int count);
    //read data
    int recv(char* pcReadBuf, int maxSize);
    //close socket
    void close();

    //for test
    static void testSelf(int argv, char **argc);

private:
    static const int CONNECT_MAX_COUNT = 10;
    static const char* const TAG;

    int m_socket;
    struct sockaddr_in m_addr;
};

#endif //NDKFUNC_SOCKETUTIL_H
