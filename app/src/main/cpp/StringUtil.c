//
// Created by Administrator on 2021/1/19.
//

#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <jni.h>

#include "android/log.h"

#include "StringUtil.h"

JNIEXPORT jstring JNICALL Java_com_suxw_ndkfunc_util_StringUtil_getOutString(
    JNIEnv *env, jclass this, jstring inputString)
{
    char str[516];

    memset(str, 0, sizeof(str));
    strcpy(str, "String from jni function ");
    const char *pcStrBuf = (*env)->GetStringUTFChars(env, inputString, JNI_FALSE);
    strcat(str, pcStrBuf);
    (*env)->ReleaseStringUTFChars(env, inputString, pcStrBuf);

    return (*env)->NewStringUTF(env, str);
}
