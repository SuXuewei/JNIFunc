//
// Created by Administrator on 2021/1/27.
//
#include <cstring>
#include "StringUtil.h"

const char* const StringUtil::TAG = "StringUtil";
const char* const StringUtil::ENCODE_GB2312 = "GB2312";
const char* const StringUtil::ENCODE_UTF8 = "utf-8";

// 静态类程序员需要在类体外进行初始化，否则编译提示未定义
// 可以理解为初始化的同事也是在实例话成员变量
JNIEnv* StringUtil::m_pJNIEnv = NULL;
JavaVM* StringUtil::m_pJavaVM = NULL;
pthread_t StringUtil::m_jvmThread = NULL;
jclass StringUtil::m_stringClass = NULL;
jmethodID StringUtil::m_midStringInit = NULL;
jmethodID StringUtil::m_midGetBytes = NULL;

void StringUtil::init(JNIEnv *env) {
    (*env).GetJavaVM(&m_pJavaVM);
    m_jvmThread = pthread_self();   //记住当前JNI环境线程
    StringUtil::m_pJNIEnv = env;

    //先存上相关的类和方法id，方便后续调用
    m_stringClass = (*env).FindClass("java/lang/String");
    m_midGetBytes = (*env).GetMethodID(m_stringClass, "getBytes",
            "(Ljava/lang/String;)[B");
    m_midStringInit = (*env).GetMethodID(m_stringClass, "<init>",
                                       "([BLjava/lang/String;)V");
}

//将jstring转换成指定格式的char数组
bool StringUtil::convertJStringToBytes(jstring jstr,
        const char* const pcEncode, char *pcStr, int *pnCStrLen)
{
    bool bRet = false;
    //生成调用getBytes的入参字符串，标示采用的编码，需要使用jni使用的UTF格式字符串
    jstring strEncode = (*StringUtil::m_pJNIEnv).NewStringUTF(pcEncode);
    // 调用类对象方法，过程类似c函数的调用,第一个是jstring字符串对象
    // 调用的String类方法getBytes方法ID
    // 调用getBytes方法时的入参strencode(字符串采用jni统一的utf编码的字符串)
    // 返回jbyteArray就是对应编码方式byteArray对象
    jbyteArray byteArray = (jbyteArray)(*StringUtil::m_pJNIEnv).CallObjectMethod(jstr, m_midGetBytes, strEncode); // String.getByte("GB2312");
    // 获取对应编码数据的子串数组长度
    jsize byteLen = (*StringUtil::m_pJNIEnv).GetArrayLength(byteArray);
    // 获取jbyteArray数组对象内部jbyte数组的地址, 这个应该是内部申请了动态空间并作为结果返回的
    jbyte* pcGBK =   (*StringUtil::m_pJNIEnv).GetByteArrayElements(byteArray, JNI_FALSE);
    // 如果字符串不为空，且数据长度没有超过接收数据缓存的上限
    if((byteLen > 0) && (byteLen < *pnCStrLen))
    {
        memcpy(pcStr, pcGBK, byteLen);
        pcStr[byteLen] = 0;
        *pnCStrLen = byteLen;
        bRet = true;
    }
    //次数对于GetByteArrayElements函数申请的动态空间结果进行资源释放
    (*StringUtil::m_pJNIEnv).ReleaseByteArrayElements(byteArray, pcGBK,0);  //
    return bRet;
}

//由指定编码以零结束的char转为Java String
jstring StringUtil::ConvBytesToJstring(const char* pcStr, const char* encoding)
{
    jstring jencoding;
    jbyteArray bytes = (*m_pJNIEnv).NewByteArray(strlen(pcStr));
    (*m_pJNIEnv).SetByteArrayRegion(bytes, 0, strlen(pcStr), (jbyte*)pcStr);
    if (encoding == NULL)
        jencoding= (*m_pJNIEnv).NewStringUTF(ENCODE_UTF8);
    else
        jencoding=(*m_pJNIEnv).NewStringUTF(encoding);

    return (jstring)(*m_pJNIEnv).NewObject(m_stringClass, m_midStringInit,
            bytes, jencoding);
}
