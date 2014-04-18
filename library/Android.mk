LOCAL_PATH := $(call my-dir)

LOCAL_MODULE := dragonbones_cocos2dx_static

LOCAL_MODULE_FILENAME := libdragonbones-cocos2d

LOCAL_SRC_FILES := \
Animation.cpp \
AnimationEvent.cpp \
AnimationState.cpp \
Armature.cpp \
ArmatureEvent.cpp \
BaseFactory.cpp \
Bone.cpp \
BytesType.cpp \
Cocos2dxAtlasNode.cpp \
Cocos2dxDisplayBridge.cpp \
Cocos2dxFactory.cpp \
Cocos2dxTextureAtlas.cpp \
ConstValues.cpp \
DBDataUtil.cpp \
DBObject.cpp \
dbtinyxml2.cpp \
DisplayData.cpp \
DragonBones.cpp \
Event.cpp \
FrameEvent.cpp \
Matrix.cpp \
Slot.cpp \
SoundEvent.cpp \
SoundEventManager.cpp \
TimelineState.cpp \
TransformTimeline.cpp \
TransformUtil.cpp \
XMLDataParser.cpp \

# TODO(hejiangzhou): Shall we disable exception?
LOCAL_CPPFLAGS := -fexceptions -std=c++11

LOCAL_EXPORT_CPPFLAGS := -fexceptions -std=c++11

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

LOCAL_C_INCLUDES := $(LOCAL_PATH)

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static

NDK_TOOLCHAIN_VERSION := 4.8

include $(BUILD_STATIC_LIBRARY)

$(call import-module,cocos2dx)
