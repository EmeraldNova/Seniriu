/*
** Seniriu Model Tester
** Matthew Suttinger & Emerald Nova (emeraldnovagames@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

#ifndef __DISPLAY_H__
#define __DISPLAY_H__

//	Includes
#include <jo/jo.h>

//	Definitions
//	Maximum number of rooms to try to display. Not used in Model Tester
#define MAX_ROOM_DISPLAY (1)
//	Gouraud table, converts 3 8-bit entries to 15 bit color
#define    GRTBL(r,g,b)    (((b&0x1f)<<10) | ((g&0x1f)<<5) | (r&0x1f) )


//	Variables
//	(full) Field of View
extern ANGLE FOV;
//	Depth shading table to make things darker with distance
static Uint16 DepthData[32];

#endif
