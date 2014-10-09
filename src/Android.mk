LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

JNI_PATH := ../android/jni
SDL_PATH        := $(JNI_PATH)/SDL
SDL_IMAGE_PATH  := $(JNI_PATH)/SDL_image
SDL_MIXER_PATH  := $(JNI_PATH)/SDL_mixer
SDL_TTF_PATH    := $(JNI_PATH)/SDL_ttf
EXTERNAL_LIB_PATH := ../external

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include \
  $(EXTERNAL_LIB_PATH) \
  $(EXTERNAL_LIB_PATH)/polyvox/library/PolyVoxCore/include \
  $(EXTERNAL_LIB_PATH)/polyvox/library/PolyVoxUtil/include \
  $(LOCAL_PATH)/$(SDL_IMAGE_PATH) \
  $(LOCAL_PATH)/$(SDL_MIXER_PATH) \
  $(LOCAL_PATH)/$(SDL_TTF_PATH) \
  $(BULLET_DIR)/include/bullet \
  $(BOOST_DIR)/include \
  $(GLM_DIR)

# Add your application source files here...
LOCAL_SRC_FILES := $(SDL_PATH)/src/main/android/SDL_android_main.c \
	 $(wildcard $(JNI_PATH)/src/*.cpp) \
	 $(wildcard $(JNI_PATH)/src/audio/*.cpp) \
	 $(wildcard $(JNI_PATH)/src/core/*.cpp) \
	 $(wildcard $(JNI_PATH)/src/extern/*.cpp) \
	 $(wildcard $(JNI_PATH)/src/graphics/*.cpp) \
	 $(wildcard $(JNI_PATH)/src/io/*.cpp) \
	 $(wildcard $(JNI_PATH)/src/math/*.cpp) \
	 $(wildcard $(JNI_PATH)/src/model/*.cpp) \
	 $(wildcard $(JNI_PATH)/src/util/*.cpp)

#  $(shell find $(JNI_PATH)/src/ -name "*.cpp")

# Alternative way to wildcard-include files.
#  $(wildcard $(JNI_PATH)/src/*.cpp)

# To view variables use:
#$(warning $(LOCAL_SRC_FILES))

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_image SDL2_mixer SDL2_ttf \
	                        noise \
	                        PolyVoxCore PolyVoxUtil \
	                        flite_cmu_us_awb flite_cmu_us_rms \
	                        flite_voice_list flite_cmulex flite_usenglish flite

# bullet
LOCAL_STATIC_LIBRARIES := cpufeatures

LOCAL_LDLIBS := -llog -lGLESv3 -lEGL
LOCAL_CFLAGS := -fno-strict-aliasing -D_REENTRANT -DGLM_FORCE_RADIANS
LOCAL_CFLAGS += -isystem $(JNI_PATH)/src/../external
LOCAL_CFLAGS += -isystem $(GLM_DIR)
LOCAL_CFLAGS += -isystem $(BOOST_DIR)/include
LOCAL_CFLAGS += -isystem $(LOCAL_PATH)/$(SDL_PATH)/include
LOCAL_CFLAGS += -isystem $(BULLET_DIR)/include/bullet


# Android ndk and stl include paths
LOCAL_CFLAGS += -isystem $(NDK_PLATFORMS_ROOT)/$(APP_PLATFORM)/arch-$(TARGET_ARCH)/usr/include
LOCAL_CFLAGS += -isystem $(NDK_ROOT)/sources/cxx-stl/llvm-libc++/libcxx/include/
LOCAL_CFLAGS += -isystem $(NDK_ROOT)/sources/cxx-stl/gabi++/include

# Application specific defines
LOCAL_CPPFLAGS += -DUSING_SDL
LOCAL_CPPFLAGS += -DLOG_SDL_EVENTS_VERBOSELY
# LOCAL_CPPFLAGS += -DLOGGING_DISABLED    # Disables all logging
# LOCAL_CPPFLAGS += -DLOG2STREAM_DISABLED # Disables logging to stream
LOCAL_CPPFLAGS += -DLOG2FILE_DISABLED   # Disables logging to file

LOCAL_CPPFLAGS += -std=c++11
LOCAL_CPPFLAGS += -Wall -Wextra -Wcast-align -Wcast-qual \
     -fpermissive \
     -Wdisabled-optimization \
     -Wfloat-equal -Wformat=2 -Wimport -Winit-self \
     -Winline -Winvalid-pch -Wlong-long \
     -Wmissing-format-attribute -Wmissing-include-dirs \
     -Wmissing-noreturn -Wpacked -Wpointer-arith \
     -Wredundant-decls -Wshadow -Wstack-protector \
     -Wstrict-aliasing=2 -Wunreachable-code \
     -Wunsafe-loop-optimizations -Wunused \
     -Wvariadic-macros -Wwrite-strings -pedantic \
     -pedantic-errors -Woverloaded-virtual \
     -Wswitch-enum

include $(BUILD_SHARED_LIBRARY)

$(call import-moduleandroid/cpufeatures)
