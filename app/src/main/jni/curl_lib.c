//
// Created by zhonglz on 16/8/6.
//

#include <stddef.h>
#include <malloc.h>
#include "curl_ssl_CurlLib.h"
#include "logger.h"
#include "curl_mgr.h"
#include "util.h"

#define CURLLIB "curl_lib"

const char* cert_path;

JNIEXPORT jint JNICALL Java_curl_ssl_CurlLib_init
        (JNIEnv * env, jobject thiz, jstring cert)
{
    cert_path = (*env)->GetStringUTFChars(env, cert, NULL);
    LOGI(CURLLIB, "cert path [%s]", cert_path);

    init_curl();
}

JNIEXPORT jbyteArray JNICALL Java_curl_ssl_CurlLib_testUrl
  (JNIEnv * env, jobject thiz, jstring url)
{
    const char* p_url = (*env)->GetStringUTFChars(env, url, NULL);
    if (p_url == NULL)
    {
        LOGE(CURLLIB, "url is null")

        return NULL;
    }else
    {
        LOGI(CURLLIB, "test url [%s]", p_url);
    }

    CURL_DOWNLOAD_OBJECT* download_object = (CURL_DOWNLOAD_OBJECT*)malloc(sizeof(CURL_DOWNLOAD_OBJECT));
    download_object->data = NULL;
    download_object->size = 0;

    if (get(p_url, download_object, cert_path))
    {
        jbyteArray ret = char2jbyteArray(env, download_object->data, (unsigned int)download_object->size);
        free(download_object->data);
        free(download_object);

        return ret;
    }

    (*env)->ReleaseStringUTFChars(env, url, p_url);

    return NULL;
}

