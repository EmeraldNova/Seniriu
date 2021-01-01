/*
** Seniriu
** Matthew Suttinger & Emerald Nova (emeraldnovagames@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

#ifndef __INPUT_H__
#define __INPUT_H__

#include <jo/jo.h>
#include "ZT/ZT_COMMON.h"
#include "game_object.h"
#include "room.h"
#include "character.h"

//	Orientation data
//extern ANGLE theta[XYZ];		//	Camera angle away from -Z
extern FIXED height;			//	Camera (player) height		
extern FIXED pl_position[XYZ];	//	Camera Position
extern FIXED tar_dist;			//	Default interaction target distance
extern FIXED tar_sense;			//	Distance from target allow for interaction
extern FIXED target[XYZ];		//	Interaction target position

//	Camera (player) height
extern FIXED cam_default_height;
extern FIXED cam_height;

//	Player movement
extern FIXED move_inc;

//	Repeat Input Delay
extern FIXED input_delay;
extern FIXED last_input;

//Uint8 get_time(void);
extern Sint8 SynchConst;

//	Functions
void add_rel(FIXED add[XYZ], ANGLE theta[XYZ], FIXED store[XYZ]);
void gamepad_input(void);
void print_orientation(void);

#endif
