/*
 * loveLog.h
 *
 *  Created on: Nov 25, 2011
 *      Author: hagish
 */

#ifndef LOVELOG_H_
#define LOVELOG_H_

#include <android/log.h>

#define  LOG_TAG    "liblove"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#endif /* LOVELOG_H_ */
