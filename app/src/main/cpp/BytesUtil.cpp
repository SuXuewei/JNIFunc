//
// Created by Administrator on 2021/2/2.
//

#include <cstring>
#include <cstdlib>
#include <clocale>
#include "BytesUtil.h"
#include "LogUtil.h"

const char* const BytesUtil::TAG = "BytesUtil";

bool BytesUtil::convertByteToHexString(const char *pcBytes, int nCount, char *pcHex, int nMaxLen)
{
    if((nCount * 2) >= nMaxLen) {
        LOGI(TAG, "convertByteToHexString data length error! nCount=%d nMaxLen=%d", nCount, nMaxLen);
        return false;
    }

    int nConvertedLen = 0;
    while(nConvertedLen < nCount) {
        sprintf(pcHex + nConvertedLen * 2, "%02x", pcBytes[nConvertedLen++]);
    }
    pcHex[nCount * 2] = 0;

    return true;
}

void BytesUtil::removeLastStr(char *pcStr, const char *pcSubStr) {
    char* pcTmp = NULL;
    if((pcTmp = strstr(pcStr, pcSubStr)))
    {
        *pcTmp = '\0';
    }
}