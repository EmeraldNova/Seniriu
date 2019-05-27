/*
** Seniriu
** Matthew Suttinger & Emerald Nova (emeraldnovagames@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

#include "collision.h"
#include <jo/jo.h>
#include "game_object.h"
#include "ZT/ZT_COMMON.h"
#include "input.h"

FIXED r_radius[1] = {0};
bool collision_master[201];

//	Calcualte Distance capped to max FIXED
FIXED dist(FIXED P1[XYZ], FIXED P2[XYZ])
{
	FIXED dx = 0;
	FIXED dy = 0;
	FIXED dz = 0;
	FIXED distf = 0;
	int dxi = 0;
	int dyi = 0;
	int dzi = 0;
	int disti = 0;
	
	//	Calculate differences along axis
	dx = JO_ABS(P1[X] - P2[X]);
	dy = JO_ABS(P1[Y] - P2[Y]);
	dz = JO_ABS(P1[Z] - P2[Z]);
	
	//	Cast to int if distance is too large for FIXED
	if(dx > (104 << 16) || dy > (104 << 16) || dz > (104 << 16))
	{
		dxi = dx >> 16;
		dyi = dy >> 16;
		dzi = dz >> 16;
		
		disti = slSquart(dxi*dxi + dyi*dyi + dzi*dzi);
	}
	else
	{
		distf = slSquartFX(slMulFX(dx,dx) + slMulFX(dy,dy) + slMulFX(dz,dz));
	}
	
	//	Sort which value to output
	if(disti > 32768)
	{
		return toFIXED(32767.9999999);
	}
	else if(disti > (distf >> 16))
	{
		return disti << 16;
	}
	else
	{
		return distf;
	}
}

//	Find (half) furthest distance between points in model
FIXED calc_rough_radius(entity_t * ent_point)
{
	int point_num = ent_point->pol[0]->nbPoint;
	
	FIXED distance = 0;
	FIXED distmax = 0;
	
	//	Loop first compare point from 0
	for(int ID1 = 0; ID1 < point_num - 1; ID1++)
	{
		//	Loop second compare point after ID1
		for(int ID2 = ID1 + 1; ID2 < point_num; ID2++)
		{
			distance = dist(ent_point->pol[0]->pntbl[ID1],
						ent_point->pol[0]->pntbl[ID2]);
			if(distance > distmax)
			{
				distmax = distance;
			}
		}
	}
	
	return slDivFX(2 << 16, distmax);
}

//	Check if rough collding with object
bool is_rough_collide(FIXED P1[XYZ], FIXED P1rr, int ID)
{
	return (dist(P1,object[ID].position) <= (P1rr + r_radius[0]));
}

//	Find (half) furthest distance between points in model
void is_rough_collide_all(void)
{
	FIXED point[XYZ];
	FIXED rr = 0 << 16;
	bool collide_hold;
	
	//	Reset collision master
	for(int ID = 0; ID < 201; ID++)
	{
		collision_master[ID] = false;
	}
	
	//	Loop first compare object from -1 (player)
	for(int ID1 = -1; ID1 < 200; ID1++)
	{
		collide_hold = false;
		
		if(ID1 < 0)
		{
			rr = 58983; //	0.9 FIXED
			point[X] = pl_position[X];
			point[Y] = pl_position[Y];
			point[Z] = pl_position[Z];
		}
		else
		{
			rr = r_radius[0];
			point[X] = object[ID1].position[X];
			point[Y] = object[ID1].position[Y];
			point[Z] = object[ID1].position[Z];
		}
		
		//	Loop second compare object after ID1
		for(int ID2 = ID1 + 1; ID2 < num_object; ID2++)
		{
			collide_hold = is_rough_collide(point, rr, ID2);
			
			if(collide_hold)
			{
				collision_master[ID1+1] = collide_hold;
				collision_master[ID2+1] = collide_hold;
			}
		}
	}
	
	return;
}

//	Stops objects that have collisions
void stop_collided(void)
{
	for(int ID = 0; ID < num_object; ID++)
	{
		if(collision_master[ID+1])
		{
			stop(ID);
		}
	}
	
	return;
}
