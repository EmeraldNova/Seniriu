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

void test2D_init(void)
{
	//	Malloc points
	//FIXED * points1 = jo_malloc(6 * sizeof(FIXED[XYZ]));
	//FIXED * points2 = jo_malloc(6 * sizeof(FIXED[XYZ]));
	
	//	Setup fields
	field1.nbPoint = 12;
	field1.verts = (FIXED*) jo_malloc(field1.nbPoint * 2 * sizeof(FIXED));
	field1.connections = (int*) jo_malloc(field1.nbPoint * sizeof(int));
	field2.nbPoint = 4;
	field2.verts = (FIXED*) jo_malloc(field2.nbPoint * 2 * sizeof(FIXED));
	field2.connections = (int*) jo_malloc(field2.nbPoint * sizeof(int));
	
	//	Define Points
	*(field1.verts +2*0 + 0) = 100 << 16;
	*(field1.verts +2*0 + 1) = 100 << 16;
	*(field1.verts +2*1 + 0) = 200 << 16;
	*(field1.verts +2*1 + 1) = 100 << 16;
	*(field1.verts +2*2 + 0) = 150 << 16;
	*(field1.verts +2*2 + 1) = 150 << 16;
	*(field1.verts +2*3 + 0) = 150 << 16;
	*(field1.verts +2*3 + 1) = 50 << 16;
	*(field1.verts +2*4 + 0) = 125 << 16;
	*(field1.verts +2*4 + 1) = 100 << 16;
	*(field1.verts +2*5 + 0) = 150 << 16;
	*(field1.verts +2*5 + 1) = 125 << 16;
	*(field1.verts +2*6 + 0) = 56 << 16;
	*(field1.verts +2*6 + 1) = 83 << 16;
	*(field1.verts +2*7 + 0) = 90 << 16;
	*(field1.verts +2*7 + 1) = 45 << 16;
	*(field1.verts +2*8 + 0) = 120 << 16;
	*(field1.verts +2*8 + 1) = 166 << 16;
	*(field1.verts +2*9 + 0) = 76 << 16;
	*(field1.verts +2*9 + 1) = 23 << 16;
	*(field1.verts +2*10 + 0) = 43 << 16;
	*(field1.verts +2*10 + 1) = 98 << 16;
	*(field1.verts +2*11 + 0) = 77 << 16;
	*(field1.verts +2*11 + 1) = 55 << 16;
	field1.connections[0] = 10;
	field1.connections[1] = 10;
	field1.connections[2] = 10;
	field1.connections[3] = 10;
	field1.connections[4] = 10;
	field1.connections[5] = 10;
	field1.connections[6] = 10;
	field1.connections[7] = 10;
	field1.connections[8] = 10;
	field1.connections[9] = 10;
	field1.connections[10] = 10;
	field1.connections[11] = 10;
	convex_hull(&field1);
	
	*(field2.verts +2*0 + 0) = 150 << 16;
	*(field2.verts +2*0 + 1) = 150 << 16;
	*(field2.verts +2*1 + 0) = 250 << 16;
	*(field2.verts +2*1 + 1) = 150 << 16;
	*(field2.verts +2*2 + 0) = 250 << 16;
	*(field2.verts +2*2 + 1) = 239 << 16;
	*(field2.verts +2*3 + 0) = 150 << 16;
	*(field2.verts +2*3 + 1) = 239 << 16;
	field2.connections[0] = 0;
	field2.connections[1] = 1;
	field2.connections[2] = 2;
	field2.connections[3] = 3;
	
	test2D_loop();
}

void test2D_loop(void)
{
	while(true)
	{
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
		
		slSynch();	
	}
}
