LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE	:= dnsproxy2
LOCAL_SRC_FILES	:= dnsproxy2.cpp \
	FrameworkListener.cpp \
	FrameworkCommand.cpp \
	DnsProxyListener.cpp \
	SocketListener.cpp \
	SocketClient.cpp \
	NetdCommand.cpp \
	gethnamaddr.c \
	getaddrinfo.c \
	getnameinfo.c \
	getservbyname.c \
	getservbyport.c \
	getservent.c \
	nsdispatch.c \
	logd_write.c \
	atomic.c \
	resolv/__dn_comp.c \
	resolv/herror.c \
	resolv/res_cache.c \
	resolv/__res_close.c \
	resolv/res_compat.c \
	resolv/res_comp.c \
	resolv/res_data.c \
	resolv/res_debug.c \
	resolv/res_init.c \
	resolv/res_mkquery.c \
	resolv/res_query.c \
	resolv/__res_send.c \
	resolv/res_send.c \
	resolv/res_state.c \
	resolv/libc_logging.cpp \
	nameser/ns_name.c \
	nameser/ns_netint.c \
	nameser/ns_parse.c \
	nameser/ns_print.c \
	nameser/ns_samedomain.c \
	nameser/ns_ttl.c

LOCAL_CFLAGS	:= -DHAVE_SYS_UIO_H \
	-DANDROID_SMP=1 \
	-DANDROID_CHANGES \
	-D_LIBC \
	-I$(LOCAL_PATH) \
	-I$(LOCAL_PATH)/resolv \
	-I$(LOCAL_PATH)/private \
	-include netdb-private.h

include $(BUILD_EXECUTABLE)
