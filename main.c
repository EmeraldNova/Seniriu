/*
** Seniriu
** Matthew Suttinger & Emerald Nova (johannesfetz@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

#include <jo/jo.h>
#include "templates.h"
#include "game_object.h"
#include "display.c"


static int framenum = 0;
static int last_create_frame = -9999;

//  Handle Input from Gamepad
void gamepad_input(void)
{
    int delta[3] = {0,0,0};
	int destroy_ID;
	int wait_frames = 15;

	if (!jo_is_pad1_available())
		return ;
	if (jo_is_pad1_key_pressed(JO_KEY_UP))
		delta[2] = 5;
	if (jo_is_pad1_key_pressed(JO_KEY_DOWN))
		delta[2] = -5;
    if (jo_is_pad1_key_pressed(JO_KEY_LEFT))
		delta[0] = -5;
	if (jo_is_pad1_key_pressed(JO_KEY_RIGHT))
		delta[0] = 5;
	if (jo_is_pad1_key_pressed(JO_KEY_A))
	{
		if(framenum > last_create_frame + wait_frames)
		{
			last_create_frame = framenum;
			create_object(cam_tar[0], cam_tar[1], cam_tar[2], metal_crate_1_v, metal_crate_1_q, 6);
		}
	}
	if (jo_is_pad1_key_pressed(JO_KEY_B))
	{
		;
	}
    if (jo_is_pad1_key_pressed(JO_KEY_C))
		delta[1] = 5;
	if (jo_is_pad1_key_pressed(JO_KEY_X))
	{
		if(framenum > last_create_frame + wait_frames)
		{
			destroy_ID = closest_object(cam_tar[0], cam_tar[1], cam_tar[2], 50);
			if(destroy_ID != -1)
			{
				destroy_object(destroy_ID);
			}
		}
	}
	if (jo_is_pad1_key_pressed(JO_KEY_Y))
		;
	if (jo_is_pad1_key_pressed(JO_KEY_Z))
		delta[1] = -5;	
	if (jo_is_pad1_key_pressed(JO_KEY_L))
		cam_angle[1] -= 1;
	if (jo_is_pad1_key_pressed(JO_KEY_R))
		cam_angle[1] += 1;
    if (jo_is_pad1_key_pressed(JO_KEY_START))
    {
        cam_pos[0] = 0;
        cam_pos[1] = 0;
        cam_pos[2] = 0;
        cam_angle[0] = 0;
        cam_angle[1] = 0;
        cam_angle[2] = 0;
    }

    //  Bound Camera Angle
    if (cam_angle[1] > 180)
        cam_angle[1] -= 360;
    else if (cam_angle[1] <= -180)
        cam_angle[1] += 360;

    //  Fix Camera Position
    cam_pos[0] += (delta[0]*jo_cos(cam_angle[1]) + delta[2]*jo_sin(cam_angle[1]))/32768;
    cam_pos[1] += delta[1];
    cam_pos[2] += (delta[2]*jo_cos(cam_angle[1]) - delta[0]*jo_sin(cam_angle[1]))/32768;

    //  Fix Camera Target/Orientation
    cam_tar[0] = (cam_pos[0]*32768 + 150*jo_sin(cam_angle[1]))/32768;
    cam_tar[1] = cam_pos[1];
    cam_tar[2] = (cam_pos[2]*32768 + 150*jo_cos(cam_angle[1]))/32768;
}

//  Main Logic Loop
void main_loop(void)
{
	// Display frame number
	framenum++;
	jo_printf(0, 0, "Frame # %10d - Object # %10d", framenum%60 + 1, num_object);
	
    //  Apply object speeds for animation
	int index;
	for( index = 0; index < num_object; index++ )
	{
		// Increment by velocity
		object[index].x += object[index].vx;
		object[index].y += object[index].vy;
		object[index].z += object[index].vz;
		
		// Increment by rotational speed
		object[index].rx += object[index].vrx;
		object[index].ry += object[index].vry;
		object[index].rz += object[index].vrz;
	}

    //  Handle Input
    gamepad_input();

	// Move Camera according to position & target
    move_cam();

    //  Draw Objects
    my_draw();
}

void jo_main(void)
{
    //  Initialize engine with black background
	jo_core_init(JO_COLOR_Black);

	//  Initialize camera
    start_cam();

    //  Initialize Templates
    initilize_geo("metal_crate_1");

    //  Create cube
    //create_cube(&objects[0], -200, 0, 500, 20, 45, 0, metal_crate_1_v, metal_crate_1_q);
    //create_cube(&objects[1], 0, 0, 500, 20, 45, 0, metal_crate_1_v, metal_crate_1_q);
    //create_cube(&objects[2], 200, 0, 500, 20, 45, 0, metal_crate_1_v, metal_crate_1_q);
    //create_cube(&objects[3], 0, 0, -500, 20, 45, 0, metal_crate_1_v, metal_crate_1_q);

	//	Set Rotations for cubes
	//objects[0].vrx = 1;
	//objects[1].vry = 1;
	//objects[2].vrz = 1;

    //  Set main loop
	jo_core_add_callback(main_loop);

	//  Start game
	jo_core_run();
}
