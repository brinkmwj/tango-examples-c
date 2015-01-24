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

#include <time.h>

// from android samples
/* return current time in milliseconds */
static double now_ms(void) {

    struct timespec res;
    clock_gettime(CLOCK_REALTIME, &res);
    return 1000.0 * res.tv_sec + (double) res.tv_nsec / 1e6;

}

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

bool DanceSteps::createRandomPixie(){
	double start_time = now_ms();
	for(int i=0; i<10; i++) {	//Make 10 tries, then give up
		int xoffset = rand()%fh_dim;
		int yoffset = rand()%fh_dim;
		int offset = xoffset + yoffset*fh_dim;
		if(floor_heights[offset].second >= 5.0f){
			Pixie p(glm::vec3(glm::vec3(minX + xoffset*squareWidth,
					floor_heights[offset].first/floor_heights[offset].second,
					minY + yoffset*squareWidth)),start_time);
			pixie_queue.push_back(p);
			return true;
		}
	}

	return false;
}

void DanceSteps::UpdatePixies(){
	if(pixie_queue.size() < 2){
		createRandomPixie();
	}

	double now = now_ms();
	while(pixie_queue.size() > 0 && (now - pixie_queue[0].start_time) > 5000){
		pixie_queue.pop_front();
	}
}

void DanceSteps::Render(const glm::mat4& projection_mat, const glm::mat4& view_mat) const  {
	for(int i=0; i<pixie_queue.size(); i++){
		//First, check the floor height of the pixie's position, make sure it is visible
		int xindex = (pixie_queue[i].position.x - minX)/squareWidth;
		int yindex = (pixie_queue[i].position.z - minY)/squareWidth;
		int offset = xindex + yindex*fh_dim;
		if(floor_heights[offset].second > 5.0f &&
				(floor_heights[offset].first/floor_heights[offset].second) < pixie_queue[i].position.y + 0.1f ){
			c->SetPosition(pixie_queue[i].position);
			c->Render(projection_mat, view_mat);
		}
	}
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
