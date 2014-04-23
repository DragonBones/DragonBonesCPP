DRAGONBONES_ROOT = ../../../..
COCOS2DX_2_ROOT = $(abspath $(DRAGONBONES_ROOT))/engines/cocos2d-x-2.x
export NDK_MODULE_PATH = $(abspath $(DRAGONBONES_ROOT)):$(COCOS2DX_2_ROOT)

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := hellocpp_shared

LOCAL_MODULE_FILENAME := libhellocpp

LOCAL_SRC_FILES := \
	hellocpp/main.cpp \
	../../Classes/AppDelegate.cpp \
	../../Classes/VisibleRect.cpp \
	../../Classes/example/DragonBonesDemo.cpp \
	../../Classes/example/DragonChaseStarling.cpp \
	../../Classes/example/DragonDemoEntry.cpp \
	../../Classes/example/DragonSwitchClothes.cpp \

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES := \
	dragonbones_cocos2dx2_static \
	cocos2dx_static \
	dragonbones_static \

include $(BUILD_SHARED_LIBRARY)

$(call import-module,renderer/cocos2d-x-2.x)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)
