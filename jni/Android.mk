LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE	:= dnsproxy2
LOCAL_SRC_FILES	:= dnsproxy2.cpp FrameworkListener.cpp \
	FrameworkCommand.cpp DnsProxyListener.cpp SocketListener.cpp \
	SocketClient.cpp NetdCommand.cpp logd_write.c atomic.c
LOCAL_CFLAGS	:= -DHAVE_SYS_UIO_H \
	-DANDROID_SMP=1 \
	-include netdb-private.h

include $(BUILD_EXECUTABLE)
