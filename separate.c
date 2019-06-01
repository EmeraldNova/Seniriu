/*
** Seniriu
** Matthew Suttinger & Emerald Nova (emeraldnovagames@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

#include "separate.h"
#include "collision.h"
#include "game_object.h"
#include <jo/jo.h>
#include "ZT/ZT_COMMON.h"

//	Cross Product: V3 = V1 X V2 = - V2 X V1
void cross_product(FIXED V1[XYZ], FIXED V2[XYZ], FIXED V3[XYZ])
{
	V3[X] = slMulFX(V1[Y],V2[Z]) - slMulFX(V1[Z], V2[Y]);
    V3[Y] = -slMulFX(V1[X],V2[Z]) + slMulFX(V1[Z], V2[X]);
    V3[Z] = slMulFX(V1[X],V2[Y]) - slMulFX(V1[Y], V2[X]);
}

//	Construct unit normal vector from two points
void unit_normal(FIXED *p1, FIXED *p2, FIXED *unit, FIXED *scaled_unit)
{
	FIXED dx = *(p2) - *(p1);
	FIXED dy = *(p2 + 1) - *(p1 + 1);
	FIXED mag = dist_2D(p1, p2);
	
	//	Construct a true unit vector
	*(unit) = slDivFX(mag, -dy);
	*(unit + 1) = slDivFX(mag, dx);
	
	//	Construct a scaled (1/1.5, 1.5 == 98304) unit vector
	//	Required in case projection is very high in both x and y
	//	Worst case, SQRT(2) * Max FIXED
	*(scaled_unit) = slDivFX(98304, *(unit));
	*(scaled_unit + 1) = slDivFX(98304, *(unit + 1));
}

//	Projecting 2D points onto line defined by scaled normal
void project_1D(field_2D *field, FIXED *scaled, FIXED *extent)
{
	FIXED value;
	
	if(field->nbPoint < 1)
	{
		//	No points, 0 extent
		*(extent) = 0;
		*(extent + 1) = 0;
		return;
	}
	
	//	First point, single point extent
	*(extent) = slMulFX(*(scaled),*(field->verts)) +
				slMulFX(*(scaled + 1),*(field->verts + 1));
	*(extent + 1) = *(extent);
	
	if(field->nbPoint < 2)
	{
		//	1 point, single point extent
		return;
	}
	
	//	Continue to add points to extent
	for(int i = 1; i < field->nbPoint; i++)
	{
		value = slMulFX(*(scaled),*(field->verts + 2*i)) +
				slMulFX(*(scaled + 1),*(field->verts + 1 + 2*i));
				
		//	Check for max or min
		if(value < *(extent))
		{
			//	Write min
			*(extent) = value;
		}
		else if(value > *(extent + 1))
		{
			*(extent + 1) = value;
		}
	}
}

//	Checks for separating axi from two field_2D's. True if colliding.
bool separate_2D(field_2D *field1, field_2D *field2)
{
	int num_line_1 = 0;
	int num_line_2 = 0;
	int start_con_1 = *(field1->connections);
	int start_con_2 = *(field2->connections);
	
	FIXED normal[2];
	FIXED scaled[2];
	FIXED extent1[2];
	FIXED extent2[2];
	
	//	Find out how many unique line segments form a convex hull for each field
	for(int i = 1; i < field1->nbPoint; i++)
	{
		if((field1->nbPoint < 2) || (field1->connections[i] == field1->connections[i-1]))
		{
			break;
		}
		num_line_1 = i;
		if(start_con_1 == field1->connections[i])
		{
			break;
		}
	}
	for(int i = 1; i < field2->nbPoint; i++)
	{
		if((field2->nbPoint < 2) || (field2->connections[i] == field2->connections[i-1]))
		{
			break;
		}
		num_line_2 = i;
		if(start_con_2 == field2->connections[i])
		{
			break;
		}
	}
	
	
	
	//	Loop through each line segment on field 1
	for(int i = 0; i <= num_line_1; i++)
	{
		//	 Get normal
		if(i < num_line_1)
		{
			unit_normal((field1->verts + 2 * *(field1->connections + i)),
				(field1->verts + 2 * *(field1->connections + (i+1))), 
				normal, scaled);
		}
		else{
			unit_normal((field1->verts + 2 * *(field1->connections + num_line_1)),
				(field1->verts + 2 * *(field1->connections)), 
				normal, scaled);
		}
		
		//	Calculate field extents projected onto normal
		project_1D(field1, scaled, extent1);
		project_1D(field2, scaled, extent2);
		
		//	Compare extents for overlap
		if(*(extent1) > *(extent2 + 1))
		{
			//	Minimum of field 1 > max of field 2, no collision
			return false;
		}
		if(*(extent2) > *(extent1 + 1))
		{
			//	Minimum of field 2 > max of field 1, no collision
			return false;
		}
	}
	
	//	Loop through each line segment on field 2
	for(int i = 0; i <= num_line_2; i++)
	{
		//	 Get normal
		if(i < num_line_2)
		{
			unit_normal((field2->verts + 2 * *(field2->connections + i)),
				(field2->verts + 2 * *(field2->connections + (i+1))), 
				normal, scaled);
		}
		else{
			unit_normal((field2->verts + 2 * *(field2->connections + num_line_2)),
				(field2->verts + 2 * *(field2->connections)), 
				normal, scaled);
		}
		
		//	Calculate field extents projected onto normal
		project_1D(field1, scaled, extent1);
		project_1D(field2, scaled, extent2);
		
		//	Compare extents for overlap
		if(*(extent1) > *(extent2 + 1))
		{
			//	Minimum of field 1 > max of field 2, no collision
			return false;
		}
		if(*(extent2) > *(extent1 + 1))
		{
			//	Minimum of field 2 > max of field 1, no collision
			return false;
		}
	}
	
	//	No separating axis, collision is true
	return true;
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
	FIXED extreme = -2147483648;			//	Fixed max
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
	ANGLE theta_adj = 32768;	//	angle + 180 to prevent negative wrap around for dy < 0
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
	bool backward = false;
	for(i = 1; i < shape->nbPoint; i++)
	{
		shape->connections[i] = convex_hull_max_angle(shape,shape->connections[i-1],backward);
		
		if(shape->connections[i] == maxdex)
		{
			backward = true;
		}
	/*}
	
	//	High to low (bottom hull)
	for(i = i; i < shape->nbPoint; i++)
	{
		shape->connections[i] = convex_hull_max_angle(shape,shape->connections[i-1],true);
	*/
		if(shape->connections[i] == shape->connections[0])
		{
			break;
		}
	}
	
	return;
}

