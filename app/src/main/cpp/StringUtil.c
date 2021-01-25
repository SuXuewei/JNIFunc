//
// Created by Administrator on 2021/1/19.
//

#include <termios.h>
#include <stdlib.h>
#include <iconv.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <jni.h>

#include "android/log.h"

#include "StringUtil.h"

//minSDK 28
//int GBK_to_UTF8(char *pcIn, int nInLen, char *pcOut, int nOutLen)
//{
//    iconv_t cd;
//    int rc;
//    char **pIn;
//    char **pOut;
//
//    cd = iconv_open("UTF-8", "GBK");
//    if(0 == cd) {
//        return -1;
//    }
//
//    if(-1 == iconv(cd, pIn, &nInLen, pOut, &nOutLen))
//    {
//        iconv_close(cd);
//        return -1;
//    }
//
//    iconv_close(cd);
//    return 0;
//}

JNIEXPORT jstring JNICALL Java_com_suxw_ndkfunc_util_StringUtil_getOutString(
    JNIEnv *env, jclass this, jstring inputString)
{
    char str[516];
    char utf8[256];
    char *pcGBK = "\xCB\xD5\xD1\xA9\xCE\xB0";

    memset(str, 0, sizeof(str));
    memset(utf8, 0, sizeof(utf8));
    strcpy(str, "JNI Func String ");
//    strcat(str, pcGBK);    //GBK 汉字
//    GBK_to_UTF8(pcGBK, strlen(pcGBK), utf8, sizeof(utf8));
//    strcpy(str, "JNI Func GBK to UTF8 String ");
//    strcat(str, utf8);    //GBK 汉字
    const char *pcStrBuf = (*env)->GetStringUTFChars(env, inputString, JNI_FALSE);
    strcat(str, pcStrBuf);
    (*env)->ReleaseStringUTFChars(env, inputString, pcStrBuf);

    return (*env)->NewStringUTF(env, str);
}

char*   Jstring2CStr(JNIEnv*   env,   jstring   jstr)
{
    char*   rtn   =   NULL;
    jclass   clsstring   =   (*env)->FindClass(env,"java/lang/String");
    jstring   strencode   =   (*env)->NewStringUTF(env,"GB2312");
    jmethodID   mid   =   (*env)->GetMethodID(env,clsstring,   "getBytes",   "(Ljava/lang/String;)[B");
    jbyteArray   barr=   (jbyteArray)(*env)->CallObjectMethod(env,jstr,mid,strencode); // String .getByte("GB2312");
    jsize   alen   =   (*env)->GetArrayLength(env,barr);
    jbyte*   ba   =   (*env)->GetByteArrayElements(env,barr,JNI_FALSE);
    if(alen   >   0)
    {
        rtn   =   (char*)malloc(alen+1);         //"\0"
        memcpy(rtn,ba,alen);
        rtn[alen]=0;
    }
    (*env)->ReleaseByteArrayElements(env,barr,ba,0);  //
    return rtn;
}