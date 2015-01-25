#include "particle_emitter.h"
#include "tango_data.h"

static const char kVertexShader[] =
    "attribute vec4 vertex;\n"
	"attribute vec4 incolor;\n"
    "uniform mat4 mvp;\n"
    "varying vec4 v_color;\n"
    "void main() {\n"
    "  gl_PointSize = 16.0;\n"
    "  gl_Position = mvp*vertex;\n"
    "  v_color = incolor;\n"
    "}\n";

static const char kFragmentShader[] = "varying vec4 v_color;\n"
    "void main() {\n"
    "  gl_FragColor = v_color;\n"
    "}\n";

float starting_alpha = 0.25f;

ParticleEmitter::ParticleEmitter() {
  glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
  shader_program_ = GlUtil::CreateProgram(kVertexShader, kFragmentShader);
  if (!shader_program_) {
    LOGE("Could not create program.");
  }
  uniform_mvp_mat_ = glGetUniformLocation(shader_program_, "mvp");
  attrib_vertices_ = glGetAttribLocation(shader_program_, "vertex");
  attrib_colors_ = glGetAttribLocation(shader_program_, "incolor");
  glGenBuffers(1, &vertex_buffers_);
  glGenBuffers(1, &color_buffers_);

  num_parts = 64;
  part_start_coords = new float[num_parts*3];
  part_end_coords = new float[num_parts*3];
  part_coords = new float[num_parts*3];
  part_colors = new float[num_parts*4];

  part_ratios = new float[num_parts];
  for(int i=0; i<3*num_parts; i++){
	  part_start_coords[i] = 0.0f;

	  part_end_coords[i] = (rand()%256 - 128)/(128.0f*24);
  }
  for(int i=0;i<num_parts;i++){
	  part_ratios[i] = (rand()%256)/256.0f;
	  part_coords[3*i] = part_start_coords[3*i]*(1.0f-part_ratios[i]) + part_end_coords[3*i]*part_ratios[i];
	  part_coords[3*i+1] = part_start_coords[3*i+1]*(1.0f-part_ratios[i]) + part_end_coords[3*i+1]*part_ratios[i];
	  part_coords[3*i+2] = part_start_coords[3*i+2]*(1.0f-part_ratios[i]) + part_end_coords[3*i+2]*part_ratios[i];
  }
  for(int i=0;i<num_parts;i++){
	  part_colors[4*i] = 1.0f;
	  part_colors[4*i+1] = 0.0f;
	  part_colors[4*i+2] = 0.25f;
	  part_colors[4*i+3] = starting_alpha*(1.0f-part_ratios[i]);
  }
}

void ParticleEmitter::UpdateParticles(){
	glm::vec3 tango_loc = TangoData::GetInstance().tango_position_depth;
	glm::vec3 diff = tango_loc - emitter_location;
	float dist = glm::length(diff);
	float whiten_amt = 0.0f;
	if(dist > 0.5f){
		whiten_amt = 1.0f*(0.5f/dist);
	} else {
		whiten_amt = 1.0f;
	}
	for(int i=0;i<num_parts;i++){
		part_ratios[i] = part_ratios[i] + 1.0f/16.0f;
		if(part_ratios[i] >= 1.0f){
			part_ratios[i] -= 1.0f;
		}
		part_coords[3*i] = part_start_coords[3*i]*(1.0f-part_ratios[i]) + part_end_coords[3*i]*part_ratios[i];
		part_coords[3*i+1] = part_start_coords[3*i+1]*(1.0f-part_ratios[i]) + part_end_coords[3*i+1]*part_ratios[i];
		part_coords[3*i+2] = part_start_coords[3*i+2]*(1.0f-part_ratios[i]) + part_end_coords[3*i+2]*part_ratios[i];

		part_colors[4*i] = 1.0f;
		part_colors[4*i+1] = 0.0f+whiten_amt;
		part_colors[4*i+2] = 0.5f+0.5f*whiten_amt;
		part_colors[4*i+3] = starting_alpha*(1.0f-part_ratios[i]);
	}
}

void ParticleEmitter::Render(glm::mat4 projection_mat, glm::mat4 view_mat){
	glUseProgram(shader_program_);

	glm::mat4 model_mat =
		      glm::translate(glm::mat4(1.0f), emitter_location);
	  // Calculate model view projection matrix.
	static glm::mat4 inverse_z_mat(1.0f, 0.0f, 0.0f, 0.0f,
	                                                 0.0f, -1.0f, 0.0f, 0.0f,
	                                                 0.0f, 0.0f, -1.0f, 0.0f,
	                                                 0.0f, 0.0f, 0.0f, 1.0f);
	  glm::mat4 mvp_mat = projection_mat * view_mat * model_mat * inverse_z_mat;
	  glUniformMatrix4fv(uniform_mvp_mat_, 1, GL_FALSE, glm::value_ptr(mvp_mat));

	  // Bind vertex buffer.
	  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers_);
	  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * num_parts * 3,
			  part_coords, GL_STATIC_DRAW);
	  glEnableVertexAttribArray(attrib_vertices_);
	  glVertexAttribPointer(attrib_vertices_, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	  //Bind color buffer
	  glBindBuffer(GL_ARRAY_BUFFER, color_buffers_);
	  	  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * num_parts * 4,
	  			  part_colors, GL_STATIC_DRAW);
	  	  glEnableVertexAttribArray(attrib_colors_);
	  	  glVertexAttribPointer(attrib_colors_, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

	  glBindBuffer(GL_ARRAY_BUFFER, 0);
	  glEnable(GL_BLEND);
	  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	  glDrawArrays(GL_POINTS, 0, 3 * num_parts);
	  glDisable(GL_BLEND);

	  glUseProgram(0);
}
