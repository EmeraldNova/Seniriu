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
//	Camera Position
FIXED pl_position[XYZ] = {0,0,0};				
//	Default interaction target distance
FIXED tar_dist = 5 << 16;	
//	Distance from target allow for interaction
FIXED tar_sense = 16384;	//	0.25 FIXED
//	Interaction target position
FIXED target[XYZ] = {0,0,5 << 16};

//	Repeat Input Delay
FIXED input_delay = 13107;	//	0.2 FIXED
FIXED last_input = -(1 << 16);

//	PLayer camera hieght
FIXED cam_default_height;
FIXED cam_height;

//	Player movement
FIXED move_inc;

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

//	Calculate forward coordinate for target based on distance and orientation
void forward_target(FIXED dist)
{
	target[Y] = pl_position[Y];
	
	//	Add to coordinate according to rotation
	target[X] = pl_position[X] + slMulFX(dist,slSin(theta[Y]));
	target[Z] = pl_position[Z] + slMulFX(dist,slCos(theta[Y]));
}

//  Handle Input from Gamepad
void gamepad_input(void)
{
	/*
		Gamepad Input
		
		Parse player input
	*/
	
	//	Set default height
	cam_default_height = slMulFX(toFIXED(1.5),scale_factor);
	
	//	Set speed of movement
	move_inc = slMulFX(8000,scale_factor);

	//	Poll for gamepad
	if (!jo_is_pad1_available())
		return ;
	
	//	Speed Modifier
	cam_height = cam_default_height;
	FIXED speed = move_inc;
	if (jo_is_pad1_key_pressed(JO_KEY_B))
	{
		//	Running
		speed *= 3;
		cam_height -= slMulFX(toFIXED(0.05),scale_factor);
	}
	if (jo_is_pad1_key_pressed(JO_KEY_Y))
	{
		//	Sneaking
		speed /= 4;
		cam_height -= slMulFX(toFIXED(0.8),scale_factor);;
	}
	move_inc = speed;
		
	
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
	
	//	Update target
	forward_target(tar_dist);
	
	//	Button Presses
	/*
	if(time_in_seconds > last_input + input_delay)
	{
		if (jo_is_pad1_key_pressed(JO_KEY_A))
		{
			ANGLE draw_theta[XYZ] = {theta[X],-theta[Y],theta[Z]};
			create_object(target, draw_theta, 0);
		}
		if (jo_is_pad1_key_pressed(JO_KEY_B))
		{
			destroy_object(closest_object(target[X], target[Y], target[Z], tar_sense));
		}
		if (jo_is_pad1_key_pressed(JO_KEY_X))
		{
			if(num_object > 0)
			{
				object[num_object - 1].ani = 1 - object[num_object - 1].ani;
			}
		}
				
		//	Set last input time
		last_input = time_in_seconds;
	}
	*/
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
	
	
	slPrint("  Position     Rotation      Target   ",slLocate(0,2));
	//	Position
	slPrintFX(pl_position[X],slLocate(0,3));
	slPrintFX(pl_position[Y],slLocate(0,4));
	slPrintFX(pl_position[Z],slLocate(0,5));
	//	Rotation
	slPrintFX(slAng2FX(theta[X]),slLocate(13,3));
	slPrintFX(slAng2FX(theta[Y]),slLocate(13,4));
	slPrintFX(slAng2FX(theta[Z]),slLocate(13,5));
	/*
	//	Target
	slPrintFX(target[X],slLocate(26,3));
	slPrintFX(target[Y],slLocate(26,4));
	slPrintFX(target[Z],slLocate(26,5));
	*/
	
	//	First object check
	if(num_object > 0)
	{
		/*
		slPrint("Position: ",slLocate(0,6));
		slPrintFX(object[num_object-1].position[Y],slLocate(10,6));
		slPrint("Velocity: ",slLocate(0,7));
		slPrintFX(object[num_object-1].velocity[Y],slLocate(10,7));
		
		//	Check rough collision
		if(collision_master_bbox[num_object])
		{
			slPrint("Box Colliding       ",slLocate(0,8));
		}
		else
		{
			slPrint("No Box Colliding    ",slLocate(0,8));
		}
		if(num_object > 1)
		{
			if(collision_master_bbox[num_object-1])
			{
				slPrint("Second Box Colliding",slLocate(0,18));
			}
			else
			{
				slPrint("Second Not Colliding",slLocate(0,18));
			}
		}
		
		// Check bounding box collision
		if(collision_master_bbox[num_object-1])
		{
			slPrint("Box Colliding       ",slLocate(20,8));
		}
		else
		{
			slPrint("No Box Colliding    ",slLocate(20,8));
		}		
		
		//print_corners(num_object-1);
		*/
	}
}

