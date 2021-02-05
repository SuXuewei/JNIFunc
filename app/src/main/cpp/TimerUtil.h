//
// Created by Administrator on 2021/2/5.
//

#ifndef NDKFUNC_TIMERUTIL_H
#define NDKFUNC_TIMERUTIL_H

#include <ctime>

class TimerUtil {

public:
    TimerUtil();
    ~TimerUtil();

    //开始计时
    void startTimer();
    //计算距最近一次调用startTimer时经过的时间，以秒为单位
    int getTimeOut();

private:
    static const int USEC_PER_SECOND = 1000000;   //每秒包含的毫秒数

    struct timeval m_tvSta;
};

#endif //NDKFUNC_TIMERUTIL_H
