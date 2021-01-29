//
// Created by Administrator on 2021/1/26.
//

#ifndef NDKFUNC_LOGUTIL_H
#define NDKFUNC_LOGUTIL_H

#include <string>
#include <ctime>
#include <iostream>
#include "android/log.h"

#define LOGI(TAG, fmt, args...) __android_log_print(ANDROID_LOG_INFO,  TAG, fmt, ##args)
#define LOGD(TAG, fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG, fmt, ##args)
#define LOGE(TAG, fmt, args...) __android_log_print(ANDROID_LOG_ERROR, TAG, fmt, ##args)

class LogUtil
{
public:
    static void log(int nLogLeval, char* pcTitle,  unsigned char *pcData, int nDataLen);

private:
    static FILE* openLogFile(char* pcFileName, struct tm* t);
    static void getTagLevel(int nTagLevel, char* tagLevel);

public:
    static const int LOG_LEVAL_INFO = 0;
    static const int LOG_LEVAL_DEBUG = 1;
    static const int LOG_LEVAL_ERROR = 2;

private:
    static const char* const TAG;
};

#endif //NDKFUNC_LOGUTIL_H
