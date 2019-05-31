/*
** Seniriu
** Matthew Suttinger & Emerald Nova (emeraldnovagames@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

#include "test2D.h"
#include "separate.h"
#include "game_object.h"
#include <jo/jo.h>
#include "ZT/ZT_COMMON.h"

field_2D field1;
field_2D field2;
FIXED point[2][2];
int dx = 0;
int dy = 0;

//	Variable initializations
FIXED normal[2];
FIXED scaled[2];
FIXED extent1[2];
FIXED extent2[2];

void test2D_init(void)
{
	//	Setup fields
	field1.nbPoint = 4;
	field1.verts = (FIXED*) jo_malloc(field1.nbPoint * 2 * sizeof(FIXED));
	field1.connections = (int*) jo_malloc(field1.nbPoint * sizeof(int));
	field2.nbPoint = 4;
	field2.verts = (FIXED*) jo_malloc(field2.nbPoint * 2 * sizeof(FIXED));
	field2.connections = (int*) jo_malloc(field2.nbPoint * sizeof(int));
}

void test2D_reset(void)
{
	//	Define Points
	*(field1.verts +2*0 + 0) = (50 + dx) << 16;
	*(field1.verts +2*0 + 1) = (50 + dy) << 16;
	*(field1.verts +2*1 + 0) = (50 + dx) << 16;
	*(field1.verts +2*1 + 1) = (100 + dy) << 16;
	*(field1.verts +2*2 + 0) = (100 + dx) << 16;
	*(field1.verts +2*2 + 1) = (100 + dy) << 16;
	*(field1.verts +2*3 + 0) = (100 + dx) << 16;
	*(field1.verts +2*3 + 1) = (50 + dy) << 16;
	field1.connections[0] = 0;
	field1.connections[1] = 0;
	field1.connections[2] = 0;
	field1.connections[3] = 0;
	convex_hull(&field1);
	
	*(field2.verts +2*0 + 0) = 50 << 16;
	*(field2.verts +2*0 + 1) = 150 << 16;
	*(field2.verts +2*1 + 0) = 150 << 16;
	*(field2.verts +2*1 + 1) = 150 << 16;
	*(field2.verts +2*2 + 0) = 100 << 16;
	*(field2.verts +2*2 + 1) = 200 << 16;
	*(field2.verts +2*3 + 0) = 100 << 16;
	*(field2.verts +2*3 + 1) = 100 << 16;
	field2.connections[0] = 0;
	field2.connections[1] = 0;
	field2.connections[2] = 0;
	field2.connections[3] = 0;
	convex_hull(&field2);
}

void test2D_loop(void)
{
	while(true)
	{
		//	Reset with start
		if(jo_is_pad1_key_pressed(JO_KEY_UP))
		{
			dy = dy + 2;
		}
		if(jo_is_pad1_key_pressed(JO_KEY_DOWN))
		{
			dy = dy - 2;
		}
		if(jo_is_pad1_key_pressed(JO_KEY_RIGHT))
		{
			dx = dx + 2;
		}
		if(jo_is_pad1_key_pressed(JO_KEY_LEFT))
		{
			dx = dx - 2;
		}
		if(jo_is_pad1_key_pressed(JO_KEY_START))
		{
			dx = 0;
			dy = 0;
		}
		jo_clear_background(JO_COLOR_Black);
		test2D_reset();
		
		
		// Field 1 Connections
		for(int i = 0; i < (field1.nbPoint - 1); i++)
		{
			jo_draw_background_line(*(field1.verts + 2* field1.connections[i]) >> 16,
									239 - (*(field1.verts + 2* field1.connections[i] + 1) >> 16),
									*(field1.verts + 2* field1.connections[i+1]) >> 16,
									239 - (*(field1.verts + 2* field1.connections[i+1] + 1) >> 16),
									JO_COLOR_Blue);
		}
		//	Draw last connection
		jo_draw_background_line(*(field1.verts + 2* field1.connections[field1.nbPoint - 1]) >> 16,
									239 - (*(field1.verts + 2* field1.connections[field1.nbPoint - 1] + 1) >> 16),
									*(field1.verts + 2* field1.connections[0]) >> 16,
									239 - (*(field1.verts + 2* field1.connections[0] + 1) >> 16),
									JO_COLOR_Blue);
		
		// Field 2 Connections
		for(int i = 0; i < field2.nbPoint - 1; i++)
		{
			jo_draw_background_line(*(field2.verts + 2* field2.connections[i] + 0) >> 16,
									239 - (*(field2.verts + 2* field2.connections[i] + 1) >> 16),
									*(field2.verts + 2* field2.connections[i+1] + 0) >> 16,
									239 - (*(field2.verts + 2* field2.connections[i+1] + 1) >> 16),
									JO_COLOR_Red);
		}
		//	Draw last connection
		jo_draw_background_line(*(field2.verts + 2* field2.connections[field2.nbPoint - 1] + 0) >> 16,
									239 - (*(field2.verts + 2* field2.connections[field2.nbPoint - 1] + 1) >> 16),
									*(field2.verts + 2* 0 + 0) >> 16,
									239 - (*(field2.verts + 2* 0 + 1) >> 16),
									JO_COLOR_Red);
		
		//	Field 1 points
		for(int i = 0; i < field1.nbPoint; i++)
		{
			jo_put_pixel_in_background(*(field1.verts + 2* i + 0) >> 16,
										239 - (*(field1.verts + 2* i + 1) >> 16),
										JO_COLOR_White);
		}
		
		//	Field 2 points
		for(int i = 0; i < field2.nbPoint; i++)
		{
			jo_put_pixel_in_background(*(field2.verts + 2* i + 0) >> 16,
										239 - (*(field2.verts + 2* i + 1) >> 16),
										JO_COLOR_Green);
		}
		
		//	Test for collision and print result
		if(separate_2D(&field1, &field2))
		{
			slPrint("Collision!     ",slLocate(0,0));
		}
		else
		{
			slPrint("Not colliding. ",slLocate(0,0));
		}
		
		
		slSynch();	
	}
}
