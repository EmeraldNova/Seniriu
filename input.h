/*
** Seniriu
** Matthew Suttinger & Emerald Nova (johannesfetz@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

#ifndef __INPUT_H__
#define __INPUT_H__

#include <jo/jo.h>
#include "ZT/ZT_COMMON.h"
#include "game_object.h"
#include "timer.h"

//	Orientation data
extern ANGLE theta[XYZ];		//	Camera angle away from -Z
extern FIXED height;			//	Camera (player) height		
extern FIXED position[XYZ];		//	Camera Position
extern FIXED tar_dist;			//	Default interaction target distance
extern FIXED tar_sense;			//	Distance from target allow for interaction
extern FIXED target[XYZ];		//	Interaction target position

//	Repeat Input Delay
extern FIXED input_delay;
extern FIXED last_input;

//Uint8 get_time(void);
void add_rel_pos(FIXED x, FIXED y, FIXED z);
void gamepad_input(void);
void print_orientation(void);

#endif
