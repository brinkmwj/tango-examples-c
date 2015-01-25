#include "tango-gl-renderer/axis.h"
#include "tango-gl-renderer/camera.h"
#include "tango-gl-renderer/cube.h"
#include "tango-gl-renderer/frustum.h"
#include "tango-gl-renderer/gl_util.h"
#include "tango-gl-renderer/grid.h"
#include "tango-gl-renderer/trace.h"

#include "dance_steps.h"
#include "tango_data.h"

const glm::vec3 kCubeScale = glm::vec3(0.1f, 0.1f, 0.1f);

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
	old_floor_heights = new std::pair<float,float>[fh_size];
	for(int i=0;i<fh_size;i++){
		floor_heights[i] = std::make_pair(0.0f,0.0f);
		old_floor_heights[i] = std::make_pair(0.0f,0.0f);
	}

	pixie_bit_this_turn = 0;
	pixie_missed_this_turn = 0;
	pixie_created_this_turn = 0;
	pixie_attacked_this_turn = 0;
	pixie_squashed_this_turn = 0;

	 pixie_wait_time = 10000.0f;
	 pixie_attack_time = 2000.0f;
	 pixie_hit_distance = 0.5f;
	 pixie_squash_distance = 0.5f;

	 pixie_spawn_time = 5000.0f;
	 pixie_spawn_start = -1.0f;
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
			pixie_created_this_turn += 1;

			last_pixie_spawn = now_ms();
			return true;
		}
	}

	return false;
}



void DanceSteps::squashPixie(){
	glm::vec3 player_position = TangoData::GetInstance().tango_position_depth;
	for(int i=0;i<pixie_queue.size();i++){
		if(!pixie_queue[i].is_dead &&
				!pixie_queue[i].is_attacking &&
				glm::length(player_position - pixie_queue[i].cur_position) < pixie_squash_distance){
			pixie_queue[i].is_dead = true;
			pixie_squashed_this_turn += 1;
		}
	}
}

void DanceSteps::UpdatePixies(){
	double now = now_ms();
	glm::vec3 player_position = TangoData::GetInstance().tango_position_depth;

	//update location of each pixie
	for(int i=0;i<pixie_queue.size();i++){
		if(now - pixie_queue[i].start_time < pixie_wait_time){
			//do nothing
			pixie_queue[i].cur_position = pixie_queue[i].start_position;
		} else if (pixie_queue[i].is_dead == false && (now - pixie_queue[i].start_time) > pixie_wait_time+pixie_attack_time){
			pixie_queue[i].is_dead = true;
			pixie_missed_this_turn += 1;
		} else if (pixie_queue[i].is_dead == false) {
			if(!pixie_queue[i].is_attacking){
				pixie_queue[i].is_attacking = true;
				pixie_attacked_this_turn += 1;

				pixie_queue[i].attack_direction = glm::normalize(player_position-pixie_queue[i].start_position);
			}

			float ratio = ((now - pixie_queue[i].start_time) - pixie_wait_time)/pixie_attack_time;
			pixie_queue[i].cur_position = pixie_queue[i].start_position + pixie_queue[i].attack_direction*(ratio*5.0f);
			if(glm::length(player_position - pixie_queue[i].cur_position) < pixie_hit_distance){
				pixie_queue[i].is_dead = true;
				pixie_bit_this_turn += 1;
			}
		}
	}

	//Dispose dead pixies
	while(pixie_queue.size() > 0 && pixie_queue[0].is_dead){
		pixie_queue.pop_front();
	}

	doPixieSpawner();
}

void DanceSteps::doPixieSpawner(){
	if(pixie_spawn_start < 0.0f){
		pixie_spawn_start = now_ms();
		last_pixie_spawn = pixie_spawn_start;
	}

	if(now_ms() - last_pixie_spawn > pixie_spawn_time){
		createRandomPixie();
	}
}

void DanceSteps::Render(const glm::mat4& projection_mat, const glm::mat4& view_mat) const  {
	for(int i=0; i<pixie_queue.size(); i++){
		//First, check the floor height of the pixie's position, make sure it is visible
		int xindex = (pixie_queue[i].cur_position.x - minX)/squareWidth;
		int yindex = (pixie_queue[i].cur_position.z - minY)/squareWidth;
		int offset = xindex + yindex*fh_dim;
		if(!pixie_queue[i].is_dead){
			c->SetPosition(pixie_queue[i].cur_position);
			c->Render(projection_mat, view_mat);
		}
	}
}

void DanceSteps::addDepthMapData(float* points, uint32_t num_points){
	std::swap(floor_heights,old_floor_heights);

	//First, clear out the buffer to fill
	for(int j=0;j<fh_size;j++){
		floor_heights[j].first = 0.0f;
		floor_heights[j].second = 0.0f;
	}
	//Next, fill it
	for(int i=0;i<num_points; i++){
		int xindex = (points[3*i+0] - minX)/squareWidth;
		int yindex = (points[3*i+2] - minY)/squareWidth;
		int offset = xindex + yindex*fh_dim;

		if(offset >= 0 && offset < fh_size) {
			floor_heights[offset].first += points[3*i+1];
			floor_heights[offset].second += 1.0f;
		}
	}
	//Then, fill in empty bits using old_floor_height
	/*for(int i=0;i<fh_size;i++){
		if(floor_heights[i].second < 5.0f && old_floor_heights[i].second >= 5.0f){
			floor_heights[i] = old_floor_heights[i];
		}
	}*/
}
