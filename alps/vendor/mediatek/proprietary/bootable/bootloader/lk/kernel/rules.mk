LOCAL_DIR := $(GET_LOCAL_DIR)

MODULES += \
	lib/libc \
	lib/debug \
	lib/heap \
	lib/zlib  \
	lib/libshowlogo	

OBJS += \
	$(LOCAL_DIR)/debug.o \
	$(LOCAL_DIR)/dpc.o \
	$(LOCAL_DIR)/event.o \
	$(LOCAL_DIR)/main.o \
	$(LOCAL_DIR)/mutex.o \
	$(LOCAL_DIR)/thread.o \
	$(LOCAL_DIR)/timer.o
##zwl add for pingbo tee 20180505
ifeq ($(MTK_TRUSTKERNEL_TEE_SUPPORT),yes)
OBJS += \
    $(LOCAL_DIR)/tkcore_wp.o
endif
