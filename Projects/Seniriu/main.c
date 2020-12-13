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

#include "main.h"

//VECTOR lighting_direction = {0, 32767, 0};


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
void load_ref(void)
{
	/*
		Load References
		
		Loads 3D models
	*/
	
	//	Start loading in LWRAM beginning
    void * currentAddress = (void*)LWRAM;
	
	//	File names
	#define NUM_FILES  (5)
	const Sint8 * filenames[NUM_FILES] = 
	{
		"FLOOR.ZTP",
		"CEIL.ZTP",
		"CORNER.ZTP",
		"DOOR.ZTP",
		"WINDOW.ZTP"
	};
	
	
	//	Load ZTP file
	for(int ent_ID = 0; ent_ID < NUM_FILES; ent_ID++)
	{
		currentAddress = ztLoad3Dmodel((Sint8*)filenames[ent_ID], currentAddress, ent_ID, false);
		
		//	Rectify vertex directions
		//	Number of quads in model
		int num_quads = entities[ent_ID].pol[0]->nbPolygon;
		//	Number of vertices in model
		int num_verts = entities[ent_ID].pol[0]->nbPoint;
		//	Pointer to point table
		POINT * pntbl_ptr = (entities[ent_ID].pol[0])->pntbl;
		//	Pointer to Polygon table
		POLYGON * pltbl_ptr = (entities[ent_ID].pol[0])->pltbl;
		
		//	Normal Components Absolute Value
		int norm_copy_abs[XYZ];
		//	Maximum absolute value of normal components
		int max_component = 0;
		
		//	Start loop
		for (int i = 0; i < num_quads; i++)
		{
			//	Assign vertices
			Uint16 * const vert_ID = pltbl_ptr[i].Vertices;
			
			//	Normal Components
			FIXED * const norm_copy = pltbl_ptr[i].norm;
			
			//	Number of permutations needed to sort
			int perm_num = 0;
			
			//	Find chief axis
			norm_copy_abs[X] = JO_ABS(norm_copy[X]);
			norm_copy_abs[Y] = JO_ABS(norm_copy[Y]);
			norm_copy_abs[Z] = JO_ABS(norm_copy[Z]);
			max_component = JO_MAX(JO_MAX(norm_copy_abs[X], norm_copy_abs[Y]), norm_copy_abs[Z]);
			
			//	Align directions
			int left_dir[XYZ] = {0, 0, 0};
			int up_dir[XYZ] = {0, 0, 0};
			
			//	Disable lighting
			entities[ent_ID].pol[0]->attbl[i].sort ^= UseLight;
			
			//	Determine Chief Normal
			int norm;
			if((max_component == norm_copy_abs[X]) &&
				(norm_copy[X] < 0))
			{
				//	-X; Z left, -Y up
				norm = -1;
				left_dir[Z] = 1;
				up_dir[Y] = -1;
				
				//	Set aligned textures to mesh display
				//entities[ent_ID].pol[0]->attbl[i].atrb |= MESHon;
			}
			else if((max_component == norm_copy_abs[X]) &&
					(norm_copy[X] > 0))
			{
				//	X; -Z left, -Y up
				norm = 1;
				left_dir[Z] = -1;
				up_dir[Y] = -1;
			}
			else if((max_component == norm_copy_abs[Y]) &&
					(norm_copy[Y] > 0))
			{
				//	Y; -X left, -Z up
				norm = 2;
				left_dir[X] = -1;
				up_dir[Z] = -1;
			}
			else if((max_component == norm_copy_abs[Z]) &&
					(norm_copy[Z] < 0))
			{
				//	-Z; -X left, -Y up
				norm = -3;
				left_dir[X] = -1;
				up_dir[Y] = -1;
				
			}
			else if((max_component == norm_copy_abs[Z]) &&
					(norm_copy[Z] > 0))
			{
				//	Z; X left, -Y up
				norm = 3;
				left_dir[X] = 1;
				up_dir[Y] = -1;
			}
			else
			{
				//	-Y (Default); -X left, Z up
				norm = -2;
				left_dir[X] = -1;
				up_dir[Z] = 1;
				
				
			}
			
			//	Retrieve two most "Up" points
			FIXED dif[XYZ];
			FIXED difs[4];
			for(int i = 0; i < 4; i++)
			{
				//	Calculate difference
				dif[X] = pntbl_ptr[vert_ID[i]][X] -
						   pntbl_ptr[vert_ID[0]][X];
				dif[Y] = pntbl_ptr[vert_ID[i]][Y] -
						   pntbl_ptr[vert_ID[0]][Y];
				dif[Z] = pntbl_ptr[vert_ID[i]][Z] -
						   pntbl_ptr[vert_ID[0]][Z];
				
				//	Dot product with up direction
				difs[i] = up_dir[X]*dif[X] +
						  up_dir[Y]*dif[Y] +
						  up_dir[Z]*dif[Z];
			}
			//	Sort by most "Up"
			int up_index[2] = {0, 1};
			int hold_dex;
			for(int i = 1; i < 4; i++)
			{
				if(difs[i] > difs[up_index[1]])
				{
					//	Overwrite second highest
					up_index[1] = i;
				}
				if(difs[up_index[1]] > difs[up_index[0]])
				{
					//	Sort the top 2
					hold_dex = up_index[1];
					up_index[1] = up_index[0];
					up_index[0] = hold_dex;
				}
			}
			
			//	Find most "Left" of the two most "Up" verts
			//	Calculate difference
			dif[X] = pntbl_ptr[vert_ID[up_index[0]]][X] -
					   pntbl_ptr[vert_ID[up_index[1]]][X];
			dif[Y] = pntbl_ptr[vert_ID[up_index[0]]][Y] -
					   pntbl_ptr[vert_ID[up_index[1]]][Y];
			dif[Z] = pntbl_ptr[vert_ID[up_index[0]]][Z] -
					   pntbl_ptr[vert_ID[up_index[1]]][Z];
			//	Dot product with left direction
			difs[0] = left_dir[X]*dif[X] +
					  left_dir[Y]*dif[Y] +
					  left_dir[Z]*dif[Z];
					  
			//	Check for which index is most left
			if(difs[0] >= 0)
			{
				perm_num = up_index[0];
			}
			else
			{
				perm_num = up_index[1];
			}
			
			//	Temporary permutation vertex variable
			Uint16 * tmpVert;
			
			//	Permute (rotate) vertex indices until reoriented
			for(int j = 0; j < perm_num; j++)
			{
				tmpVert = pltbl_ptr[i].Vertices[0];
				pltbl_ptr[i].Vertices[0] = pltbl_ptr[i].Vertices[1];
				pltbl_ptr[i].Vertices[1] = pltbl_ptr[i].Vertices[2];
				pltbl_ptr[i].Vertices[2] = pltbl_ptr[i].Vertices[3];
				pltbl_ptr[i].Vertices[3] = tmpVert;
			}
		}
	}
	
	return;
}


//	Drawing objects (every frame)
void my_draw(void)
{	
	/*
		My Draw
		
		Draws all objects
	*/
	
	//	Draw background
	//draw_bg();
	
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

		//	Lighting direction
		//slLight(lighting_direction);
		
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
	
	

	//	Load assets
	load_ref();		//	Load 3D Object
	//load_rooms();	//	Load in rooms
	
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
