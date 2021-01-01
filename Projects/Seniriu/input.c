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
//ANGLE theta[XYZ] = {DEGtoANG(0.0),DEGtoANG(0.0),DEGtoANG(0.0)};
//	Camera Position
FIXED pl_position[XYZ] = {0,0,0};				
//	Default interaction target distance
//FIXED tar_dist = 5 << 16;	
//	Distance from target allow for interaction
//FIXED tar_sense = 16384;	//	0.25 FIXED
//	Interaction target position
//FIXED target[XYZ] = {0,0,5 << 16};

//	Repeat Input Delay
FIXED input_delay = 13107;	//	0.2 FIXED
FIXED last_input = -(1 << 16);

//	PLayer camera hieght
FIXED cam_default_height;
FIXED cam_height;

//	Player movement
FIXED move_inc;

//	Functions
void add_rel(FIXED add[XYZ], ANGLE theta[XYZ], FIXED store[XYZ])
{
	/*
		Add Relative Change
		
		Adds a vector to another after performing rotations
	*/

	//	Add to coordinate according to rotation
	store[X] += slMulFX(add[X],slCos(theta[Y])) + slMulFX(add[Z],slSin(theta[Y]));
	store[Y] += add[Y];
	store[Z] += slMulFX(add[Z],slCos(theta[Y])) - slMulFX(add[X],slSin(theta[Y]));
	
	return;
}

//	Calculate forward coordinate for target based on distance and orientation
void forward_target(FIXED dist)
{
	//target[Y] = pl_position[Y];
	
	//	Add to coordinate according to rotation
	//target[X] = pl_position[X] + slMulFX(dist,slSin(theta[Y]));
	//target[Z] = pl_position[Z] + slMulFX(dist,slCos(theta[Y]));
	
	return;
}

//  Handle Input from Gamepad
void gamepad_input(void)
{
	/*
		Gamepad Input
		
		Parse player input
	*/
	
	//	Set default height
	cam_default_height = toFIXED(1.5);
	
	//	Set speed of movement
	move_inc = (50<<16);

	//	Poll for gamepad
	if (!jo_is_pad1_available())
		return ;
	
	//	Speed Modifier
	cam_height = cam_default_height;
	if (jo_is_pad1_key_pressed(JO_KEY_Z))
	{
		//	Running
		move_inc *= 3;
		cam_height -= jo_fixed_mult(toFIXED(0.05), cam_default_height);
	}
	if (jo_is_pad1_key_pressed(JO_KEY_Y))
	{
		//	Sneaking
		player.sneaking = true;
		move_inc /= 4;
		cam_height -= jo_fixed_mult(toFIXED(0.05), cam_default_height);
	}
	else
	{
		player.sneaking = false;
	}
		
	
	//	Directional
	//	Turning
	if (jo_is_pad1_key_pressed(JO_KEY_LEFT))
	{
		//	Rotate left, convert FIXED time to ANGLE
		player.theta[Y] -= jo_fixed_mult(360,(45*delta_time));
	}
	if (jo_is_pad1_key_pressed(JO_KEY_RIGHT))
	{
		//	Rotate right, convert FIXED time to ANGLE
		player.theta[Y] += jo_fixed_mult(360,(45*delta_time));
	}

	//	Movement
	FIXED acc[XYZ] = {0, 0, 0};
	FIXED add[XYZ];
	if(player.colliding)
	{
		if (jo_is_pad1_key_pressed(JO_KEY_UP))
		{
			//	Move Forward
			add[X] = 0;
			add[Y] = 0;
			add[Z] = move_inc;
			add_rel(add, player.theta, acc);
			//add_rel_pos(0,0,move_inc);
		}
		if (jo_is_pad1_key_pressed(JO_KEY_DOWN))
		{	
			//	Move Backward
			add[X] = 0;
			add[Y] = 0;
			add[Z] = -move_inc;
			add_rel(add, player.theta, acc);
			//add_rel_pos(0,0,-move_inc);
		}
		if (jo_is_pad1_key_pressed(JO_KEY_L))
		{
			//	Move Left
			add[X] = -move_inc;
			add[Y] = 0;
			add[Z] = 0;
			add_rel(add, player.theta, acc);
			//add_rel_pos(-move_inc,0,0);
		}
		if (jo_is_pad1_key_pressed(JO_KEY_R))
		{
			//	Move Right
			add[X] = move_inc;
			add[Y] = 0;
			add[Z] = 0;
			add_rel(add, player.theta, acc);
			//add_rel_pos(move_inc,0,0);
		}
		if (jo_is_pad1_key_pressed(JO_KEY_C))
		{
			//	Jump
			add[X] = 0;
			add[Y] = -(10*gravity[Y]);
			add[Z] = 0;
			add_rel(add, player.theta, acc);
			
			//	Cheat physics to avoid collision issues
			player.pos[Y] -= 1<<13;
			player.pos_next[Y] -= 1<<13;
		}
			
		//	Apply acceleration
		acc_character(&player, acc);
	}
		
	//	Actions
	/*
	if (jo_is_pad1_key_down(JO_KEY_A))
	{
		//	Change animation of first object
		object[0].ani_con.currentAni++;
		if(object[0].ani_con.currentAni > 1)
		{
			object[0].ani_con.currentAni = 0;
		}
	}
	*/
	/*
	if (jo_is_pad1_key_down(JO_KEY_X))
	{
		//	Hurt Player
		player.HealthP = 1;
		player.StaminaP = 0;
		player.MagicP = 0;
		//slPrint("HURT",slLocate(0,20));
	}
	*/
	/*
	if (jo_is_pad1_key_down(JO_KEY_START) && 
		jo_is_pad1_key_pressed(JO_KEY_A) &&
		jo_is_pad1_key_pressed(JO_KEY_B) &&
		jo_is_pad1_key_pressed(JO_KEY_C) )
	{
		
		//	Reset Height
		player.pos[X] = 0;
		player.pos_next[X] = 0;
		player.pos[Y] = -(1<<0);
		player.pos_next[Y] = -(1<<0);
		player.pos[Z] = 0;
		player.pos_next[Z] = 0;
		player.vel[X] = 0;
		player.vel_next[X] = 0;
		player.vel[Y] = 0;
		player.vel_next[Y] = 0;
		player.vel[Z] = 0;
		player.vel_next[Z] = 0;
		
		
		//	Reset
		SYS_Exit();
	}
	*/
	
	//	Update target
	//forward_target(tar_dist);
	
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
	slPrintFX(player.pos[X],slLocate(0,3));
	slPrintFX(player.pos[Y],slLocate(0,4));
	slPrintFX(player.pos[Z],slLocate(0,5));
	//	Rotation
	slPrintFX(slAng2FX(player.theta[X]),slLocate(13,3));
	slPrintFX(slAng2FX(player.theta[Y]),slLocate(13,4));
	slPrintFX(slAng2FX(player.theta[Z]),slLocate(13,5));
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

