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
#include "collision.h"
#include <jo/jo.h>
#include "ZT/ZT_COMMON.h"

typedef struct {
	int nbPoint;
	FIXED *verts;
	int *connections;
}field_2D;

void cross_product(FIXED V1[XYZ], FIXED V2[XYZ], FIXED V3[XYZ]);
void unit_normal(FIXED *p1, FIXED *p2, FIXED *unit, FIXED *scaled_unit);
void project_1D(field_2D *field, FIXED *scaled, FIXED *extent);
//void project_1D_bbox(b_box *box, FIXED scaled[XYZ], FIXED *extent)
bool separate_2D(field_2D *field1, field_2D *field2);
//bool separate_3D_bbox(b_box *box1, b_box *box2);
bool in_list(int value, int list_len, int *list);
int find_x_extreme(field_2D *field, bool max);
int convex_hull_max_angle(field_2D *field, int ind, bool backward);
void convex_hull(field_2D * shape);

#endif
