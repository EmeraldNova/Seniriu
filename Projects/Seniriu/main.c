/*
** Seniriu
** Matthew Suttinger & Emerald Nova (emeraldnovagames@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/
/*
** Jo Sega Saturn Engine
** Copyright (c) 2012-2017, Johannes Fetz (johannesfetz@gmail.com)
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of the Johannes Fetz nor the
**       names of its contributors may be used to endorse or promote products
**       derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL Johannes Fetz BE LIABLE FOR ANY
** DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//	Include
#include "main.h"


//	Functions
void my_draw(void)
{	
	/*
		My Draw
		
		Draws all objects
	*/
	
	//	Draw background
	//draw_bg();
	
	//	Draw HUD
	draw_HUD();
	
	//	Draw Equipped Items
	draw_equipped();
	
	
	//	Loop through objects
	for(int ind = 0; ind < num_object; ind++)
	{
		slPushMatrix();		//	Advance matrix buffer pointer 1 step
		
		//	Rotate perspective 
		slRotY(-player.theta[Y]);	
		//	Translate draw location
		slTranslate(16*(object[ind].position[X] - player.pos[X]),
					16*(object[ind].position[Y] - player.pos[Y] + cam_height),
					16*(object[ind].position[Z] - player.pos[Z]));		
		//	Rotate object
		slRotY(-object[ind].theta[Y]);	
		//	Sets scale of 3D draw, larger numbers draw bigger		
		slScale(object[ind].scale[X],object[ind].scale[Y],
			object[ind].scale[Z]);	
	
		//	Animation
		display_animated_model(ind);
		//slPutPolygon((XPDATA*)object[ind].pDataStart);
		
		slPopMatrix();		//	Return matrix buffer pointer back 1 step
	}
	
	//	Draw Rooms
	draw_rooms();

	//	Print polygon info
	//slPrintFX(toFIXED(jo_3d_get_polygon_count()), slLocate(0,1));
    //slPrintFX(toFIXED(jo_3d_get_vertices_count()), slLocate(0,2));
    //slPrintFX(toFIXED(jo_3d_get_displayed_polygon_count()), slLocate(0,3));
	//slPrintFX(toFIXED(framerate), slLocate(0,4));

	//	Print pData infoslPutPolygon((XPDATA*)entities[0].pol[0]);
	//jo_printf(0, 10, "pdm:(%10i)", (int)pdataMaster[num_object-1][0]);
	//jo_printf(20, 10, "pds:(%10i)", (int)object[num_object-1].pDataStart);
	//jo_printf(0, 11, "size:(%10i)", (int)pDataSizes[0]);
	//jo_printf(0, 12, "epd+5:(%10i)", (int)(*(int*)(entities[0].pol[0]+5)));
	//jo_printf(20, 12, "opd+5:(%10i)", (int)(*(int*)(object[0].pDataStart+5)));

    return;
}

