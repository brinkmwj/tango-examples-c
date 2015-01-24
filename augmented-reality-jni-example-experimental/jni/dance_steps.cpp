#include "tango-gl-renderer/axis.h"
#include "tango-gl-renderer/camera.h"
#include "tango-gl-renderer/cube.h"
#include "tango-gl-renderer/frustum.h"
#include "tango-gl-renderer/gl_util.h"
#include "tango-gl-renderer/grid.h"
#include "tango-gl-renderer/trace.h"

#include "dance_steps.h"
#include "tango_data.h"

const glm::vec3 kCubeScale = glm::vec3(0.1f, 0.001f, 0.1f);

DanceSteps::DanceSteps(){
	c = new Cube();
	c->SetScale(kCubeScale);
}

DanceSteps::~DanceSteps(){
	delete c;
}

void DanceSteps::Render(const glm::mat4& projection_mat, const glm::mat4& view_mat) const {
	float floor_height = TangoData::GetInstance().floor_height;
	for(float y = 0.0f; y < 10.0f; y += 0.5f){
		for(float x = 0.0f; x < 10.0f; x += 0.5f){
			c->SetPosition(glm::vec3(x,floor_height,y));
			c->Render(projection_mat, view_mat);
		}
	}
}
