//
// Created by zhonglz on 16/8/6.
// 关于curl访问 https的例子参考这里 https://curl.haxx.se/libcurl/c/https.html
//

#include "curl_mgr.h"
#include "logger.h"
#include "curl/curl.h"

size_t curl_callback(char *data, size_t size, size_t count, void* userdata);

int init_curl()
{
    LOGI(CURLMGR, "init curl, NOTE that it should be just once and in main thread!!!!!!!!!!!!!");
    /**
     *  参数：flags
     *  CURL_GLOBAL_ALL              //初始化所有的可能的调用。
     *  CURL_GLOBAL_SSL              //初始化支持 安全套接字层。
     *  CURL_GLOBAL_WIN32            //初始化win32套接字库。
     *  CURL_GLOBAL_NOTHING          //没有额外的初始化。
     */
    return curl_global_init(CURL_GLOBAL_ALL);
}

char * get_curl_version()
{
    return curl_version();
}

int get(const char* url, LPCURL_DOWNLOAD_OBJECT obj, const char* cacert)
{
    LOGI(CURLMGR, "begin get method[%s]", url);
    LOGI(CURLMGR, "cert path[%s]", cacert);

    CURL* curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);

#ifdef SKIP_PEER_VERIFICATION
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &curl_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, obj);
//    curl_easy_setopt(curl, CURLMOPT_PIPELINING, 1L);//开启多路复用
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_CAINFO, cacert);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
    curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);

    curl_version_info_data * cvid = curl_version_info(CURLVERSION_NOW);
    if (cvid->features & CURL_VERSION_SSL)
    {
        LOGI(CURLMGR, "SSL support enabled [version %s]", cvid->ssl_version);
    }else
    {
        LOGE(CURLMGR, "no SSL support");
    }

    CURLcode code = curl_easy_perform(curl);
    if (code != CURLE_OK)
    {
        LOGE(CURLMGR, "CURL failed with error code [%d]", code);
    } else
    {
        LOGI(CURLMGR, "CURL success, result[%d]", code);
    }

    curl_easy_cleanup(curl);

    LOGI(CURLMGR, "end get method[%s]", url);

    return code == CURLE_OK;
}

void post()
{

}

void put()
{

}

size_t curl_callback(char *data, size_t size, size_t count, void* userdata) {

    LOGI(CURLMGR, "Downloaded data size is [%d]", size * count);

    LPCURL_DOWNLOAD_OBJECT downloadObject = (LPCURL_DOWNLOAD_OBJECT) userdata;

    long newSize = 0;
    long offset = 0;
    char* dataPtr;

    if (downloadObject == NULL)
    {
        LOGE(CURLMGR, "download object is null");
        return 0;
    }

    if (downloadObject->data == NULL)
    {
        newSize = size * count * sizeof(char);
        dataPtr = (char *)malloc(newSize);
    }else
    {
        newSize = downloadObject->size + (size * count * sizeof(char));
        dataPtr = (char *)realloc(downloadObject->data, newSize);
        offset = downloadObject->size;
    }

    if (dataPtr==NULL)
    {
        if (downloadObject->data != NULL)
        {
            free(downloadObject->data);
            downloadObject->data = NULL;
            downloadObject->size = 0;
        }

        return 0;
    }

    downloadObject->data = dataPtr;
    downloadObject->size = newSize;

    memcpy(downloadObject->data + offset, data, size * count * sizeof(char));

    return size*count;
}