void load_game_objects(void)
{
	/*
		Load Game Objects
		
		Initialize game objects for testing
	*/
	
	//	Load model at position
	FIXED position[XYZ] = {0<<16, -(1<<14), 0<<16};
	ANGLE rot[XYZ] = {0, DEGtoANG(180.0), 0};
	
	
	//	Object 0
	position[X] = -(3<<16);
	position[Y] = -(0<<6);
	position[Z] =  (3<<16);
	create_object(position, rot, enemy_start);
	object[num_object-1].ani_con.currentAni = 1;
	object[num_object-1].ani_con.currentFrm = 0;
	object[num_object-1].ani_con.currentKeyFrm = 0;
	object[num_object-1].ani_con.last_frame[0] = 0;
	object[num_object-1].ani_con.last_frame[1] = (29)*3 - 1;
	for(int j = 0; j <= 29; j++)
	{
		object[num_object-1].ani_con.frames[1][3*j] = j;
		object[num_object-1].ani_con.frames[1][3*j+1] = j;
		object[num_object-1].ani_con.frames[1][3*j+2] = j;
	}
	
	//	Object 1
	position[X] =  (3<<16);
	position[Y] = -(0<<6);
	position[Z] =  (3<<16);
	create_object(position, rot, enemy_start + 1);
	object[num_object-1].ani_con.currentAni = 1;
	object[num_object-1].ani_con.currentFrm = 0;
	object[num_object-1].ani_con.currentKeyFrm = 0;
	object[num_object-1].ani_con.last_frame[0] = 0;
	object[num_object-1].ani_con.last_frame[1] = (39)*3 - 1;
	for(int j = 0; j <= 39; j++)
	{
		object[num_object-1].ani_con.frames[1][3*j] = j;
		object[num_object-1].ani_con.frames[1][3*j+1] = j;
		object[num_object-1].ani_con.frames[1][3*j+2] = j;
	}
	
	//	Object 2
	position[X] =  (3<<16);
	position[Y] =  (1<<15);
	position[Z] = -(3<<16);
	create_object(position, rot, enemy_start + 2);
	object[num_object-1].ani_con.currentAni = 1;
	object[num_object-1].ani_con.currentFrm = 0;
	object[num_object-1].ani_con.currentKeyFrm = 0;
	object[num_object-1].ani_con.last_frame[0] = 0;
	object[num_object-1].ani_con.last_frame[1] = (59)*3 - 1;
	for(int j = 0; j <= 59; j++)
	{
		object[num_object-1].ani_con.frames[1][3*j] = j;
		object[num_object-1].ani_con.frames[1][3*j+1] = j;
		object[num_object-1].ani_con.frames[1][3*j+2] = j;
	}
	
	//	Object 3
	position[X] = -(3<<16);
	position[Y] = -(0<<6);
	position[Z] = -(3<<16);
	create_object(position, rot, enemy_meshed_start);
	object[num_object-1].ani_con.currentAni = 1;
	object[num_object-1].ani_con.currentFrm = 0;
	object[num_object-1].ani_con.currentKeyFrm = 0;
	object[num_object-1].ani_con.last_frame[0] = 0;
	object[num_object-1].ani_con.last_frame[1] = (79)*3 - 1;
	for(int j = 0; j <= 79; j++)
	{
		object[num_object-1].ani_con.frames[1][3*j] = j;
		object[num_object-1].ani_con.frames[1][3*j+1] = j;
		object[num_object-1].ani_con.frames[1][3*j+2] = j;
	}
	
	return;
}

//  Main Logic Loop
void main_loop(void)
{
	while(1)
	{
		//	Time
		framerate = (int)SynchConst;
		jo_fixed_point_time();
		
		//	Pop matrix to unit matrix at pointer '0'
		slUnitMatrix(0);	

		//  Handle Input
		gamepad_input();

		//	Print Orientation Data
		//print_orientation();
		//slPrintFX(toFIXED(num_object),slLocate(30,0));

		//	Apply physics on objects
		//apply_accel_all(gravity);
		//is_rough_collide_all();
		//is_bbox_collide_all();
		//stop_collided();
		//update_obj_position();
		character_physics(&player);

		//	Lighting direction
		//slLight(lighting_direction);
		
		//  Draw Objects
		my_draw();
		
		//	Force screen blanking to wait until events are done processing
		slSynch();			
	}
}

void jo_main(void)
{
	/*
		Jo Main
		
		Main initialization and program starting function
	*/
	
	//  Initialize engine with black background
	jo_core_init(JO_COLOR_Black);	
	
	//*(volatile unsigned long *)0x20200000 = 0xDEADBEEF;
	//while (true) { }
	
	//test2D_init();
	//test2D_reset();
	//test2D_loop();

	

	//	Added by XL2 
	fadeOut(true);			//	Fade out screen and blank background
    SynchConst = (Sint8)2; 	//	Framerate control. 1/60 = 60 FPS, 2/60 = 30 FPS, etc.
	framerate = 2;			//	Repeat framerate definition
	slDynamicFrame(ON);		//	Dynamic framerate
	slZdspLevel(7);			//	Define frustrum culling near plane
    //	FOV
	slPerspective(FOV);
	
	//	Background Layer
	jo_img bg;
    bg.data = NULL;
    jo_tga_loader(&bg, "BG", "BG3.TGA", JO_COLOR_Transparent);
    jo_set_background_sprite(&bg, 0, 0);
    jo_free_img(&bg);
	//bg_init(0);

	//	Prepare NBGH layers
	slScrAutoDisp(NBG0ON | NBG1ON | NBG3ON | SPRON);

	//	Load assets
	load_models();			//	Load 3D Object
	load_game_objects();	//	Load test game objects
	load_HUD();
	init_items();			//	Initalize items
	init_planes();			//	Initalize items

	//	Prepare rooms
	initialize_rooms();
	generate_rooms();
	map_rooms();	

	//	Initialize player colission
	init_character_test();	//	Prepare for physics test
	
	//	Smooth background fade in
	fadeIn();
	
	//	Main loop of game
	main_loop();	
}
