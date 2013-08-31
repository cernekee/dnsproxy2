LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE	:= dnsproxy2
LOCAL_SRC_FILES	:= dnsproxy2.c

include $(BUILD_EXECUTABLE)
