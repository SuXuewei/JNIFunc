//
// Created by Administrator on 2021/2/5.
//

#include "TimerUtil.h"

TimerUtil::TimerUtil() {
    m_tvSta.tv_sec = 0;
    m_tvSta.tv_usec = 0;
}

TimerUtil::~TimerUtil() {
    m_tvSta.tv_sec = 0;
    m_tvSta.tv_usec = 0;
}

void TimerUtil::startTimer() {
    struct timezone tzEnd;
    gettimeofday(&m_tvSta, &tzEnd);
}

int TimerUtil::getTimeOut() {
    struct timeval tvEnd;
    struct timezone tzEnd;
    int usec = 0;

    gettimeofday(&m_tvSta, &tzEnd);
    usec = USEC_PER_SECOND * (tvEnd.tv_sec - m_tvSta.tv_sec) + tvEnd.tv_usec - m_tvSta.tv_usec;
    return (usec / USEC_PER_SECOND);
}
