/*
** Seniriu
** Matthew Suttinger & Emerald Nova (johannesfetz@gmail.com)
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

static int framenum = 0;
static unsigned int old_tick = -1;
static unsigned int tick = 0;
static int last_create_frame = -9999;

/**Functions/variables added by XL2 **/
/**Added by XL2 to use my own CD loading functions**/
#define     OPEN_MAX    (Sint32)5
#define     DIR_MAX     (Sint32)25
GfsDirTbl gfsDirTbl;
GfsDirName gfsDirName[DIR_MAX];
Uint32 gfsLibWork[GFS_WORK_SIZE(OPEN_MAX)/sizeof(Uint32)];
Sint32 gfsDirN;

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

/**Taken from RB demo**/
static scale3D = 16;		//	Scale for drawing objects

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

//	Render Positioning
ANGLE yy;
FIXED xDist = toFIXED(0.0);
FIXED yDist = toFIXED(0.0);
FIXED zDist = toFIXED(800.0);
Uint32 nbModels = 1;

//	Load 3D object
void load_ref(void){
    /**ZTP stands for Z-Treme polygonal model**/
    void * currentAddress = (void*)LWRAM;
	currentAddress = ztLoad3Dmodel((Sint8*)"WCHAIR.ZTP", currentAddress, &entities[0], false);
}

//	Drawing objects (every frame)
void my_draw(void)
{
    slPushMatrix();		//	Advance matrix buffer pointer 1 step
	slTranslate((xDist), (yDist), (zDist));		//	Translate draw location
	slRotY((yy));								//	Rotate draw location
	jo_3d_set_scale(scale3D,scale3D,scale3D);	//	Sets scale of 3D draw, larger numbers draw bigger
	display_model(&entities[0]);				//	Displays model
	slPopMatrix();		//	Return matrix buffer pointer back 1 step

	//	Print polygon info
	//slPrintFX(toFIXED(jo_3d_get_polygon_count()), slLocate(0,1));
    //slPrintFX(toFIXED(jo_3d_get_vertices_count()), slLocate(0,2));
    //slPrintFX(toFIXED(jo_3d_get_displayed_polygon_count()), slLocate(0,3));
	//slPrintFX(toFIXED(framerate), slLocate(0,4));

    return;
}

//  Handle Input from Gamepad
void gamepad_input(void)
{
	int wait_frames = 15;

	if (!jo_is_pad1_available())
		return ;
	if (jo_is_pad1_key_pressed(JO_KEY_UP))
		zDist -= toFIXED(10.0);
    if (jo_is_pad1_key_pressed(JO_KEY_DOWN))
		zDist += toFIXED(10.0);
    if (jo_is_pad1_key_pressed(JO_KEY_L))
		yy += (DEGtoANG(1.0));
    if (jo_is_pad1_key_pressed(JO_KEY_R))
		yy -= (DEGtoANG(1.0));
	if (jo_is_pad1_key_pressed(JO_KEY_Z))
		yDist += toFIXED(10.0);
    if (jo_is_pad1_key_pressed(JO_KEY_C))
		yDist -= toFIXED(10.0);
	if (jo_is_pad1_key_pressed(JO_KEY_LEFT))
		xDist -= toFIXED(10.0);
    if (jo_is_pad1_key_pressed(JO_KEY_RIGHT))
		xDist += toFIXED(10.0);
}

//  Main Logic Loop
void main_loop(void)
{
	while(1)
	{
		//	Pop matrix to unit matrix at pointer '0'
		slUnitMatrix(0);	
		
		// Display frame number
		framenum++;
		old_tick = tick;
		tick = jo_get_ticks();
		float fps = 1000.0/(tick - old_tick);
		//jo_printf(0, 0, "FPS:%3.2f", fps);

		//  Handle Input
		gamepad_input();

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

	//	Added by XL2 
	fadeOut(true);			//	Fade out screen and blank background
    SynchConst = (Sint8)1; 	//	Framerate control. 1/60 = 60 FPS, 2/60 = 30 FPS, etc.
	framerate = 1;			//	Repeat framerate definition
	slZdspLevel(7);			//	Define frustrum culling near plane
    /****/

	/**XL2**/
	ztCDinit(); 	//	Prepare file system for loading things in directory
	load_ref();		//	Load 3D Object
	slScrAutoDisp(NBG0ON | NBG1ON | NBG3ON | SPRON);	//	?

	fadeIn();	//	Smooth background fade in

	main_loop();	//	Main loop of game
}
