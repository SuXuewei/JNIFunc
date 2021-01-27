//
// Created by Administrator on 2021/1/20.
//

#ifndef NDKFUNC_STRINGTOOLS_H
#define NDKFUNC_STRINGTOOLS_H

#include <jni.h>

JNIEXPORT jobject JNICALL Java_com_suxw_ndkfunc_util_StringUtil_getOutString(
    JNIEnv *env, jclass this, jstring inputString);

#endif //NDKFUNC_STRINGTOOLS_H
