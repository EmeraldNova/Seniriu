/*
** Seniriu
** Matthew Suttinger & Emerald Nova (emeraldnovagames@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/



//	Includes
#include "bg.h"

//	Variables
jo_img_8bits bg_img;	//	BG Image3 data
jo_palette bg_pal;		//	BG Palette

//	Functions
jo_palette *bg_tga_palette_handling(void)
{
	/*
		Background TGA Palette Handling
		
		Initilized palette handler for backgrounds
	*/
	
    jo_create_palette(&bg_pal);
    return (&bg_pal);
}

void bg_init(int index)
{
	/*
		BG Initilization
		
		Initilized all required backgroudn settings
	*/
	
	//	Set palette handler
	jo_set_tga_palette_handling(bg_tga_palette_handling);
	
	//	Load background and initialize planes
	load_bg(index);
	
	return;
}

void load_bg(int index)
{
	/*
		Load Background
		
		Loads background as 8bit TGA and initialize planes
	*/
	
	//	ENable 3D background plane
	jo_enable_background_3d_plane(JO_COLOR_Black);
	
	//	Pick file to load
	if(index == 0)
	{
		index = index;
	}
	
	//
    bg_img.data = JO_NULL;
    jo_tga_8bits_loader(&bg_img, "BG", "SKY.TGA", 0);
    jo_background_3d_plane_b_img(&bg_img, bg_pal.id, true, false);
    jo_free_img(&bg_img);
	
	return;
}

void draw_bg(void)
{
	/*
		Draw Background
		
		Draws background with rotation
	*/
	
	//SKY
	jo_3d_push_matrix();
	{
	    //jo_3d_translate_matrix(rot.rz * 100, 110 - 112 * //jo_cos_radf(rot.rx), 100);
        jo_3d_rotate_matrix_z(-theta[Y]/182);
        jo_background_3d_plane_b_draw(false);
	}
	jo_3d_pop_matrix();
	
	return;
}



