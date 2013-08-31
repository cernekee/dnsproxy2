LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE	:= dnsproxy2
LOCAL_SRC_FILES	:= dnsproxy2.c FrameworkListener.cpp \
	FrameworkCommand.cpp DnsProxyListener.cpp
LOCAL_CFLAGS	:= -DHAVE_SYS_UIO_H -include netdb-private.h

include $(BUILD_EXECUTABLE)
