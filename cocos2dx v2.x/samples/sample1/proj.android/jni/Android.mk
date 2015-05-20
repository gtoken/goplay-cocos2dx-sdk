LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

CPP_FILES  := $(wildcard $(LOCAL_PATH)/../../Classes/*.cpp)
CPP_FILES  += $(wildcard $(LOCAL_PATH)/../../Classes/**/*.cpp)
CPP_FILES  += $(wildcard $(LOCAL_PATH)/../../Classes/**/**/*.cpp)

CPP_FILES  += $(wildcard $(LOCAL_PATH)/../../Classes/*.c)
CPP_FILES  += $(wildcard $(LOCAL_PATH)/../../Classes/**/*.c)
CPP_FILES  += $(wildcard $(LOCAL_PATH)/../../Classes/**/**/*.c)

CPP_FILES  := $(CPP_FILES:$(LOCAL_PATH)/%=%)

LOCAL_SRC_FILES := hellocpp/main.cpp \
$(CPP_FILES) \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes\
$(LOCAL_PATH)/../../Classes/NDKHelper \
$(LOCAL_PATH)/../../Classes/jansson \
$(LOCAL_PATH)/../../Classes/Lua \


LOCAL_STATIC_LIBRARIES := cocos2dx_static
LOCAL_STATIC_LIBRARIES += PluginProtocolStatic

LOCAL_WHOLE_STATIC_LIBRARIES += PluginProtocolStatic

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
$(call import-module,plugin/protocols/proj.android/jni)