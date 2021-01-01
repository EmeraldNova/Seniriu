/*
** Seniriu
** Matthew Suttinger & Emerald Nova (emeraldnovagames@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

#include "input.h"

//	Orientation data
//	Camera angle away from -Z
ANGLE theta[XYZ] = {DEGtoANG(0.0),DEGtoANG(0.0),DEGtoANG(0.0)};
ANGLE theta2 = 0;
//	Camera Position
FIXED pl_position[XYZ] = {0,0,0};				

//	PLayer camera hieght
FIXED cam_default_height;
FIXED cam_height;

//	Player movement
FIXED move_inc;
int num_object = 5;
int entity_ID = 0;

//	Add relative change to position coordinate
void add_rel_pos(FIXED x, FIXED y, FIXED z)
{
	//	Y is invariant under y-axis rotation
	pl_position[Y] += y;
	//	Floor is 0
	/*
	if(pl_position[Y]>0)
	{
		pl_position[Y] = 0;
	}
	*/
	
	//	Add to coordinate according to rotation
	pl_position[X] += slMulFX(x,slCos(theta[Y])) + slMulFX(z,slSin(theta[Y]));
	pl_position[Z] += slMulFX(z,slCos(theta[Y])) - slMulFX(x,slSin(theta[Y]));
}

//  Handle Input from Gamepad
void gamepad_input(void)
{
	/*
		Gamepad Input
		
		Parse player input
	*/
	
	//	Set default height
	cam_default_height = slMulFX(toFIXED(1.8),scale_factor);
	cam_height = cam_default_height;
	
	//	Set speed of movement
	move_inc = slMulFX(8000,scale_factor);

	//	Poll for gamepad
	if (!jo_is_pad1_available())
		return ;
	
	//	Directional
	if (jo_is_pad1_key_pressed(JO_KEY_UP))
		add_rel_pos(0,0,move_inc);
    if (jo_is_pad1_key_pressed(JO_KEY_DOWN))
		add_rel_pos(0,0,-move_inc);
	if (jo_is_pad1_key_pressed(JO_KEY_LEFT))
		theta[Y] -= (DEGtoANG(3.0));
    if (jo_is_pad1_key_pressed(JO_KEY_RIGHT))
		theta[Y] += (DEGtoANG(3.0));
    if (jo_is_pad1_key_pressed(JO_KEY_L))
		add_rel_pos(-move_inc,0,0);
    if (jo_is_pad1_key_pressed(JO_KEY_R))
		add_rel_pos(move_inc,0,0);
	if (jo_is_pad1_key_pressed(JO_KEY_Z))
		add_rel_pos(0,-move_inc,0);
    if (jo_is_pad1_key_pressed(JO_KEY_C))
		add_rel_pos(0,move_inc,0);
	if (jo_is_pad1_key_down(JO_KEY_A))
		num_object += 1;
	if (jo_is_pad1_key_down(JO_KEY_X))
		num_object = JO_MAX(1, num_object-1);
	if (jo_is_pad1_key_down(JO_KEY_B))
		entity_ID += 1;
	if (jo_is_pad1_key_down(JO_KEY_Y))
		entity_ID = JO_MAX(0, entity_ID-1);
	if (jo_is_pad1_key_down(JO_KEY_START))
		theta2 += DEGtoANG(90.0);
	
	return;
}


//	Prints orientation data on screen
void print_orientation(void)
{
	//	Timer
	slPrint("Time:",slLocate(0,1));
	slPrintFX(time_in_seconds,slLocate(6,1));
	slPrint("Framerate:",slLocate(19,1));
	slPrintFX(slDivFX(delta_time,1 << 16),slLocate(29,1));
	slPrint("FPS",slLocate(37,1));
	slPrintFX(slAng2FX(theta2),slLocate(6,0));
	slPrint("Angle",slLocate(0,0));
	
	
	slPrint("  Position     Rotation      # Rendered",slLocate(0,2));
	//	Position
	slPrintFX(pl_position[X],slLocate(0,3));
	slPrintFX(pl_position[Y],slLocate(0,4));
	slPrintFX(pl_position[Z],slLocate(0,5));
	//	Rotation
	slPrintFX(slAng2FX(theta[X]),slLocate(13,3));
	slPrintFX(slAng2FX(theta[Y]),slLocate(13,4));
	slPrintFX(slAng2FX(theta[Z]),slLocate(13,5));
	//	Number of rendered copies
	slPrintFX(slAng2FX(theta[Z]),slLocate(29,5));
	
	return;
}

