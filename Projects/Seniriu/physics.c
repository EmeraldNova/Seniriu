/*
** Seniriu
** Matthew Suttinger & Emerald Nova (emeraldnovagames@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

//	Inlcude
#include "physics.h"

//	Variables
FIXED gravity[XYZ] = {0, toFIXED(9.8), 0};
plane plane_list[NUM_PLANES];

//	Functions
FIXED dot_product(FIXED a[XYZ], FIXED b[XYZ])
{
	/*
		Dot Product
		
		Calculates dot product of a and b and returns FIXED result
	*/
	
	return jo_fixed_mult(a[X], b[X]) + jo_fixed_mult(a[Y], b[Y]) + jo_fixed_mult(a[Z], b[Z]);
}


void cross_product(FIXED a[XYZ], FIXED b[XYZ], FIXED n[XYZ])
{
	/*
		Cross Product
		
		Claculates a X b = n, and writes reuslt into n;
	*/
	
	n[X] = jo_fixed_mult(a[Y], b[Z]) - jo_fixed_mult(a[Z], b[Y]);
	n[Y] = jo_fixed_mult(a[Z], b[X]) - jo_fixed_mult(a[X], b[Z]);
	n[Z] = jo_fixed_mult(a[X], b[Y]) - jo_fixed_mult(a[Y], b[X]);
	
	return;
}


void generate_plane_info(plane * p)
{
	/*
		Generate Plane Information
		
		Calculates plane normal and polynomial offset and writes into plane struct.
	*/
	
	//	Cross product components
	FIXED v41[XYZ];
	FIXED v21[XYZ];
	
	//	Calculate differences
	v41[X] = p->v4[X] - p->v1[X];
	v41[Y] = p->v4[Y] - p->v1[Y];
	v41[Z] = p->v4[Z] - p->v1[Z];
	v21[X] = p->v2[X] - p->v1[X];
	v21[Y] = p->v2[Y] - p->v1[Y];
	v21[Z] = p->v2[Z] - p->v1[Z];
	
	//	Fix triangles
	if( v41[X] == 0 && v41[Y] == 0 && v41[Z] == 0 )
	{
		v41[X] = p->v3[X] - p->v1[X];
		v41[Y] = p->v3[Y] - p->v1[Y];
		v41[Z] = p->v3[Z] - p->v1[Z];
	}
	else if( v21[X] == 0 && v21[Y] == 0 && v21[Z] == 0 )
	{
		v21[X] = p->v3[X] - p->v1[X];
		v21[Y] = p->v3[Y] - p->v1[Y];
		v21[Z] = p->v3[Z] - p->v1[Z];
	}
	
	//	Calculate cross product
	cross_product(v41, v21, p->n);
	FIXED divisor = slSquartFX(jo_fixed_mult(p->n[X],p->n[X]) + jo_fixed_mult(p->n[Y],p->n[Y]) + jo_fixed_mult(p->n[Z],p->n[Z]));
	p->n[X] = jo_fixed_div(p->n[X], divisor);
	p->n[Y] = jo_fixed_div(p->n[Y], divisor);
	p->n[Z] = jo_fixed_div(p->n[Z], divisor);
	
	//	Calculate polynomial offset
	p->polyn_off = -dot_product(p->n, p->v1);
	
	return;
}


