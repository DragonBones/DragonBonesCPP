LOCAL_PATH := $(call my-dir)

LOCAL_MODULE := dragonbones_static

LOCAL_MODULE_FILENAME := libdragonbones


LOCAL_SRC_FILES := ../../../animation/Animation.cpp \
                   ../../../animation/AnimationState.cpp \
                   ../../../animation/TimelineState.cpp \
                   ../../../animation/WorldClock.cpp \
                   ../../../core/Armature.cpp \
                   ../../../core/Bone.cpp \
                   ../../../core/Object.cpp \
                   ../../../core/Slot.cpp \
                   ../../../events/EventData.cpp \
                   ../../../factories/BaseFactory.cpp \
                   ../../../parsers/BaseDataParser.cpp \
                   ../../../parsers/ConstValues.cpp \
                   ../../../parsers/dbtinyxml2.cpp \
                   ../../../parsers/XMLDataParser.cpp \
                   ../DBCCArmature.cpp \
                   ../DBCCArmatureNode.cpp \
                   ../DBCCFactory.cpp \
                   ../DBCCLuaUtils.cpp \
                   ../DBCCSlot.cpp \
				   ../DBCCUtils.cpp


LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/.. \
$(LOCAL_PATH)/../../.. 

LOCAL_EXPORT_C_INCLUDES := \
$(LOCAL_PATH)/.. \
$(LOCAL_PATH)/../../.. 

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static

LOCAL_CFLAGS += -Wno-psabi
LOCAL_EXPORT_CFLAGS += -Wno-psabi

include $(BUILD_STATIC_LIBRARY)

$(call import-module,.)
