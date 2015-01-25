#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include "tango-gl-renderer/gl_util.h"

class ParticleEmitter {
public:
	ParticleEmitter();

	int num_parts;
	float* part_coords;
	float* part_start_coords;
	float* part_end_coords;
	float* part_ratios;
	float* part_colors;

	GLuint vertex_buffers_;
	GLuint color_buffers_;

	GLuint shader_program_;
	GLuint attrib_vertices_;
	GLuint attrib_colors_;
	GLuint uniform_mvp_mat_;

	void UpdateParticles();

	void Render(glm::mat4 projection_mat, glm::mat4 view_mat, bool pixie_is_attacking);
	glm::vec3 emitter_location;
};

#endif //PARTICLE_EMITTER_H
