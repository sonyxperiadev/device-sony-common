LIBJSON_ROOT := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := libjson
LOCAL_PATH := $(LIBJSON_ROOT)
ifeq ($(TARGET_COMPILE_WITH_MSM_KERNEL),true)
LOCAL_C_INCLUDES += $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr/include
LOCAL_ADDITIONAL_DEPENDENCIES += $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr
endif
LOCAL_COPY_HEADERS_TO := libjson/inc
LOCAL_COPY_HEADERS := bits.h \
		config.h \
		debug.h \
		linkhash.h \
		arraylist.h \
		json.h \
		json_config.h \
		json_inttypes.h \
		json_util.h \
		json_object.h \
		json_tokener.h \
		json_object_iterator.h \
		json_c_version.h
LOCAL_SRC_FILES := arraylist.c \
		debug.c \
		json_c_version.c \
		json_object.c \
		json_object_iterator.c \
		json_tokener.c \
		json_util.c \
		libjson.c \
		linkhash.c \
		printbuf.c \
		random_seed.c
LOCAL_CFLAGS += -Wno-unused-parameter -Werror
LOCAL_SHARED_LIBRARIES := libcutils libutils
LOCAL_MODULE_TAG := optional
include $(BUILD_SHARED_LIBRARY)
