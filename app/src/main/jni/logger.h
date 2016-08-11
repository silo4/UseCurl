//
// Created by zhonglz on 16/8/6.
//

#ifndef USECURL_LOGGER_H
#define USECURL_LOGGER_H

#include <android/log.h>

#define LOGI(TAG, ...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__);
#define LOGE(TAG, ...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__);
#define LOGW(TAG, ...) __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__);

#endif //USECURL_LOGGER_H
