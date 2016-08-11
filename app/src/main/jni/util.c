//
// Created by zhonglz on 16/8/6.
//

#include "util.h"
#include <stddef.h>

jbyteArray char2jbyteArray(JNIEnv *env, char* pat, unsigned int nSize)
{
    int i = 0;
    if(nSize <= 0 || pat == NULL)
    {
        return NULL;
    }

    jbyteArray tempJByteArray = (*env)->NewByteArray(env, nSize);
    jbyte *bytes = (*env)->GetByteArrayElements(env, tempJByteArray, 0);

    while(i < nSize)
    {
        bytes[i] = pat[i];
        i++;
    }

    (*env)->SetByteArrayRegion(env, tempJByteArray, 0, nSize, bytes );

    return tempJByteArray;
}