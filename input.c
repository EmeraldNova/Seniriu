/*
** Seniriu
** Matthew Suttinger & Emerald Nova (emeraldnovagames@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

#include "input.h"
#include <jo/jo.h>
#include "ZT/ZT_COMMON.h"
#include "game_object.h"
#include "timer.h"

//	Orientation data
//	Camera angle away from -Z
ANGLE theta[XYZ] = {DEGtoANG(0.0),DEGtoANG(0.0),DEGtoANG(0.0)};
//	Camera (player) height
FIXED height = toFIXED(1.8);
//	Camera Position
FIXED position[XYZ] = {toFIXED(0.0),toFIXED(1.8),toFIXED(0.0)};				
//	Default interaction target distance
FIXED tar_dist = toFIXED(0.5);	
//	Distance from target allow for interaction
FIXED tar_sense = toFIXED(0.25);	
//	Interaction target position
FIXED target[XYZ] = {toFIXED(0.0),toFIXED(0.0),toFIXED(0.5)};

//	Repeat Input Delay
FIXED input_delay = toFIXED(0.2);
FIXED last_input = toFIXED(-1.0);;
extern Sint8 SynchConst;

//	Add relative change to position coordinate
void add_rel_pos(FIXED x, FIXED y, FIXED z)
{
	//	Y is invariant under y-axis rotation
	position[Y] += y;
	
	//	Add to coordinate according to rotation
	position[X] += slMulFX(x,slCos(theta[Y])) + slMulFX(z,slSin(theta[Y]));
	position[Z] += slMulFX(z,slCos(theta[Y])) - slMulFX(x,slSin(theta[Y]));
}

//	Calculate forward coordinate for target based on distance and orientation
void forward_target(FIXED dist)
{
	target[Y] = position[Y]-height;
	
	//	Add to coordinate according to rotation
	target[X] = position[X] + slMulFX(-dist,slSin(theta[Y]));
	target[Z] = position[Z] + slMulFX(dist,slCos(theta[Y]));
}

//  Handle Input from Gamepad
void gamepad_input(void)
{
	int wait_frames = 15;
	FIXED move_inc = toFIXED(0.1);

	//	Poll for gamepad
	if (!jo_is_pad1_available())
		return ;
	
	//	Directional
	if (jo_is_pad1_key_pressed(JO_KEY_UP))
		add_rel_pos(toFIXED(0.0),toFIXED(0.0),move_inc);
    if (jo_is_pad1_key_pressed(JO_KEY_DOWN))
		add_rel_pos(toFIXED(0.0),toFIXED(0.0),-move_inc);
	if (jo_is_pad1_key_pressed(JO_KEY_LEFT))
		add_rel_pos(-move_inc,toFIXED(0.0),toFIXED(0.0));
    if (jo_is_pad1_key_pressed(JO_KEY_RIGHT))
		add_rel_pos(move_inc,toFIXED(0.0),toFIXED(0.0));
    if (jo_is_pad1_key_pressed(JO_KEY_L))
		theta[Y] -= (DEGtoANG(1.0));
    if (jo_is_pad1_key_pressed(JO_KEY_R))
		theta[Y] += (DEGtoANG(1.0));
	if (jo_is_pad1_key_pressed(JO_KEY_Z))
		add_rel_pos(toFIXED(0.0),-move_inc,toFIXED(0.0));
    if (jo_is_pad1_key_pressed(JO_KEY_C))
		add_rel_pos(toFIXED(0.0),move_inc,toFIXED(0.0));
	
	//	Button Presses
	if(time > last_input + input_delay)
	{
		if (jo_is_pad1_key_pressed(JO_KEY_A))
			create_object(target[X], target[Y], target[Z], theta[X], theta[Y], theta[Z], &entities[0]);
		if (jo_is_pad1_key_pressed(JO_KEY_B))
		{
			destroy_object(closest_object(target[X], target[Y], target[Z], tar_sense));
		}
		if (jo_is_pad1_key_pressed(JO_KEY_X))
		{
			//	Iterate framerate
			if (SynchConst == 1 )
			{
				SynchConst = (Sint8)3;
			}
			else
			{
				SynchConst -= (Sint8)1;
			}
		}
				
		//	Set last input time
		last_input = time;
	}
}


//	Prints orientation data on screen
void print_orientation(void)
{
	//	Timer
	slPrint("Time:",slLocate(0,1));
	slPrintFX(time,slLocate(6,1));
	slPrint("Framerate:",slLocate(19,1));
	jo_printf(30,1,"%i FPS",(60/SynchConst));
	
	slPrint("  Position     Rotation      Target   ",slLocate(0,2));
	//	Position
	slPrintFX(position[X],slLocate(0,3));
	slPrintFX(position[Y],slLocate(0,4));
	slPrintFX(position[Z],slLocate(0,5));
	//	Rotation
	slPrintFX(slAng2FX(theta[X]),slLocate(13,3));
	slPrintFX(slAng2FX(theta[Y]),slLocate(13,4));
	slPrintFX(slAng2FX(theta[Z]),slLocate(13,5));
	//	Target
	slPrintFX(target[X],slLocate(26,3));
	slPrintFX(target[Y],slLocate(26,4));
	slPrintFX(target[Z],slLocate(26,5));
}

