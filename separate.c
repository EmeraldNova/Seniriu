/*
** Seniriu
** Matthew Suttinger & Emerald Nova (emeraldnovagames@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

#include "separate.h"
#include <jo/jo.h>
#include "game_object.h"
#include "ZT/ZT_COMMON.h"

//	Cross Product: V3 = V1 X V2 = - V2 X V1
void cross_product(FIXED V1[XYZ], FIXED V2[XYZ], FIXED V3[XYZ])
{
	V3[X] = slMulFX(V1[Y],V2[Z]) - slMulFX(V1[Z], V2[Y]);
    V3[Y] = slMulFX(V1[X],V2[Z]) - slMulFX(V1[Z], V2[X]);
    V3[Z] = slMulFX(V1[X],V2[Y]) - slMulFX(V1[Y], V2[X]);
}

//	Check if value is already in list
bool in_list(int value, int list_len, int *list)
{
	for(int i = 0; i < list_len; i++)
	{
		if(list[i] == value)
			return true;
	}
	
	return false;
}

//	Finds index of min or max x value coordinate
int find_x_extreme(field_2D *field, bool max)
{
	FIXED extreme = -2147483648;
	int signmod = 1;
	int exdex = 0;
	if(!max)
	{
		extreme = -extreme;
		signmod = -1;
	}
	
	for(int i = 0; i < field->nbPoint; i++)
	{
		if(signmod*(*(field->verts + 2*i)) > signmod*extreme)
		{
			exdex = i;
			extreme = *(field->verts + 2*i);
		}
	}
	
	return exdex;
}

//	Finds point that forms maximum forward angle not already connected, starting from ind
int convex_hull_max_angle(field_2D *shape, int ind, bool backward)
{
	int ID = ind;
	FIXED dx;
	FIXED dy;
	ANGLE theta_current;
	ANGLE theta_adj = 32768;	//	angle + 180 to prevent negative wrap around
	ANGLE theta_record;
	
	if(backward)
	{
		theta_adj = 0;
	}
	theta_record = DEGtoANG(0); 
	
	bool check1;
	bool check2;
	bool check3;
	
	for(int j = 0; j < shape->nbPoint; j++)
	{
		dx = *(shape->verts +2*j) - *(shape->verts +2*ind);
		check1 = !in_list(j, shape->nbPoint, shape->connections);
		check2 = (dx >= 0);
		check3 = !(j == ind);
		
		if(backward)
		{
			check1 = (!in_list(j, shape->nbPoint, shape->connections)) || (j == shape->connections[0]);
			check2 = (dx <= 0);
		}
		
		if(check1 && check2 && check3)
		{
			dy = *(shape->verts +2*j + 1) - *(shape->verts +2*ind + 1);
			theta_current = slAtan(dx,dy) + theta_adj;
			
			if(slAng2FX(theta_current) > slAng2FX(theta_record))
			{
				ID = j;
				theta_record = theta_current;
			}
		}
	}
	return ID;
}

//	Maps Convex Hull and culls extra points
void convex_hull(field_2D *shape)
{
	//	Find min and max x points
	int mindex = find_x_extreme(shape, false);
	int maxdex = find_x_extreme(shape, true);
	int i;
	
	//	Wipe connections and set initial
	for(i = 0; i < shape->nbPoint; i++)
	{
		shape->connections[i] = mindex;
	}

	//	Low to high (top hull)
	for(i = 1; i < shape->nbPoint; i++)
	{
		shape->connections[i] = convex_hull_max_angle(shape,shape->connections[i-1],false);
		
		if(shape->connections[i] == maxdex)
		{
			break;
		}
	}
	
	//	High to low (bottom hull)
	while((shape->connections[i] != mindex) && i <= shape->nbPoint)
	{
		i++;
		shape->connections[i] = convex_hull_max_angle(shape,shape->connections[i-1],true);
		if(shape->connections[i] == mindex)
		{
			break;
		}
	}
	
	return;
}

//	2D Separate Axis Collision Check
bool separate_axis_2d(field_2D * shape_1, field_2D * shape_2)
{
	bool colliding = false;
	
	
	
	return colliding;
}

