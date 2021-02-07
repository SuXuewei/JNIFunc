//
// Created by Administrator on 2021/2/7.
//

#include "Viewer.h"
#include "BytesUtil.h"

JNIEnv *Viewer::m_env = nullptr;
jobject Viewer::m_obj = nullptr;
jmethodID Viewer::m_midDisplay = nullptr;

void Viewer::init(JNIEnv *env, jobject obj, const char* pcFuncName, const char* sig) {
    m_env = env;
    m_obj = obj;

    jclass clazz = (*env).GetObjectClass(obj);
    m_midDisplay = (*env).GetMethodID(clazz, pcFuncName, sig);
}

void Viewer::display(const char* pcTitle, const unsigned char* pcData, int nDataLen) {
    char tmp[1024];

    jstring jstrInfo;
    jstrInfo = StringUtil::ConvBytesToJstring(pcTitle, StringUtil::ENCODE_GB2312);
    (*m_env).CallVoidMethod(m_obj, m_midDisplay, jstrInfo);

    memset(tmp, 0, sizeof(tmp));
    BytesUtil::convertByteToHexString((const char*)pcData, nDataLen, tmp, sizeof(tmp));
    jstrInfo = StringUtil::ConvBytesToJstring(tmp, StringUtil::ENCODE_GB2312);
    (*m_env).CallVoidMethod(m_obj, m_midDisplay, jstrInfo);
}

void Viewer::display(char* pcInfo) {
    jstring jstrInfo;
    jstrInfo = StringUtil::ConvBytesToJstring(pcInfo, StringUtil::ENCODE_GB2312);
    (*m_env).CallVoidMethod(m_obj, m_midDisplay, jstrInfo);
}

bool Viewer::isVlalid(char *pcInfo) {
    return m_midDisplay != nullptr;
}
