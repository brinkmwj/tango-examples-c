#ifndef DANCE_STEPS_H
#define DANCE_STEPS_H
#include <deque>
class DanceSteps {
public:
	DanceSteps();
	void Render(const glm::mat4& projection_mat, const glm::mat4& view_mat);// const;
	~DanceSteps();

	void addDepthMapData(float* points, uint32_t);
private:
	Cube* c;
	class Pixie {
	public:
		glm::vec3 position; //In world coordinates
		double start_time;

		Pixie(glm::vec3 ipos, double ist){
			position = ipos;
			start_time = ist;
		}
	};

	bool createRandomPixie();

	std::pair<float,float>* floor_heights;
	std::deque<Pixie> pixie_queue;
};

#endif //DANCE_STEPS_H
