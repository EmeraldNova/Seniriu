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

#include "input.h"
#include "game_object.h"
#include "collision.h"
#include "test2D.h"

static int framenum = 0;
static Uint8 old_tick = 0;
static Uint8 tick = 0;


/**Functions/variables added by XL2 **/
/**Added by XL2 to use my own CD loading functions**/
#define     OPEN_MAX    (Sint32)5
#define     DIR_MAX     (Sint32)25
GfsDirTbl gfsDirTbl;
GfsDirName gfsDirName[DIR_MAX];
Uint32 gfsLibWork[GFS_WORK_SIZE(OPEN_MAX)/sizeof(Uint32)];
Sint32 gfsDirN;
//static int BG;

//	File access with calls to SGL structs and definitions
void ztCDinit(void)
{
    GFS_DIRTBL_TYPE(&gfsDirTbl) = GFS_DIR_NAME;
    GFS_DIRTBL_DIRNAME(&gfsDirTbl) = gfsDirName;
    GFS_DIRTBL_NDIR(&gfsDirTbl) = DIR_MAX;
    gfsDirN = GFS_Init(OPEN_MAX, gfsLibWork, &gfsDirTbl);
}

//	Framerate definitions (different integer types for different context)
extern Sint8 SynchConst;
Sint32 framerate;
/****/


/**Added by XL2 - Very basic function to display a 3D model **/
void display_model(entity_t * model)
{
    int i;
    for (i=0; i<model->nbMeshes; i++)
    {
	   slPutPolygon((PDATA*)model->pol[i]);
    }
}
/**End of added functions and global variables**/

//	Number of models to load
Uint32 nbModels = 1;

//	Load 3D object
void load_ref(void){
    /**ZTP stands for Z-Treme polygonal model**/
    void * currentAddress = (void*)LWRAM;
	currentAddress = ztLoad3Dmodel((Sint8*)"TESTBOX.ZTP", currentAddress, &entities[0], false);
	r_radius[0] = calc_rough_radius(&entities[0]);
}

//	Drawing objects (every frame)
void my_draw(void)
{
	for(int ind = 0; ind < num_object; ind++)
	{
		slPushMatrix();		//	Advance matrix buffer pointer 1 step

		//	Rotate perspective 
			slRotY(-theta[Y]);	
		//	Translate draw location
		slTranslate(object[ind].position[X] - pl_position[X],
			object[ind].position[Y] - pl_position[Y],
			object[ind].position[Z] - pl_position[Z]);		
		//	Rotate object
			slRotY(-object[ind].theta[Y]);	
		//	Sets scale of 3D draw, larger numbers draw bigger		
		slScale(object[ind].scale[X],object[ind].scale[Y],
			object[ind].scale[Z]);	
	
		display_model(object[ind].entity);			//	Displays model
		slPopMatrix();		//	Return matrix buffer pointer back 1 step
	}

	//	Print polygon info
	//slPrintFX(toFIXED(jo_3d_get_polygon_count()), slLocate(0,1));
    //slPrintFX(toFIXED(jo_3d_get_vertices_count()), slLocate(0,2));
    //slPrintFX(toFIXED(jo_3d_get_displayed_polygon_count()), slLocate(0,3));
	//slPrintFX(toFIXED(framerate), slLocate(0,4));

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
		timer();
		
		//	Pop matrix to unit matrix at pointer '0'
		slUnitMatrix(0);	
		slPrintFX(toFIXED(num_object),slLocate(30,0));

		//  Handle Input
		gamepad_input();

		//	Print Orientation Data
		print_orientation();

		//	Apply physics on objects
		apply_accel_all(gravity);
		is_rough_collide_all();
		stop_collided();
		update_obj_position();

		//  Draw Objects
		my_draw();
		
		//	Force screen blanking to wait until events are done processing
		slSynch();			
	}
}

void jo_main(void)
{
	//  Initialize engine with black background
	jo_core_init(JO_COLOR_Black);	
	test2D_init();

	//	Added by XL2 
	fadeOut(true);			//	Fade out screen and blank background
    SynchConst = (Sint8)1; 	//	Framerate control. 1/60 = 60 FPS, 2/60 = 30 FPS, etc.
	framerate = 1;			//	Repeat framerate definition
	slZdspLevel(7);			//	Define frustrum culling near plane
    /****/
	
	//	Shoehorn in Background Layer
	// BG = jo_sprite_add_tga(JO_ROOT_DIR, "BGTREE1.TGA", JO_COLOR_Transparent);
	// jo_set_background_sprite(BG, 0, 0); 	

	/**XL2**/
	ztCDinit(); 	//	Prepare file system for loading things in directory
	load_ref();		//	Load 3D Object
	slScrAutoDisp(NBG0ON | NBG1ON | NBG3ON | SPRON);	//	?

	fadeIn();	//	Smooth background fade in

	//slPrint("Go!",slLocate(19,0));

	main_loop();	//	Main loop of game
}
