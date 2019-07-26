/*
** Seniriu
** Matthew Suttinger & Emerald Nova (emeraldnovagames@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

#ifndef __COLLISION_H__
#define __COLLISION_H__

#include <jo/jo.h>
#include "game_object.h"
#include "ZT/ZT_COMMON.h"
#include "input.h"

//	Bounding Box
typedef struct {
	//	Center point offset from parent's postion[XYZ]
	FIXED *center;
	
	//	Corners of bounding box
	FIXED corners[8][3];
	
	//	Game Object for the bounding box
	int parent_ID;
}b_box;

FIXED r_radius[1];
bool collision_master[201];

FIXED dist(FIXED P1[XYZ], FIXED P2[XYZ]);
FIXED dist_2D(FIXED *P1, FIXED *P2);
FIXED calc_rough_radius(entity_t * ent_point);
bool is_rough_collide(FIXED P1[XYZ], FIXED P1rr, int ID);
void is_rough_collide_all(void);
void stop_collided(void);

#endif
