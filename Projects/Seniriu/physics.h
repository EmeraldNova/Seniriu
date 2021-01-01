/*
** Seniriu
** Matthew Suttinger & Emerald Nova (emeraldnovagames@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

#ifndef __PHYSICS_H__
#define __PHYSICS_H__

//	Include
#include <jo/jo.h>

//	Define
#define	NUM_PLANES (36)

//	Structs
typedef struct {
	/*
		Plane
		
		Defines Finite Plane
	*/
	
	//	Vertices
	FIXED v1[XYZ];
	FIXED v2[XYZ];
	FIXED v3[XYZ];
	FIXED v4[XYZ];
	
	//	Normal
	FIXED n[XYZ];
	
	//	Plane Polynomial Description
	//	dot_product(normal, XYZ) + polyn_off = 0
	FIXED polyn_off;
} plane;

//	Variables
extern FIXED gravity[XYZ];
extern plane plane_list[NUM_PLANES];

//	Functions
FIXED dot_product(FIXED a[XYZ], FIXED b[XYZ]);
void cross_product(FIXED a[XYZ], FIXED b[XYZ], FIXED n[XYZ]);
void generate_plane_info(plane * p);
bool plane_intersect_check(FIXED pos[XYZ], FIXED next[XYZ], plane * p, FIXED proj[XYZ], FIXED * dt_collide);
FIXED project_to_plane(FIXED pos[XYZ], FIXED vel[XYZ], plane * p, FIXED proj[XYZ]);
void init_planes(void);

#endif
