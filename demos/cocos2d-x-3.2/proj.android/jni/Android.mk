LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../../../engines/cocos2d-x-3.2)
$(call import-add-path,$(LOCAL_PATH)/../../../../engines/cocos2d-x-3.2/external)
$(call import-add-path,$(LOCAL_PATH)/../../../../engines/cocos2d-x-3.2/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../../..)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/Demo.cpp \
                   ../../Classes/VisibleRect.cpp \
                   ../../Classes/examples/DemoChaseStarling.cpp \
                   ../../Classes/examples/DemoDragonBoy.cpp \
                   ../../Classes/examples/DemoKnight.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
#LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../dragonbones
#LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../dragonbones/renderer/cocos2d-x-3.2

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += dragonbones_static

# LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocosbuilder_static
# LOCAL_WHOLE_STATIC_LIBRARIES += spine_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocostudio_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocos_network_static
# LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,audio/android)
$(call import-module,dragonbones/renderer/cocos2d-x-3.2/android_demo)

# $(call import-module,Box2D)
# $(call import-module,editor-support/cocosbuilder)
# $(call import-module,editor-support/spine)
# $(call import-module,editor-support/cocostudio)
# $(call import-module,network)
# $(call import-module,extensions)
