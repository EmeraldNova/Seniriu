/*
** Seniriu
** Matthew Suttinger & Emerald Nova (emeraldnovagames@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

#ifndef __SEPARATE_H__
#define __SEPARATE_H__

#include "game_object.h"
#include <jo/jo.h>
#include "ZT/ZT_COMMON.h"

typedef struct {
	int nbPoint;
	FIXED *verts;
	int *connections;
}field_2D;

void cross_product(FIXED V1[XYZ], FIXED V2[XYZ], FIXED V3[XYZ]);
bool in_list(int value, int list_len, int *list);
int find_x_extreme(field_2D *field, bool max);
int convex_hull_max_angle(field_2D *field, int ind, bool backward);
void convex_hull(field_2D * shape);
bool separate_axis_2d(field_2D * shape_1, field_2D * shape_2);

#endif
