/*
** Seniriu
** Matthew Suttinger & Emerald Nova (emeraldnovagames@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include <jo/jo.h>
#include "ZT/ZT_COMMON.h"
#include "input.h"
#include "timer.h"

//  Game Object containing necessary items for rendering
typedef struct {
	//	ID
	int ID;
	
    //  Render flag
    bool alive;

    // Position
	FIXED position[XYZ];	
	// Velocity
	FIXED velocity[XYZ];
    // Rotation
	ANGLE theta[XYZ];
	// Rotation Speed
	FIXED omega[XYZ];
	//  Set Scale
	FIXED scale[XYZ];
	
	//	Pointer to entity to render
	entity_t *entity;
}game_object;

//  Array of game objects
extern int max_objects;
int num_object;
game_object object[200];	//	Same as max objects


//	Functions
void stop(int ID);
void create_object(FIXED position[XYZ], ANGLE rot[XYZ], entity_t *ent);
void clone_object(int ID, int destination);
void destroy_object(int ID);
int closest_object(FIXED x, FIXED y, FIXED z, FIXED threshold);
bool is_colliding(int x, int y, int z, int x_size, int y_size, int z_size);
void apply_accel(int ID, FIXED accel[XYZ]);
void apply_accel_all(FIXED accel[XYZ]);
void update_obj_position(void);

#endif
