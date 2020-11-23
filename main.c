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

#include <jo/jo.h>
#include "ZT/ZT_COMMON.h"
#include "separate_3D.h"
#include "animate.h"
#include "input.h"
#include "game_object.h"
#include "collision.h"
#include "room.h"


//static int framenum = 0;
//static Uint8 old_tick = 0;
//static Uint8 tick = 0;


/**Functions/variables added by XL2 **/
/**Added by XL2 to use my own CD loading functions**/

#define     OPEN_MAX    (Sint32)5
#define     DIR_MAX     (Sint32)25
GfsDirTbl gfsDirTbl;
GfsDirName gfsDirName[DIR_MAX];
Uint32 gfsLibWork[GFS_WORK_SIZE(OPEN_MAX)/sizeof(Uint32)];
Sint32 gfsDirN;

//	Track loading addresses in LWRAM
void * currentAddress;

//	Added by XL2 - Very basic function to display a 3D model
void display_model(entity_t * model)
{
    int i;
    for (i=0; i<model->nbMeshes; i++)
    {
	   slPutPolygon((PDATA*)model->pol[i]);
    }
}
//	End of added functions and global variables


//	Number of models to load
Uint32 nbModels = 1;


//	Load 3D object
void load_ref(void){
	/*
		Load References
		
		Loads 3D models
	*/
	
	//	Start loading in LWRAM beginning
    void * currentAddress = (void*)LWRAM;
	
	//	Load rooms
	currentAddress = ztLoad3Dmodel((Sint8*)"COR1W.ZTP", currentAddress, 0, false);
	
	
	/*
	//	Load Jelly enemy
	currentAddress = ztLoad3Dmodel((Sint8*)"JELLY.ZTP", currentAddress, 0, false);
	r_radius[0] = calc_rough_radius(&entities[0]);
	
	//	Animations for Jelly
	//	Idle
	animationMaster[0][0].uniform = true;
	animationMaster[0][0].startFrm = 1;
	animationMaster[0][0].endFrm = 2;
	animationMaster[0][0].currentFrm = animationMaster[0][0].startFrm  * ANIM_CONST;
	//	Walk
	animationMaster[0][1].uniform = true;
	animationMaster[0][1].startFrm = 3;
	animationMaster[0][1].endFrm = 6;
	animationMaster[0][1].currentFrm = animationMaster[0][1].startFrm  * ANIM_CONST;
	*/
	
	return;
}


//	Drawing objects (every frame)
void my_draw(void)
{	
	/*
		My Draw
		
		Draws all objects
	*/
	
	//	Draw Rooms
	draw_rooms();
	
	//	Loop through objects
	for(int ind = 0; ind < num_object; ind++)
	{
		slPushMatrix();		//	Advance matrix buffer pointer 1 step
		
		//	Rotate perspective 
		slRotY(-theta[Y]);	
		//	Translate draw location
		slTranslate(object[ind].position[X] - pl_position[X],
			object[ind].position[Y] - pl_position[Y] + cam_height,
			object[ind].position[Z] - pl_position[Z]);		
		//	Rotate object
		slRotY(-object[ind].theta[Y]);	
		//	Sets scale of 3D draw, larger numbers draw bigger		
		slScale(object[ind].scale[X],object[ind].scale[Y],
			object[ind].scale[Z]);	
	
		//	Animation
		//display_animated_model(&object[ind], object[ind].ani);
		slPutPolygon((XPDATA*)object[ind].pDataStart);
		
		slPopMatrix();		//	Return matrix buffer pointer back 1 step
	}
	

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

//  Main Logic Loop
void main_loop(void)
{
	VECTOR gravity = {0, toFIXED(0.05), 0};
	
	while(1)
	{
		
		//	Time
		framerate = (int)SynchConst;
		jo_fixed_point_time();
		
		//	Pop matrix to unit matrix at pointer '0'
		slUnitMatrix(0);	
		slPrintFX(toFIXED(num_object),slLocate(30,0));

		//  Handle Input
		gamepad_input();

		//	Print Orientation Data
		print_orientation();

		//	Apply physics on objects
		//apply_accel_all(gravity);
		//is_rough_collide_all();
		//is_bbox_collide_all();
		//stop_collided();
		//update_obj_position();

		//  Draw Objects
		my_draw();
		
		
		//	Force screen blanking to wait until events are done processing
		slSynch();			
	}
}


void load_rooms_object()
{
	/*
		Load Rooms
		
		Manually Load room objects for display
	*/
	
	//	Grid spacing
	FIXED grid = 10<<16;
	
	//	Location to place room
	FIXED room_loc[XYZ];
	FIXED room_angle[XYZ];
	
	//	Room 1
	room_loc[X] = 0;
	room_loc[Y] = 0<<16;
	room_loc[Z] = 0*grid;
	room_angle[X] = DEGtoANG(90.0);
	room_angle[Y] = 0;
	room_angle[Z] = 0;
	create_object(room_loc, room_angle, 0);
	
	//	Room 2
	room_loc[X] = 0;
	room_loc[Y] = 0<<16;
	room_loc[Z] = 1*grid;
	room_angle[X] = 0;
	room_angle[Y] = 0;
	room_angle[Z] = 0;
	create_object(room_loc, room_angle, 0);
	
	//	Room 3
	room_loc[X] = 0;
	room_loc[Y] = 0<<16;
	room_loc[Z] = 2*grid;
	room_angle[X] = 0;
	room_angle[Y] = 0;
	room_angle[Z] = 0;
	create_object(room_loc, room_angle, 0);
	
	//	Room 4
	room_loc[X] = 0;
	room_loc[Y] = 0<<16;
	room_loc[Z] = 3*grid;
	room_angle[X] = -DEGtoANG(90.0);
	room_angle[Y] = 0;
	room_angle[Z] = 0;
	create_object(room_loc, room_angle, 0);
	
	return;
}


void jo_main(void)
{
	
	
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
	slPerspective(DEGtoANG(90.0));
	
	//	Background Layer
	jo_img bg;
    bg.data = NULL;
    jo_tga_loader(&bg, "BG", "BG2.TGA", JO_COLOR_Transparent);
    jo_set_background_sprite(&bg, 0, 0);
    jo_free_img(&bg);
	

	//	Load assets
	//load_ref();		//	Load 3D Object
	load_rooms();	//	Load in rooms
	
	//	Prepare NBGH layers
	slScrAutoDisp(NBG0ON | NBG1ON | NBG3ON | SPRON);

	//	Prepare rooms
	initialize_rooms();
	generate_rooms();
	map_rooms();	
	//load_rooms_object();

	//	Initialize player colission
	player_bbox_init();
	
	//	Smooth background fade in
	fadeIn();
	
	//	Main loop of game
	main_loop();	
}
