//
// Created by Administrator on 2021/1/20.
//

#ifndef NDKFUNC_SERIALPORT_H
#define NDKFUNC_SERIALPORT_H

#include <jni.h>

extern "C" {
    JNIEXPORT jstring JNICALL Java_com_suxw_ndkfunc_util_SerialPort_getStringFromCpp(
        JNIEnv *env, jobject obj, jstring input);

    JNIEXPORT void JNICALL Java_com_suxw_ndkfunc_util_SerialPort_testSerialPort(
        JNIEnv *env, jobject obj);
};

#endif //NDKFUNC_SERIALPORT_H
