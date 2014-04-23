LOCAL_PATH := $(call my-dir)

LOCAL_MODULE := dragonbones_static

LOCAL_MODULE_FILENAME := libdragonbones

LOCAL_SRC_FILES := \
	Animation.cpp \
	AnimationEvent.cpp \
	AnimationState.cpp \
	Armature.cpp \
	ArmatureEvent.cpp \
	BaseFactory.cpp \
	Bone.cpp \
	BytesType.cpp \
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

NDK_TOOLCHAIN_VERSION := 4.8

include $(BUILD_STATIC_LIBRARY)
