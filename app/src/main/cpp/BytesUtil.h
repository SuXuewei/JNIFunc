//
// Created by Administrator on 2021/2/2.
//

#ifndef NDKFUNC_BYTESUTIL_H
#define NDKFUNC_BYTESUTIL_H


class BytesUtil {

public:
    //
    static bool convertByteToHexString(const char* pcBytes, int nCount,
                                       char* pcHex, int nMaxLen);

    //去除字符串末尾的pcSubStr子串
    static void removeLastStr(char *pcStr, const char* pcSubStr);
private:
    static const char* const TAG;
};


#endif //NDKFUNC_BYTESUTIL_H