bool plane_intersect_check(FIXED pos[XYZ], FIXED next[XYZ], plane * p, FIXED proj[XYZ], FIXED * dt_collide)
{
	/*
		Plane Intersect Check
		
		Checks if current position (pos) and next lead a path through the finite plane. Will write the intersection poitn and time to proj and dt_collide
		
		Of note, if you are on a plane, this will only return true if you are trying to travel agaisnt the normal, effectively making plane based collision one-way to allow for jumping/leaving the plane 
	*/
	
	//	Calculate relative position
	FIXED rel_pos[XYZ];
	FIXED rel_pos_next[XYZ];
	
	rel_pos[X] = pos[X] - p->v1[X];
	rel_pos[Y] = pos[Y] - p->v1[Y];
	rel_pos[Z] = pos[Z] - p->v1[Z];
	rel_pos_next[X] = next[X] - p->v1[X];
	rel_pos_next[Y] = next[Y] - p->v1[Y];
	rel_pos_next[Z] = next[Z] - p->v1[Z];
	
	//	Find sign of product with normal
	FIXED rel_prod = dot_product(rel_pos, p->n);
	FIXED rel_prod_next = dot_product(rel_pos_next, p->n);
	
	//	Check for sign switch, assuming neither point is on plane
	//slPrint("                                        ",slLocate(0,10));
	if( (JO_ABS(rel_prod) > (1<<9)) && 
		(JO_ABS(rel_prod_next) > (1<<9)) && 
		((rel_prod & 1<<31) == (rel_prod_next & 1<<31)) )
	{
		//slPrint("Sign Switch",slLocate(0,10));
		return false;
	}
	//	Object crosses plane or one point is on plane. 
	
	//	Check for current on plane and next against normal (making contact and attempting to leave)
	if( (JO_ABS(rel_prod) < (1<<9)) && 
		((rel_prod_next & 1<<31)>>31 == 0) && 
		(JO_ABS(rel_prod_next) > (1<<9)) )
	{
		//slPrint("Next Point",slLocate(13,10));
		return false;
	}
	
	//	Test if inside finite plane
	//	Find planar intersection
	FIXED dummy_vel[XYZ] = {next[X] - pos[X], next[Y] - pos[Y], next[Z] - pos[Z]};
	*dt_collide = project_to_plane(pos, dummy_vel, p, proj);
	
	//	Construct edges
	FIXED e1[XYZ] = {p->v2[X] - p->v1[X], p->v2[Y] - p->v1[Y], p->v2[Z] - p->v1[Z]};
	FIXED e2[XYZ] = {p->v3[X] - p->v2[X], p->v3[Y] - p->v2[Y], p->v3[Z] - p->v2[Z]};
	FIXED e3[XYZ] = {p->v4[X] - p->v3[X], p->v4[Y] - p->v3[Y], p->v4[Z] - p->v3[Z]};
	FIXED e4[XYZ] = {p->v1[X] - p->v4[X], p->v1[Y] - p->v4[Y], p->v1[Z] - p->v4[Z]};
	
	//	Construct displacement
	FIXED d1[XYZ] = {proj[X] - p->v1[X], proj[Y] - p->v1[Y], proj[Z] - p->v1[Z]};
	FIXED d2[XYZ] = {proj[X] - p->v2[X], proj[Y] - p->v2[Y], proj[Z] - p->v2[Z]};
	FIXED d3[XYZ] = {proj[X] - p->v3[X], proj[Y] - p->v3[Y], proj[Z] - p->v3[Z]};
	FIXED d4[XYZ] = {proj[X] - p->v4[X], proj[Y] - p->v4[Y], proj[Z] - p->v4[Z]};
	
	//	Calculate normals constructed from displacement and edges to determine
	FIXED m1[XYZ];
	FIXED m2[XYZ];
	FIXED m3[XYZ];
	FIXED m4[XYZ];
	cross_product(e1, d1, m1);
	cross_product(e2, d2, m2);
	cross_product(e3, d3, m3);
	cross_product(e4, d4, m4);
	
	//	Calculate dot product signs
	FIXED s1 = dot_product(m1, m2) & 1<<31;
	FIXED s2 = dot_product(m1, m3) & 1<<31;
	FIXED s3 = dot_product(m1, m4) & 1<<31;
	FIXED s4 = dot_product(m4, m1) & 1<<31;
	
	//	One of the signs disagree, meaning the point is outside the plane
	if(s1 != s2 || s1 != s3 || s1 != s4)
	{
		//slPrint("Outside Plane",slLocate(26,10));
		return false;
	}
	
	//	Either there is a crossing, the next point is on the plane, or the current point is on the plane and would attempt to cross on the next point. Return true
	return true;
}


FIXED project_to_plane(FIXED pos[XYZ], FIXED vel[XYZ], plane * p, FIXED proj[XYZ])
{
	/*
		Project to Plane
		
		Takes position and velocity and projects position where plane intersects. Returns necessary time to travel at velocity to hit plane.
	*/
	
	//	Calculate divisor
	FIXED dividend = (-1)*(dot_product(pos, p->n) + (p->polyn_off));
	FIXED divisor = dot_product(vel, p->n);
	
	//	Calculate time to intersect plane
	FIXED time_to_inter = slDivFX(divisor, dividend);
	//	Ensure positive time within frame
	time_to_inter = JO_MIN(JO_MAX(0, JO_ABS(time_to_inter)), delta_time);
	
	//	Print
	//slPrint("  Divisor      Dividend    T-t-Intercept",slLocate(0,11));
	//slPrintFX(divisor,slLocate(0,12));
	//slPrintFX(dividend,slLocate(13,12));
	//slPrintFX(time_to_inter,slLocate(26,12));
	
	//	Write projected coordinates
	proj[X] = pos[X] + jo_fixed_mult(time_to_inter, vel[X]);
	proj[Y] = pos[Y] + jo_fixed_mult(time_to_inter, vel[Y]);
	proj[Z] = pos[Z] + jo_fixed_mult(time_to_inter, vel[Z]);
	
	return time_to_inter;
}

