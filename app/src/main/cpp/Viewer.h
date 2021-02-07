//
// Created by Administrator on 2021/2/7.
//

#ifndef NDKFUNC_VIEWER_H
#define NDKFUNC_VIEWER_H

#include <jni.h>
#include "StringUtil.h"

class Viewer {

public:
    static void init(JNIEnv *env, jobject obj, const char* pcFuncName, const char* sig);

    static void display(char* pcInfo);
    static void display(const char* pcTitle, const unsigned char* pcData, int nDataLen);

    static bool isVlalid(char* pcInfo);

private:
    static JNIEnv *m_env;
    static jmethodID m_midDisplay;
    static jobject m_obj;
};

#endif //NDKFUNC_VIEWER_H
