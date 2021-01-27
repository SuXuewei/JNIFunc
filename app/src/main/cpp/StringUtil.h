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
    static void init(JNIEnv *env);

    static bool convertJStringToBytes(jstring jstr,
            const char* const pcEncode, char *pcStr, int *pnCStrLen);

    static bool convertJStringToGBK(JNIEnv *env, jstring jstr,
            char *pcStr, int *pnCStrLen);

    static jstring strToJstring(JNIEnv* env, const char* pStr);

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
