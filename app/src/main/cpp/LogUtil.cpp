//
// Created by Administrator on 2021/1/28.
//

#include <string>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cerrno>
#include "LogUtil.h"
#include "StringUtil.h"
#include "BytesUtil.h"

const char* const LogUtil::TAG = "LogUtil";

FILE* LogUtil::openLogFile(struct tm* t)
{
    char fileName[64];
    char *pcRootDir = "/sdcard/NDKFuncLog";
    FILE* fp = NULL;

    //判定根目录是否存在
    if (access(pcRootDir, F_OK))
    {
        //目录不存在需要创建
        if(mkdir(pcRootDir, S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO))
        {
            LOGI(TAG, "fail to mkdir %s, error %s, errno %d",
                    pcRootDir, strerror(errno), errno);
            return NULL;
        }
    }

    //获取日志文件名
    memset(fileName, 0, sizeof(fileName));
    //sprintf(fileName, "/sdcard/jni_%04d%02d%02d.log",
    sprintf(fileName, "%s/jni_%04d%02d%02d.log", pcRootDir,
            1900 + t->tm_year, 1 + t->tm_mon, t->tm_mday);
    LOGI(TAG, "fileName %s", fileName);

    //
    fp = fopen(fileName, "at");
    if(NULL == fp) {
        LOGI(TAG, "fail to open file %s error %s errno %d",
             fileName, strerror(errno), errno);
        return NULL;
    }
    LOGI(TAG, "open file %s fp=%d error %s errno %d",
         fileName, fp, strerror(errno), errno);

    return fp;
}

void LogUtil::getTagLevel(int nTagLevel, char *tagLevel) {
    switch (nTagLevel) {
        case LOG_LEVAL_INFO:
            strcpy(tagLevel, "I");
            break;
        case LOG_LEVAL_DEBUG:
            strcpy(tagLevel, "D");
            break;
        case LOG_LEVAL_ERROR:
            strcpy(tagLevel, "E");
            break;
            break;
        default:
            strcpy(tagLevel, "UNKNOW");
            break;
    }
}

//
void LogUtil::log(int nLogLeval, char *pcTitle, unsigned char *pcData, int nDataLen)
{
    char tagLevel[8];
    char logInfo[2048];
    FILE *fp;
    time_t lTime;
    struct tm* t;
    int nSendLen = 0;

    memset(tagLevel, 0, sizeof(tagLevel));
    memset(logInfo, 0, sizeof(logInfo));

    //获取时间信息
    time(&lTime);
    t = localtime(&lTime);

    //获取日志文件名
    fp = openLogFile(t);
    if(fp == NULL) {
        return;
    }

    //生成日志内容并输出到日志中
    memset(logInfo, 0, sizeof(logInfo));
    memset(tagLevel, 0, sizeof(tagLevel));
    getTagLevel(nLogLeval, tagLevel);
    sprintf(logInfo, "[%04d-%02d-%02d %2.2d:%2.2d:%2.2d %s]: %s\r\n",
            1900 + t->tm_year, 1 + t->tm_mon, t->tm_mday,
            t->tm_hour, t->tm_min, t->tm_sec,
            tagLevel, pcTitle);
    nSendLen = strlen(logInfo);
    if(fwrite(logInfo, 1, strlen(logInfo), fp) != nSendLen) {
        LOGI(TAG, "write data fail by fputs, data:%s nSendLen:%d error:%s errno:%d",
             logInfo, nSendLen, strerror(errno), errno);
        fclose(fp);
        return;
    }
    LOGI(TAG, "fputs, data:%s error:%s errno:%d",
        logInfo, strerror(errno), errno);

    if((pcData != NULL) && (nDataLen > 0))
    {
        //转化数据为二进制字符串并输出到日志中
        memset(logInfo, -1, sizeof(logInfo));
        BytesUtil::convertByteToHexString((char *)pcData, nDataLen, logInfo, sizeof(logInfo));
        strcat(logInfo, "\r\n");
        //if(fputs(logInfo, fp) == EOF) {
        nSendLen = strlen(logInfo);
        if(fwrite(logInfo, 0, strlen(logInfo), fp) != nSendLen) {
            LOGI(TAG, "write data fail by fputs, data:%s nSendLen:%d error:%s errno:%d",
                 logInfo, nSendLen, strerror(errno), errno);
            fclose(fp);
            return;
        }
        LOGI(TAG, "fputs, data:%s error:%s errno:%d",
             logInfo, strerror(errno), errno);
    }

    fclose(fp);
    fp = NULL;
}

//
void LogUtil::logcat(int nLogLeval, char *pcTitle, const unsigned char *pcData, int nDataLen)
{
    char tagLevel[8];
    char logInfo[2048];
    time_t lTime;
    struct tm* t;
    int nSendLen = 0;

    memset(tagLevel, 0, sizeof(tagLevel));
    memset(logInfo, 0, sizeof(logInfo));

    //获取时间信息
    time(&lTime);
    t = localtime(&lTime);

    //生成日志内容并输出到日志中
    memset(logInfo, 0, sizeof(logInfo));
    memset(tagLevel, 0, sizeof(tagLevel));
    getTagLevel(nLogLeval, tagLevel);
    sprintf(logInfo, "[%04d-%02d-%02d %2.2d:%2.2d:%2.2d %s]: %s\r\n",
            1900 + t->tm_year, 1 + t->tm_mon, t->tm_mday,
            t->tm_hour, t->tm_min, t->tm_sec,
            tagLevel, pcTitle);
    nSendLen = strlen(logInfo);
    androidLogcat(nLogLeval, TAG, logInfo);

    //转化数据为二进制字符串并输出到日志中
    memset(logInfo, 0, sizeof(logInfo));
    BytesUtil::convertByteToHexString((char *)pcData, nDataLen, logInfo, sizeof(logInfo));
    strcat(logInfo, "\r\n");
    //if(fputs(logInfo, fp) == EOF) {
    nSendLen = strlen(logInfo);
    androidLogcat(nLogLeval, TAG, logInfo);
}

//
void LogUtil::androidLogcat(int nLogLeval, const char* const tag, char* logInfo)
{
    switch (nLogLeval) {
        case LOG_LEVAL_INFO:
            LOGI(tag, "%s", logInfo);
            break;
        case LOG_LEVAL_DEBUG:
            LOGD(tag, "%s", logInfo);
            break;
        case LOG_LEVAL_ERROR:
            LOGE(tag, "%s", logInfo);
            break;
        default:
            LOGI(tag, "%s", logInfo);
            break;
    }
}
