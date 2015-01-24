#ifndef DANCE_STEPS_H
#define DANCE_STEPS_H

class DanceSteps {
public:
	DanceSteps();
	void Render(const glm::mat4& projection_mat, const glm::mat4& view_mat) const;
	~DanceSteps();

	void addDepthMapData(float* points, uint32_t);
private:
	Cube* c;
	std::pair<float,float>* floor_heights;

};

#endif //DANCE_STEPS_H
