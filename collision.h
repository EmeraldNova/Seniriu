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
#include "separate.h"
#include "separate_3d.h"

FIXED r_radius[1];
bool collision_master_rough[201];
bool collision_master_bbox[201];

FIXED dist(FIXED P1[XYZ], FIXED P2[XYZ]);
FIXED dist_2D(FIXED *P1, FIXED *P2);
FIXED calc_rough_radius(entity_t * ent_point);
bool is_rough_collide(FIXED P1[XYZ], FIXED P1rr, int ID);
void is_rough_collide_all(void);
bool is_bbox_collide(b_box *box1, int ID2);
void is_bbox_collide_all(void);
void stop_collided(void);

#endif
