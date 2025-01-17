/*
 * Copyright 2014 Google Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AUGMENTED_REALITY_JNI_EXAMPLE_TANGO_DATA_H_
#define AUGMENTED_REALITY_JNI_EXAMPLE_TANGO_DATA_H_

#include <pthread.h>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>

#include "tango_client_api.h"
#include "tango-gl-renderer/gl_util.h"

const int kVersionStringLength = 27;
// Opengl camera to color camera matrix.
const glm::mat4 oc_2_c_mat =
    glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
              -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
// Start service to opengl world matrix.
const glm::mat4 ss_2_ow_mat =
    glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
              0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
static const glm::mat4 inverse_z_mat = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
                                                 0.0f, -1.0f, 0.0f, 0.0f,
                                                 0.0f, 0.0f, -1.0f, 0.0f,
                                                 0.0f, 0.0f, 0.0f, 1.0f);
class TangoData {
 public:
  static TangoData& GetInstance() {
    static TangoData instance;
    return instance;
  }
  TangoData();

  TangoErrorType Initialize(JNIEnv* env, jobject activity);
  bool SetConfig(bool isAutoReset);
  TangoErrorType Connect();
  void Disconnect();
  void ResetMotionTracking();
  void ConnectTexture(GLuint texture_id);
  void UpdateColorTexture();
  bool GetPoseAtTime();
  bool GetIntrinsics();
  bool GetExtrinsics();
  const char* getStatusStringFromStatusCode(TangoPoseStatusType status);
  void UpdateXYZijData();

  pthread_mutex_t xyzij_mutex;
  pthread_mutex_t pose_mutex;
  pthread_mutex_t event_mutex;
  float depth_average_length;
    float depth_frame_delta_time;
  uint32_t max_vertex_count;

  // Device to start service matrix.
    glm::mat4 d_2_ss_mat_depth;

  glm::vec3 tango_position;
  glm::vec3 tango_position_depth;
  glm::quat tango_rotation;
  glm::mat4 d_to_imu_mat;
  glm::mat4 c_to_imu_mat;


  float* depth_buffer;
  float* points_in_world;
    uint32_t depth_buffer_size;
    bool is_xyzij_dirty;

  int status_count[3];
  double timestamp;
  std::string event_string;
  std::string lib_version_string;
  std::string pose_string;

  // Tango Extrinsic for device to IMU frame and
  // color camera to IMU frame.
  glm::vec3 d_to_imu_position;
  glm::quat d_to_imu_rotation;
  glm::vec3 c_to_imu_position;
  glm::quat c_to_imu_rotation;

  // Tango Intrinsic for color camera.
  int cc_width;
  int cc_height;
  double cc_fx;
  double cc_fy;
  double cc_cx;
  double cc_cy;
  double cc_distortion[5];

  // Localization status.
  bool is_localized;
  std::string cur_uuid;

 private:
  TangoConfig config_;
};

#endif  // AUGMENTED_REALITY_JNI_EXAMPLE_TANGO_DATA_H_
