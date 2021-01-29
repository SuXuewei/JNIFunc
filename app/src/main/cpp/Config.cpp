//
// Created by Administrator on 2021/1/29.
//

#include <cerrno>
#include "Config.h"
#include "LogUtil.h"

const char* const Config::TAG = "Config";
const char* const Config::FILE_PATH = "/sdcard/NDKFuncLog/ndk_func.cfg";

Config::Config() {
    fp = NULL;
}

Config::~Config() {
    close();
}

bool Config::open(const char *pcFileName) {
    fp = fopen(Config::FILE_PATH, "r+");
    if (NULL == fp) {
        LOGI(TAG, "fail to open file %s error %s errno %d",
             Config::FILE_PATH, strerror(errno), errno);
        return false;
    }

    return true;
}

void Config::close() {
    if(isValid()) {
        fclose(fp);
        fp = NULL;
    }
}

//根据group和item名称获取对应的数值
bool Config::getItem(const char *pcGroupName, const char *pcItemName,
        char *pcItem, int nMaxSize)
{
    char tagBuff[256];
    int nTagLen = 0;
    bool bFindTag = false;

    if (!isValid()) {
        return false;
    }
    //rewind(fp);

    nTagLen = sizeof(tagBuff);
    //找到GROUP元素对应的位置
    memset(tagBuff, 0, sizeof(tagBuff));
    while(fgets(tagBuff, nTagLen, fp) != NULL)
    {
        LOGI(TAG, "getItem group len=%d, tagBuff=%s", strlen(tagBuff), tagBuff);
        if(strcmp(pcGroupName, tagBuff) == 0)
        {
            bFindTag = true;
            break;
        }
    }
    if (!bFindTag)
    {
        return false;
    }

    bFindTag = false;
    //找到ITEM位置
    memset(tagBuff, 0, sizeof(tagBuff));
    while(fgets(tagBuff, nTagLen, fp) != NULL)
    {
        LOGI(TAG, "getItem group len=%d, tagBuff=%s", strlen(tagBuff), tagBuff);
        if((strcmp(pcItemName, tagBuff) == 0) &&
            (strlen(tagBuff) - strlen(pcItemName)) < nMaxSize)
        {
            strcpy(pcItem, tagBuff + strlen(pcItemName));
            bFindTag = true;
            break;
        }
    }
    if (!bFindTag)
    {
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
    //if (!isValid()) {
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
        m_grp1_item1 = item;
    }

    pcItemName = "ITEM_1";
    memset(item, 0, nItemLen);
    if (getItem(pcGroupName, pcItemName, item, nItemLen))
    {
        LOGI(TAG, "Group:%s Item:%s Value:%s",
             pcGroupName, pcItemName, item);
        m_grp1_item2 = item;
    }

    //
    pcGroupName = "GROUP_1";
    pcItemName = "ITEM_0";
    memset(item, 0, nItemLen);
    if (getItem(pcGroupName, pcItemName, item, nItemLen))
    {
        LOGI(TAG, "Group:%s Item:%s Value:%s",
             pcGroupName, pcItemName, item);
        m_grp2_item1 = item;
    }

    pcItemName = "ITEM_1";
    memset(item, 0, nItemLen);
    if (getItem(pcGroupName, pcItemName, item, nItemLen))
    {
        LOGI(TAG, "Group:%s Item:%s Value:%s",
                pcGroupName, pcItemName, item);
        m_grp2_item2 = item;
    }

    close();
    return true;
}
