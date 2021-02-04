//
// Created by Administrator on 2021/1/27.
//

#ifndef NDKFUNC_STRINGUTIL_H
#define NDKFUNC_STRINGUTIL_H

#include <jni.h>
#include <string>
#include <pthread.h>

class StringUtil {

public:
    //调用init后，才可使用StringUtil提供的相关功能函数
    static void init(JNIEnv *env);

    //将jstring转换成指定编码的char数组
    static bool convertJStringToBytes(jstring jstr,
            const char* const pcEncode, char *pcStr, int *pnCStrLen);

    //使用指定编码的char数组，生成jni的string对象
    static jstring ConvBytesToJstring(const char* pcStr, const char* encoding);

    //GBK to UTF-8
    static bool ConvertGBKToUTF8(const char* pcGBK, char* pcUTF8, int nMaxLen);

    //UTF-8 to GBK
    static bool ConvertUTF8ToGBK(const char* pcUTF8, char* pcGBK, int nMaxLen);

public:
    static const char* const ENCODE_GB2312;
    static const char* const ENCODE_UTF8;

private:
    static const char* const TAG;

    static JNIEnv* m_pJNIEnv;
    static JavaVM* m_pJavaVM;
    static pthread_t m_jvmThread;

    static jclass m_stringClass;
    static jmethodID m_midStringInit;
    static jmethodID m_midGetBytes;
};

#endif //NDKFUNC_STRINGUTIL_H
