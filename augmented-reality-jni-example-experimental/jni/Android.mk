#
# Copyright 2014 Google Inc. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH:= $(call my-dir)/..

include $(CLEAR_VARS)
LOCAL_MODULE := libtango-prebuilt
LOCAL_SRC_FILES := ../tango-service-sdk/libtango_client_api.so
LOCAL_EXPORT_C_INCLUDES := ../tango-service-sdk/include
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := libaugmented_reality_jni_example
LOCAL_SHARED_LIBRARIES := libtango-prebuilt
LOCAL_CFLAGS    := -std=c++11
LOCAL_SRC_FILES := jni/tango_augmented_reality.cpp \
                   jni/tango_data.cpp \
                   jni/pointcloud.cpp \
                   jni/dance_steps.cpp \
                   jni/particle_emitter.cpp \
                   ../tango-gl-renderer/ar_ruler.cpp \
                   ../tango-gl-renderer/axis.cpp \
                   ../tango-gl-renderer/cube.cpp \
                   ../tango-gl-renderer/camera.cpp \
                   ../tango-gl-renderer/frustum.cpp \
                   ../tango-gl-renderer/gl_util.cpp \
                   ../tango-gl-renderer/grid.cpp \
                   ../tango-gl-renderer/trace.cpp \
                   ../tango-gl-renderer/transform.cpp \
                   jni/video_overlay.cpp
LOCAL_C_INCLUDES := ../tango-gl-renderer/include \
                    ../third-party/glm/
LOCAL_LDLIBS    := -llog -lGLESv2 -L$(SYSROOT)/usr/lib
include $(BUILD_SHARED_LIBRARY)
