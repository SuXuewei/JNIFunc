//
// Created by Administrator on 2021/1/29.
//

#include <cerrno>
#include <cstring>
#include "Config.h"
#include "StringUtil.h"
#include "LogUtil.h"
#include "BytesUtil.h"

const char* const Config::TAG = "Config";
const char* const Config::FILE_PATH = "/sdcard/NDKFuncLog/ndk_func.cfg";

Config::Config() {
    m_fp = NULL;
}

Config::~Config() {
    close();
}

bool Config::open(const char *pcFileName) {
    m_fp = fopen(Config::FILE_PATH, "r+");
    if (NULL == m_fp) {
        LOGI(TAG, "fail to open file %s error %s errno %d",
             Config::FILE_PATH, strerror(errno), errno);
        return false;
    }
    LOGI(TAG, "success to open file %s error %s errno %d",
         Config::FILE_PATH, strerror(errno), errno);

    return true;
}

void Config::close() {
    if(isValid()) {
        fclose(m_fp);
        m_fp = NULL;
    }
}

bool Config::getTag(const char* pcLineData, const char* space, char *pcTag, int nMaxLen)
{
    const char* tag = NULL;
    tag = strstr(pcLineData,space);
    if(tag == NULL) {
        LOGI(TAG, "getTag: can not find [%s] in [%s]", space, pcLineData);
        LogUtil::logcat(LogUtil::LOG_LEVAL_INFO, "line", (const unsigned char*)pcLineData, strlen(pcLineData));
        LogUtil::logcat(LogUtil::LOG_LEVAL_INFO, "space", (const unsigned char*)space, strlen(space));
        return false;
    }

    int len = tag - pcLineData;
    if(len >= nMaxLen) {
        LOGI(TAG, "getTag: len >= nMaxLen [%d] [%d]", len, nMaxLen);
        return false;
    }

    memcpy(pcTag, pcLineData, len);
    pcTag[len] = 0;

    return true;
}

//根据group和item名称获取对应的数值
bool Config::getItem(const char *pcGroupName, const char *pcItemName,
        char *pcItem, int nMaxSize)
{
    char line[256];
    char tag[32];
    int nLen = 0;
    bool bFindTag = false;

    if (!isValid()) {
        return false;
    }
    fseek(m_fp, 0, SEEK_SET);

    //找到GROUP元素对应的位置
    memset(line, 0, sizeof(line));
    while(fgets(line, sizeof(line), m_fp) != NULL)
    {
        BytesUtil::removeLastStr(line, "\x0d\x0a");
        LogUtil::logcat(LogUtil::LOG_LEVAL_INFO, "line",
                        (unsigned char*)line, strlen(line));
        LOGI(TAG, "getItem group len=%d, data=%s", strlen(line), line);
        if(strcmp(pcGroupName, line) == 0)
        {
            bFindTag = true;
            break;
        }
        LOGI(TAG, "getItem %s != %s", pcGroupName, line);
    }
    if (!bFindTag) {
        return false;
    }

    bFindTag = false;
    //找到ITEM位置
    memset(line, 0, sizeof(line));
    while(fgets(line, sizeof(line), m_fp) != NULL)
    {
        BytesUtil::removeLastStr(line, "\x0d\x0a");
        LOGI(TAG, "getItem group len=%d, data=%s", strlen(line), line);
        memset(tag, 0, sizeof(tag));
        if(!getTag(line, "=", tag, sizeof(tag))) {
            LOGI(TAG, "can`t find item data in [%s] by [=]", line);
            return false;
        }

        nLen = strlen(line) - strlen(pcItemName);
        if((strcmp(pcItemName, tag) == 0) &&
            (nLen) < nMaxSize)
        {
            strcpy(pcItem, line + strlen(pcItemName) + 1);
            bFindTag = true;
            break;
        }
        LOGI(TAG, "getItem %s != %s", pcItemName, line);
    }
    if (!bFindTag) {
        return false;
    }

    return true;
}

bool Config::parseConfigFile(const char *pcConfigFilePath)
{
    char item[128];
    int nItemLen = 0;
    char *pcGroupName = NULL;
    char *pcItemName = NULL;

    LOGI(TAG, "parseConfigFile");
    //判定文件句柄是否可用
    if (!open(pcConfigFilePath)) {
        return false;
    }

    nItemLen = sizeof(item);
    //
    pcGroupName = "GROUP_0";
    pcItemName = "ITEM_0";
    memset(item, 0, nItemLen);
    LOGI(TAG, "getItem, %s, %s", pcGroupName, pcItemName);
    if (getItem(pcGroupName, pcItemName, item, nItemLen))
    {
        LOGI(TAG, "Group:%s Item:%s Value:%s",
             pcGroupName, pcItemName, item);
        //减配置文件中读取的数据写入到日志文件中
        LogUtil::log(LogUtil::LOG_LEVAL_INFO, item,
                (unsigned char*)item, strlen(item));
        // 将程序中的汉子写入文件中,这么直接写采用开发环境默认的编码UTF-8
        // 如果生成文件对编码有要求，需要主动转换编码方式后再写入文件
        memset(item, 0, sizeof(item));
        strcpy(item, "测试");
        LogUtil::log(LogUtil::LOG_LEVAL_INFO, item,
                     NULL, 0);
        StringUtil::ConvertUTF8ToGBK(item, item, sizeof(item));
        //写转换后台的数据
        LogUtil::log(LogUtil::LOG_LEVAL_INFO, item,
                     NULL, 0);
        //m_grp1_item1 = item;
    }

    pcItemName = "ITEM_1";
    memset(item, 0, nItemLen);
    LOGI(TAG, "getItem, %s, %s", pcGroupName, pcItemName);
    if (getItem(pcGroupName, pcItemName, item, nItemLen))
    {
        LOGI(TAG, "Group:%s Item:%s Value:%s",
             pcGroupName, pcItemName, item);
        //m_grp1_item2 = item;
    }

    //
    pcGroupName = "GROUP_1";
    pcItemName = "ITEM_0";
    memset(item, 0, nItemLen);
    LOGI(TAG, "getItem, %s, %s", pcGroupName, pcItemName);
    if (getItem(pcGroupName, pcItemName, item, nItemLen))
    {
        LOGI(TAG, "Group:%s Item:%s Value:%s",
             pcGroupName, pcItemName, item);
        //m_grp2_item1 = item;
    }

    pcItemName = "ITEM_1";
    memset(item, 0, nItemLen);
    LOGI(TAG, "getItem, %s, %s", pcGroupName, pcItemName);
    if (getItem(pcGroupName, pcItemName, item, nItemLen))
    {
        LOGI(TAG, "Group:%s Item:%s Value:%s",
                pcGroupName, pcItemName, item);
        //m_grp2_item2 = item;
    }

    close();
    return true;
}
