//
// Created by Administrator on 2021/2/6.
//

#ifndef NDKFUNC_CONTROLLER_H
#define NDKFUNC_CONTROLLER_H


#include <jni.h>

extern "C" {
JNIEXPORT void JNICALL Java_com_suxw_ndkfunc_util_SerialPortActivity_doSomething(
        JNIEnv *env, jobject obj);
};

#endif //NDKFUNC_CONTROLLER_H
