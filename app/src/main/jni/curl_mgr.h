//
// Created by 钟龙州 on 16/8/6.
//

#ifndef USECURL_CURL_MGR_H
#define USECURL_CURL_MGR_H

#define SKIP_PEER_VERIFICATION
#define SKIP_HOSTNAME_VERIFICATION

#define CURLMGR "curl_mgr"

typedef struct _CURL_DOWNLOAD_OBJECT {
    long  size;
    char* data;
} CURL_DOWNLOAD_OBJECT, *LPCURL_DOWNLOAD_OBJECT;

int    init_curl();
char*  get_curl_version();


int   get(const char* url, LPCURL_DOWNLOAD_OBJECT obj, const char* cacert);
void   post();
void   put();



#endif //USECURL_CURL_MGR_H
