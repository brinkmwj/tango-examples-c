#ifndef DANCE_STEPS_H
#define DANCE_STEPS_H
#include <deque>
#include "particle_emitter.h"
class DanceSteps {
public:
	DanceSteps();
	void Render(const glm::mat4& projection_mat, const glm::mat4& view_mat) const;
	void UpdatePixies();
	~DanceSteps();
	void doPixieSpawner();

	void addDepthMapData(float* points, uint32_t);

	ParticleEmitter* pe;
	class Pixie {
	public:
		glm::vec3 start_position; //In world coordinates, where pixie starts
		glm::vec3 attack_direction;
		glm::vec3 cur_position;

		double start_time;
		bool is_new;
		bool is_dead;
		bool is_attacking;



		Pixie(glm::vec3 ipos, double ist){
			start_position = ipos;
			start_time = ist;
			is_new = true;
			is_dead = false;
			is_attacking = false;
		}
	};

	bool createRandomPixie();
	void squashPixie();

	float pixie_wait_time ;
	float pixie_attack_time ;
	float pixie_hit_distance ;
	float pixie_squash_distance ;

	float pixie_spawn_time ;
	float pixie_spawn_start ;

	int pixie_bit_this_turn;
	int pixie_missed_this_turn;
	int pixie_created_this_turn;
	int pixie_attacked_this_turn;
	int pixie_squashed_this_turn;

	int pixies_spawned;

	double last_pixie_spawn;

	std::pair<float,float>* floor_heights;
	std::pair<float,float>* old_floor_heights;
	std::deque<Pixie> pixie_queue;
};

#endif //DANCE_STEPS_H
