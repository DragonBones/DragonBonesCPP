LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := dragonbones_static

LOCAL_MODULE_FILENAME := libdragonbones


LOCAL_SRC_FILES :=      ../animation/Animation.cpp \
                        ../animation/AnimationState.cpp \
                        ../animation/TimelineState.cpp \
                        ../animation/WorldClock.cpp \
                        ../armature/Armature.cpp \
                        ../armature/Bone.cpp \
                        ../armature/Slot.cpp \
                        ../core/BaseObject.cpp \
                        ../events/EventObject.cpp \
                        ../factories/BaseFactory.cpp \
                        ../model/AnimationData.cpp \
                        ../model/ArmatureData.cpp \
                        ../model/DragonBonesData.cpp \
                        ../model/FrameData.cpp \
                        ../model/TimelineData.cpp \
                        ../parsers/DataParser.cpp \
                        ../parsers/JSONDataParser.cpp \
                        ../textures/TextureData.cpp \
                        ../cocos2dx/CCArmatureDisplay.cpp \
                        ../cocos2dx/CCFactory.cpp \
                        ../cocos2dx/CCSlot.cpp \
                        ../cocos2dx/CCTextureData.cpp


LOCAL_C_INCLUDES :=     $(LOCAL_PATH)/../ \
                        $(LOCAL_PATH)/../animation \
                        $(LOCAL_PATH)/../armature \
                        $(LOCAL_PATH)/../core \
                        $(LOCAL_PATH)/../events \
                        $(LOCAL_PATH)/../factories \
                        $(LOCAL_PATH)/../geom \
                        $(LOCAL_PATH)/../model \
                        $(LOCAL_PATH)/../parsers \
                        $(LOCAL_PATH)/../cocos2dx \
                        $(LOCAL_PATH)/../textures \

LOCAL_EXPORT_C_INCLUDES :=     $(LOCAL_PATH)/../ \
                        $(LOCAL_PATH)/../animation \
                        $(LOCAL_PATH)/../armature \
                        $(LOCAL_PATH)/../core \
                        $(LOCAL_PATH)/../events \
                        $(LOCAL_PATH)/../factories \
                        $(LOCAL_PATH)/../geom \
                        $(LOCAL_PATH)/../model \
                        $(LOCAL_PATH)/../parsers \
                        $(LOCAL_PATH)/../cocos2dx \
                        $(LOCAL_PATH)/../textures \

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static

LOCAL_CFLAGS += -Wno-psabi
LOCAL_EXPORT_CFLAGS += -Wno-psabi

include $(BUILD_STATIC_LIBRARY)

$(call import-module,.)
