/*
** Seniriu
** Matthew Suttinger & Emerald Nova (emeraldnovagames@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

//	Includes
#include <jo/jo.h>
#include "ZT/ZT_COMMON.h"
#include "separate_3D.h"
#include "input.h"
#include "animate.h"
#include "display.h"
//#include "ZT/ANORM.h"

#define MAX_OBJECTS (10)

//	Structs
typedef struct {
	/*
		Game Object
		
		Game Object containing necessary items for rendering
	*/
	
	//	ID
	int ID;
	
    //  Render flag
    bool alive;
	//	Animation Number render flag (0 = default)
	int ani;

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
	
	//	Entity ID
	int entity_ID;
	
	//	Pointer to entity to render
	entity_t *entity;
	
	//	Pointer to entity to render
	XPDATA *pDataStart;
	
	//	Animation Control
	animationControl ani_con;
	
} game_object;

//  Array of game objects
extern int max_objects;
extern int num_object;
extern game_object object[MAX_OBJECTS];	//	Same as max objects
//	Pointer to copy of XPDATA 
extern XPDATA *pdataMaster[MAX_OBJECTS][MAX_MESHES];


//	Functions
void stop(int ID);
void create_object(FIXED position[XYZ], ANGLE rot[XYZ], int entity_ID);
void clone_object(int ID, int destination);
void destroy_object(int ID);
int closest_object(FIXED x, FIXED y, FIXED z, FIXED threshold);
bool is_colliding(int x, int y, int z, int x_size, int y_size, int z_size);
void apply_accel(int ID, FIXED accel[XYZ]);
void apply_accel_all(FIXED accel[XYZ]);
void update_obj_position(void);
void display_animated_model(int objdex);

#endif