void init_planes(void)
{
	/*
		Initialize Planes
		
		Set up planes for use in rooms
	*/
	
	int index = 0;
	FIXED floo_ext = 6<<16;
	FIXED ceil_ext = toFIXED(1.5);
	FIXED door_ext = toFIXED(0.5);
	FIXED wall_ext = toFIXED(3.4);
	/*
	//	Floor	0
	plane_list[index].v1[X] =  floo_ext;
	plane_list[index].v1[Y] =  0;
	plane_list[index].v1[Z] = -floo_ext;
	plane_list[index].v2[X] =  floo_ext;
	plane_list[index].v2[Y] = -0;
	plane_list[index].v2[Z] =  floo_ext;
	plane_list[index].v3[X] = -floo_ext;
	plane_list[index].v3[Y] = -0;
	plane_list[index].v3[Z] =  floo_ext;
	plane_list[index].v4[X] = -floo_ext;
	plane_list[index].v4[Y] =  0;
	plane_list[index].v4[Z] = -floo_ext;
	generate_plane_info(&plane_list[index]);
	index++;
	
	//	Ceiling	1
	plane_list[index].v1[X] =  floo_ext;
	plane_list[index].v1[Y] = -ceil_ext;
	plane_list[index].v1[Z] = -floo_ext;
	plane_list[index].v2[X] =  floo_ext;
	plane_list[index].v2[Y] = -ceil_ext;
	plane_list[index].v2[Z] =  floo_ext;
	plane_list[index].v3[X] = -floo_ext;
	plane_list[index].v3[Y] = -ceil_ext;
	plane_list[index].v3[Z] =  floo_ext;
	plane_list[index].v4[X] = -floo_ext;
	plane_list[index].v4[Y] = -ceil_ext;
	plane_list[index].v4[Z] = -floo_ext;
	generate_plane_info(&plane_list[index]);
	index++;
	
	//	North Solid Wall	2
	plane_list[index].v1[X] =  floo_ext;
	plane_list[index].v1[Y] =  toFIXED(1.0);
	plane_list[index].v1[Z] =  wall_ext;
	plane_list[index].v2[X] =  floo_ext;
	plane_list[index].v2[Y] = -toFIXED(4.0);
	plane_list[index].v2[Z] =  wall_ext;
	plane_list[index].v3[X] = -floo_ext;
	plane_list[index].v3[Y] = -toFIXED(4.0);
	plane_list[index].v3[Z] =  wall_ext;
	plane_list[index].v4[X] = -floo_ext;
	plane_list[index].v4[Y] =  toFIXED(1.0);
	plane_list[index].v4[Z] =  wall_ext;
	generate_plane_info(&plane_list[index]);
	index++;
	
	//	North Right Wall With Doorway	3
	plane_list[index].v1[X] =  floo_ext;
	plane_list[index].v1[Y] =  toFIXED(1.0);
	plane_list[index].v1[Z] =  wall_ext;
	plane_list[index].v2[X] =  floo_ext;
	plane_list[index].v2[Y] = -toFIXED(4.0);
	plane_list[index].v2[Z] =  wall_ext;
	plane_list[index].v3[X] =  door_ext;
	plane_list[index].v3[Y] = -toFIXED(4.0);
	plane_list[index].v3[Z] =  wall_ext;
	plane_list[index].v4[X] =  door_ext;
	plane_list[index].v4[Y] =  toFIXED(1.0);
	plane_list[index].v4[Z] =  wall_ext;
	generate_plane_info(&plane_list[index]);
	index++;
	
	//	North Left Wall With Doorway	4
	plane_list[index].v1[X] = -door_ext;
	plane_list[index].v1[Y] =  toFIXED(1.0);
	plane_list[index].v1[Z] =  wall_ext;
	plane_list[index].v2[X] = -door_ext;
	plane_list[index].v2[Y] = -toFIXED(4.0);
	plane_list[index].v2[Z] =  wall_ext;
	plane_list[index].v3[X] = -floo_ext;
	plane_list[index].v3[Y] = -toFIXED(4.0);
	plane_list[index].v3[Z] =  wall_ext;
	plane_list[index].v4[X] = -floo_ext;
	plane_list[index].v4[Y] =  toFIXED(1.0);
	plane_list[index].v4[Z] =  wall_ext;
	generate_plane_info(&plane_list[index]);
	index++;
	
	//	North Doorway Right Side	5
	plane_list[index].v1[X] =  door_ext;
	plane_list[index].v1[Y] =  toFIXED(1.0);
	plane_list[index].v1[Z] =  wall_ext;
	plane_list[index].v2[X] =  door_ext;
	plane_list[index].v2[Y] = -toFIXED(4.0);
	plane_list[index].v2[Z] =  wall_ext;
	plane_list[index].v3[X] =  door_ext;
	plane_list[index].v3[Y] = -toFIXED(4.0);
	plane_list[index].v3[Z] =  floo_ext;
	plane_list[index].v4[X] =  door_ext;
	plane_list[index].v4[Y] =  toFIXED(1.0);
	plane_list[index].v4[Z] =  floo_ext;
	generate_plane_info(&plane_list[index]);
	index++;
	
	//	North Doorway Left Side	6
	plane_list[index].v1[X] = -door_ext;
	plane_list[index].v1[Y] =  toFIXED(1.0);
	plane_list[index].v1[Z] =  floo_ext;
	plane_list[index].v2[X] = -door_ext;
	plane_list[index].v2[Y] = -toFIXED(4.0);
	plane_list[index].v2[Z] =  floo_ext;
	plane_list[index].v3[X] = -door_ext;
	plane_list[index].v3[Y] = -toFIXED(4.0);
	plane_list[index].v3[Z] =  wall_ext;
	plane_list[index].v4[X] = -door_ext;
	plane_list[index].v4[Y] =  toFIXED(1.0);
	plane_list[index].v4[Z] =  wall_ext;
	generate_plane_info(&plane_list[index]);
	index++;
	*/
	
	return;
}

