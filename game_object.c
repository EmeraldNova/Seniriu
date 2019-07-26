/*
** Seniriu
** Matthew Suttinger & Emerald Nova (emeraldnovagames@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

#include "game_object.h"
#include <jo/jo.h>
#include "ZT/ZT_COMMON.h"
#include "input.h"
#include "timer.h"
#include "collision.h"

//  Array of game objects
int max_objects = 200;
int num_object = 0;
game_object object[200];	//	Same as max objects

//	Stops game object
void stop(int ID)
{
	object[ID].velocity[X] = 0;
	object[ID].velocity[Y] = 0;
	object[ID].velocity[Z] = 0;
}

//	Creates a game_object and add it to the objects array
void create_object(FIXED position[XYZ], ANGLE rot[XYZ], entity_t *ent)
{
	if(num_object < max_objects)
	{
		//	Increment object number
		num_object++;
		
		//	ID
		object[num_object-1].ID = num_object-1;
		
		//  Set alive
		object[num_object-1].alive = true;

		//  Set Position
		object[num_object-1].position[X] = position[X];
		object[num_object-1].position[Y] = position[Y];
		object[num_object-1].position[Z] = position[Z];
		
		//  Set Velocity
		object[num_object-1].velocity[X] = 0;
		object[num_object-1].velocity[Y] = 0;
		object[num_object-1].velocity[Z] = 0;

		//  Set Rotation
		object[num_object-1].theta[X] = rot[X];
		object[num_object-1].theta[Y] = rot[Y];
		object[num_object-1].theta[Z] = rot[Z];
		
		//  Set Rotation Speed
		object[num_object-1].omega[X] = 0;
		object[num_object-1].omega[Y] = 0;
		object[num_object-1].omega[Z] = 0;
		
		//  Set Scale
		object[num_object-1].scale[X] = 1 << 16;
		object[num_object-1].scale[Y] = 1 << 16;
		object[num_object-1].scale[Z] = 1 << 16;
		
		//	Point to entity_t
		object[num_object-1].entity = ent;
	}
}

//	Clone Object from ID to destination ID in object array
void clone_object(int ID, int destination)
{
	//  Set Position
	object[destination].position[X] = object[ID].position[X];
	object[destination].position[Y] = object[ID].position[Y];
	object[destination].position[Z] = object[ID].position[Z];
	
	//  Set Velocity
    object[destination].velocity[X] = object[ID].velocity[X];
	object[destination].velocity[Y] = object[ID].velocity[Y];
	object[destination].velocity[Z] = object[ID].velocity[Z];

    //  Set Rotation
    object[destination].theta[X] = object[ID].theta[X];
	object[destination].theta[Y] = object[ID].theta[Y];
	object[destination].theta[Z] = object[ID].theta[Z];
	
	//  Set Rotation Speed
    object[destination].omega[X] = object[ID].omega[X];
	object[destination].omega[Y] = object[ID].omega[Y];
	object[destination].omega[Z] = object[ID].omega[Z];
	
	//	Set Entity
	object[destination].entity = object[ID].entity;
}

//	Destroy Object
void destroy_object(int ID)
{
	//	Return if nonsensical ID
	if(ID >= num_object || ID < 0 || num_object <= 0)
	{
		return;
	}
	else
	{
		// Clone last object into destroyed object's lcoation
		clone_object(num_object-1, ID);
		
		// Resize objects
		num_object--;
	}
}

//	Find closest object and return object ID
int closest_object(FIXED x, FIXED y, FIXED z, FIXED threshold)
{
	int closest_ID = -1;
	FIXED dist;
	FIXED max_dist = threshold;
	for(int index = 0; index < num_object; index++)
	{
		dist = slSquartFX(
			slMulFX((object[index].position[X] - x),(object[index].position[X] - x)) +
			slMulFX((object[index].position[Y] - y),(object[index].position[Y] - y)) +
			slMulFX((object[index].position[Z] - z),(object[index].position[Z] - z)));
		if(dist < max_dist)
		{
			closest_ID = index;
			max_dist = dist;
		}
	}
	
	return closest_ID;
}

//	Applies an acceleration on an object
void apply_accel(int ID, FIXED accel[XYZ])
{	
	//	Change velocities
	object[ID].velocity[X] = object[ID].velocity[X] + slMulFX(dt,accel[X]);
	object[ID].velocity[Z] = object[ID].velocity[Z] + slMulFX(dt,accel[Z]);
	
	//	Remove y acceleration if on floor
	if(object[ID].position[Y] < 0)
	{
		object[ID].velocity[Y] = object[ID].velocity[Y] + slMulFX(dt,accel[Y]);
	}
	
	return;
}

//	Applies an acceleration to all objects
void apply_accel_all(FIXED accel[XYZ])
{
	for(int ID = 0; ID < num_object; ID++)
	{
		apply_accel(ID, accel);	
	}
	
	return;
}

//	Update all obejct positiosn based on velocities
void update_obj_position(void)
{
	for(int ID = 0; ID < num_object; ID++)
	{
		//	Update object position
		object[ID].position[X] = object[ID].position[X] + slMulFX(dt,object[ID].velocity[X]);
		object[ID].position[Y] = object[ID].position[Y] + slMulFX(dt,object[ID].velocity[Y]);
		object[ID].position[Z] = object[ID].position[Z] + slMulFX(dt,object[ID].velocity[Z]);

		//	Floor is 0
		if(object[ID].position[Y] >= 0)
		{
			object[ID].position[Y] = 0;
			object[ID].velocity[Y] = 0;
		}
	}
	
	return;
}

/*
//	Basic bounding box colission
bool is_colliding(int x, int y, int z, int x_size, int y_size, int z_size)
{
	int index;
	int x_collide = false;
	int y_collide = false;
	int z_collide = false;

	for(index = 0; index < num_object; index++)
	{
		//	Reset values
		x_collide = false;
		y_collide = false;
		z_collide = false;
		
		//	Check x
		if(object[index].sx >= 0 && (JO_ABS(x-object[index].x) < (object[index].x_size*object[index].sx + x_size)/2))
		{
			x_collide = true;
		}
		
		//	Check y
		if(object[index].sy >= 0 && (JO_ABS(y-object[index].y) < (object[index].y_size*object[index].sy + y_size)/2))
		{
			y_collide = true;
		}
		
		//	Check z
		if(object[index].sz >= 0 && (JO_ABS(z-object[index].z) < (object[index].z_size*object[index].sz + z_size)/2))
		{
			z_collide = true;
		}
		
		jo_printf(0, 2, "Collisions:\t%d\t%d\t%d ",(int) x_collide, (int) y_collide, (int) z_collide);
		
		//	Return if colliding
		if(x_collide && y_collide && z_collide)
		{
			return true;
		}
	}
	return false;
}
*/

