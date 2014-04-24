LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := hellocpp_shared

LOCAL_MODULE_FILENAME := libhellocpp

LOCAL_SRC_FILES := \
	hellocpp/main.cpp \
	$(wildcard ../../Classes/*.cpp) \
	$(wildcard ../../Classes/example/*.cpp) \

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES := dragonbones_cocos2dx2_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,renderer/cocos2d-x-2.x)
