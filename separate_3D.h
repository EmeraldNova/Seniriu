/*
** Seniriu
** Matthew Suttinger & Emerald Nova (emeraldnovagames@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

#ifndef __SEPARATE_3D_H__
#define __SEPARATE_3D_H__

#include <jo/jo.h>
#include "ZT/ZT_COMMON.h"
#include "separate.h"
#include "game_object.h"

//	Bounding Box
typedef struct {
	//	Center point offset from parent's postion[XYZ]
	FIXED center[XYZ];
	
	//	Corners of bounding box
	FIXED corners[8][3];
	
	//	Half Dimensions
	FIXED half[XYZ];
	
	//	Game Object for the bounding box
	int parent_ID;
}b_box;

//	Collection of bounding boxes
extern b_box boxes[200];

//	Player bounding box
extern b_box player_bbox;

void x_product(FIXED V1[XYZ], FIXED V2[XYZ], FIXED V3[XYZ]);
void reset_corners(b_box *box);
void create_bbox(b_box *box, FIXED center[XYZ], FIXED x_dim, FIXED y_dim, FIXED z_dim, int parent_ID);
void project_1D_bbox(b_box *box, FIXED scaled[XYZ], FIXED *extent);
bool separate_3D_bbox(b_box *box1, b_box *box2);
void clone_bbox(int ID, int destination);
void destroy_bbox(int ID);
void player_bbox_init(void);
void print_corners(int ID);

#endif
