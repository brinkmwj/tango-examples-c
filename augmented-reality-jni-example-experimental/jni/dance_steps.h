#ifndef DANCE_STEPS_H
#define DANCE_STEPS_H
class DanceSteps {
public:
	DanceSteps();
	void Render(const glm::mat4& projection_mat, const glm::mat4& view_mat) const;
	~DanceSteps();

private:
	Cube* c;
};

#endif //DANCE_STEPS_H
