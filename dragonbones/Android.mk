LOCAL_PATH := $(call my-dir)

LOCAL_MODULE := dragonbones_static

LOCAL_MODULE_FILENAME := libdragonbones

LOCAL_SRC_FILES := \
	$(wildcard $(LOCAL_PATH)/*.cpp) \
	$(wildcard $(LOCAL_PATH)/animation/*.cpp) \
	$(wildcard $(LOCAL_PATH)/core/*.cpp) \
	$(wildcard $(LOCAL_PATH)/display/*.cpp) \
	$(wildcard $(LOCAL_PATH)/events/*.cpp) \
	$(wildcard $(LOCAL_PATH)/factories/*.cpp) \
	$(wildcard $(LOCAL_PATH)/objects/*.cpp) \
	$(wildcard $(LOCAL_PATH)/textures/*.cpp) \
	$(wildcard $(LOCAL_PATH)/utils/*.cpp) \

# TODO(hejiangzhou): Shall we disable exception?
LOCAL_CPPFLAGS := -fexceptions -std=c++11

LOCAL_EXPORT_CPPFLAGS := -fexceptions -std=c++11

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

LOCAL_C_INCLUDES := $(LOCAL_PATH)

NDK_TOOLCHAIN_VERSION := 4.8

include $(BUILD_STATIC_LIBRARY)
