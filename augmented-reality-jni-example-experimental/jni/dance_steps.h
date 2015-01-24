#ifndef DANCE_STEPS_H
#define DANCE_STEPS_H
#include <deque>
class DanceSteps {
public:
	DanceSteps();
	void Render(const glm::mat4& projection_mat, const glm::mat4& view_mat) const;
	void UpdatePixies();
	~DanceSteps();

	void addDepthMapData(float* points, uint32_t);

	Cube* c;
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

	bool pixie_bit_this_turn;
	bool pixie_missed_this_turn;
	bool pixie_created_this_turn;
	bool pixie_attacked_this_turn;

	std::pair<float,float>* floor_heights;
	std::deque<Pixie> pixie_queue;
};

#endif //DANCE_STEPS_H
