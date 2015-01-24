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

const float squareWidth = 0.25;
const float minX = -2.0;
const float maxX = 2.0;
const float minY = -2.0;
const float maxY = 2.0;
const int fh_dim = (int)((maxX-minX)/squareWidth);
const int fh_size = fh_dim*fh_dim;

DanceSteps::DanceSteps(){
	c = new Cube();
	c->SetScale(kCubeScale);
	floor_heights = new std::pair<float,float>[fh_size];
	for(int i=0;i<fh_size;i++){
		floor_heights[i] = std::make_pair(0.0f,0.0f);
	}
}

DanceSteps::~DanceSteps(){
	delete c;
	delete[] floor_heights;
}

void DanceSteps::Render(const glm::mat4& projection_mat, const glm::mat4& view_mat) const {
	LOGI("Start render");
	for(int j=0;j<fh_dim;j++){
		for(int i=0;i<fh_dim;i++){
			int offset = j*fh_dim + i;
			if(offset >= 0 && offset < fh_size && floor_heights[offset].second >= 1.0f){
				c->SetPosition(glm::vec3(minX + i*squareWidth,
					floor_heights[offset].first/floor_heights[offset].second,
					minY + j*squareWidth));

				c->Render(projection_mat, view_mat);
			}
		}
	}
	LOGI("Finish render");

}

void DanceSteps::addDepthMapData(float* points, uint32_t num_points){
	for(int j=0;j<fh_size;j++){
		floor_heights[j].first = 0.0f;
		floor_heights[j].second = 0.0f;
	}

	for(int i=0;i<num_points; i++){
		int xindex = (points[3*i+0] - minX)/squareWidth;
		int yindex = (points[3*i+2] - minY)/squareWidth;
		int offset = xindex + yindex*fh_dim;

		if(offset >= 0 && offset < fh_size) {
			floor_heights[offset].first += points[3*i+1];
			floor_heights[offset].second += 1.0f;
		}
	}
}
