//
// Created by Administrator on 2021/1/26.
//

#ifndef NDKFUNC_LOGUTIL_H
#define NDKFUNC_LOGUTIL_H

#include "android/log.h"

#define LOGI(TAG, fmt, args...) __android_log_print(ANDROID_LOG_INFO,  TAG, fmt, ##args)
#define LOGD(TAG, fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, TAG, fmt, ##args)
#define LOGE(TAG, fmt, args...) __android_log_print(ANDROID_LOG_ERROR, TAG, fmt, ##args)

#endif //NDKFUNC_LOGUTIL_H
