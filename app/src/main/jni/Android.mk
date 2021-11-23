LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := boost-serialization
LOCAL_SRC_FILES := $(LOCAL_PATH)/boost/lib/libboost_serialization-clang-mt-1_72.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/boost/include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := sfml-example


LOCAL_CFLAGS += -I$(LOCAL_PATH)/boost/include
LOCAL_CFLAGS += -I$(LOCAL_PATH)/cereal/include

LOCAL_LDLIBS += -L$(LOCAL_PATH)/boost/lib

LOCAL_LDLIBS += -llog
#LIBS += -lboost_serialization -lboost_archive

LOCAL_CPPFLAGS += -fexceptions
LOCAL_CPPFLAGS += -frtti


LOCAL_SRC_FILES := main.cpp
LOCAL_SRC_FILES += $(LOCAL_PATH)/client/entity.cpp
LOCAL_SRC_FILES += $(LOCAL_PATH)/client/player.cpp
LOCAL_SRC_FILES += $(LOCAL_PATH)/client/enemy.cpp
LOCAL_SRC_FILES += $(LOCAL_PATH)/client/ingameText.cpp
LOCAL_SRC_FILES += $(LOCAL_PATH)/client/item.cpp
LOCAL_SRC_FILES += $(LOCAL_PATH)/client/projectile.cpp
LOCAL_SRC_FILES += $(LOCAL_PATH)/client/random.cpp
LOCAL_SRC_FILES += $(LOCAL_PATH)/client/wall.cpp
LOCAL_SRC_FILES += $(LOCAL_PATH)/client/animatedGIF.cpp
LOCAL_SRC_FILES += $(LOCAL_PATH)/client/Chatbox.cpp
LOCAL_SRC_FILES += $(LOCAL_PATH)/client/GamePlay.cpp
LOCAL_SRC_FILES += $(LOCAL_PATH)/client/Inputbox.cpp
LOCAL_SRC_FILES += $(LOCAL_PATH)/client/sha256.cpp
LOCAL_SRC_FILES += $(LOCAL_PATH)/client/MainMenu.cpp

LOCAL_SRC_FILES += $(LOCAL_PATH)/client/StateManager.cpp
LOCAL_SRC_FILES += $(LOCAL_PATH)/client/Game.cpp

#LOCAL_C_INCLUDES := $(LOCAL_PATH)/boost/include
LOCAL_C_INCLUDES += $(LOCAL_PATH)/net



LOCAL_SHARED_LIBRARIES := sfml-system-d
LOCAL_SHARED_LIBRARIES += sfml-window-d
LOCAL_SHARED_LIBRARIES += sfml-graphics-d
LOCAL_SHARED_LIBRARIES += sfml-audio-d
LOCAL_SHARED_LIBRARIES += sfml-network-d
LOCAL_SHARED_LIBRARIES += sfml-activity-d
LOCAL_SHARED_LIBRARIES += openal

LOCAL_WHOLE_STATIC_LIBRARIES := sfml-main-d boost-serialization
#LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_SHARED_LIBRARY)

$(call import-module,third_party/sfml)
#$(call import-module,android/native_app_glue)
