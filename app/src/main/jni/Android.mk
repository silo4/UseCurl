##################################

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := curlwithssl

LOCAL_C_INCLUDES := $(LOCAL_PATH)/.
LOCAL_C_INCLUDES += $(NDK_PATH)/platforms/$(TARGET_PLATFORM)/arch-$(TARGET_ARCH)/usr/include
LOCAL_C_INCLUDES += curl

LOCAL_SRC_FILES += curl_lib.c \
                   curl_mgr.c \
                   util.c

LOCAL_LDLIBS := -llog
LOCAL_LDLIBS += $(LOCAL_PATH)/srcLib/libcurl.so

include $(BUILD_SHARED_LIBRARY)

