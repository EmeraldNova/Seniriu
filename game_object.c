/*
** Seniriu
** Matthew Suttinger & Emerald Nova (emeraldnovagames@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

#include "game_object.h"

//  Array of game objects
int max_objects = MAX_OBJECTS;
int num_object = 0;
game_object object[MAX_OBJECTS];	//	Same as max objects
XPDATA *pdataMaster[MAX_OBJECTS][MAX_MESHES];

//	Stops game object
void stop(int ID)
{
	object[ID].velocity[X] = 0;
	object[ID].velocity[Y] = 0;
	object[ID].velocity[Z] = 0;
}

//	Creates a game_object and add it to the objects array
void create_object(FIXED position[XYZ], ANGLE rot[XYZ], int entity_ID)
{
	if(num_object < max_objects)
	{
		//	ID
		object[num_object].ID = num_object;
		
		//  Set alive
		object[num_object].alive = true;
		
		//	Set default animation
		object[num_object].ani = 0;

		//  Set Position
		object[num_object].position[X] = position[X];
		object[num_object].position[Y] = position[Y];
		object[num_object].position[Z] = position[Z];
		
		//  Set Velocity
		object[num_object].velocity[X] = 0;
		object[num_object].velocity[Y] = 0;
		object[num_object].velocity[Z] = 0;

		//  Set Rotation
		object[num_object].theta[X] = rot[X];
		object[num_object].theta[Y] = rot[Y];
		object[num_object].theta[Z] = rot[Z];
		
		//  Set Rotation Speed
		object[num_object].omega[X] = 0;
		object[num_object].omega[Y] = 0;
		object[num_object].omega[Z] = 0;
		
		//  Set Scale
		object[num_object].scale[X] = 1 << 16;
		object[num_object].scale[Y] = 1 << 16;
		object[num_object].scale[Z] = 1 << 16;
		
		//	Point to entity_t
		object[num_object].entity_ID = entity_ID;
		object[num_object].entity = &entities[entity_ID];
		
		//	Copy pData
		pdataMaster[object[num_object].ID][0] = jo_malloc((int)pDataSizes[entity_ID]);
		slDMACopy((XPDATA*)entities[entity_ID].pol[0],
					pdataMaster[object[num_object].ID][0],
					pDataSizes[entity_ID]);
		object[num_object].pDataStart = pdataMaster[object[num_object].ID][0];
		
		//	Assign bounding box
		VECTOR center = {0, 0, 0};
		create_bbox(&boxes[object[num_object].ID], center,
				1 << 16, 1 << 16, 1 << 16, object[num_object].ID);
				
		//	Increment object number
		num_object++;
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
	
	//	Clone Bounding Box
	clone_object(ID, destination);
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
		//	Destroy pData copy
		jo_free(pdataMaster[ID][0]);
		
		// Destroy old bounding box
		destroy_bbox(ID);
		
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
	object[ID].velocity[X] = object[ID].velocity[X] + slMulFX(delta_time,accel[X]);
	object[ID].velocity[Z] = object[ID].velocity[Z] + slMulFX(delta_time,accel[Z]);
	
	//	Remove y acceleration if on floor
	if(object[ID].position[Y] < 0)
	{
		object[ID].velocity[Y] = object[ID].velocity[Y] + slMulFX(delta_time,accel[Y]);
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
		object[ID].position[X] = object[ID].position[X] + slMulFX(delta_time,object[ID].velocity[X]);
		object[ID].position[Y] = object[ID].position[Y] + slMulFX(delta_time,object[ID].velocity[Y]);
		object[ID].position[Z] = object[ID].position[Z] + slMulFX(delta_time,object[ID].velocity[Z]);

		//	Floor is 0
		if(object[ID].position[Y] >= 0)
		{
			object[ID].position[Y] = 0;
			object[ID].velocity[Y] = 0;
		}
	}
	
	return;
}



