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
int HUD_spid;			//	HUD Jo Image sprite ID

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
	
	//	Enable 3D background plane
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
        jo_3d_rotate_matrix_z(-player.theta[Y]/182);
        jo_background_3d_plane_b_draw(false);
	}
	jo_3d_pop_matrix();
	
	return;
}

void load_HUD(void)
{
	/*
		Load HUD
		
		Load in HUD TGAs
	*/
	
	int dummy;
	HUD_spid = jo_sprite_add_tga("BG", "HUDFULL.TGA", JO_COLOR_Blue);
	dummy = jo_sprite_add_tga("BG", "HEALTHL.TGA", JO_COLOR_Blue);
	dummy = jo_sprite_add_tga("BG", "HEALTHH.TGA", JO_COLOR_Blue);
	dummy = jo_sprite_add_tga("BG", "STAML.TGA", JO_COLOR_Blue);
	dummy = jo_sprite_add_tga("BG", "STAMH.TGA", JO_COLOR_Blue);
	dummy = jo_sprite_add_tga("BG", "MAGICL.TGA", JO_COLOR_Blue);
	dummy = jo_sprite_add_tga("BG", "MAGICH.TGA", JO_COLOR_Blue);
	
	return;
}

void draw_HUD(void)
{
	/*
		Draw HUD
		
		Draw HUD on screen
	*/
	
	jo_sprite_draw3D(HUD_spid, 		-1, -(224-46)/2-1, 5);
	jo_sprite_draw3D(HUD_spid + 1, 	-106, -(224)/2+4, 12);
	jo_sprite_draw3D(HUD_spid + 2,
		-106-(
		(96*((get_at(&player, 9)-player.HealthP)>>16 ))/
		(get_at(&player, 9)>>16) ),
		-(224)/2+4, 11);
	jo_sprite_draw3D(HUD_spid + 3, 	0, -(224)/2+2, 14);
	jo_sprite_draw3D(HUD_spid + 4, 	
		0-(
		(96*((get_at(&player, 10)-player.StaminaP)>>16 ))/
		(get_at(&player, 10)>>16) ),
		-(224)/2+2, 13);
	jo_sprite_draw3D(HUD_spid + 5, 	106, -(224)/2+4, 16);
	jo_sprite_draw3D(HUD_spid + 6, 	
		106-(
		(96*((get_at(&player, 11)-player.MagicP)>>16 ))/
		(get_at(&player, 11)>>16) ),
		-(224)/2+4, 15);
	
	//	Test Print
	/*
	slPrint("Health:               /             ",slLocate(0,3));
	slPrintFX(player.HealthP, slLocate(9,3));
	slPrintFX(get_at(&player, 9), slLocate(24,3));
	slPrint("Stamina:              /             ",slLocate(0,4));
	slPrintFX(player.StaminaP,slLocate(9,4));
	slPrintFX(get_at(&player, 10),slLocate(24,4));
	slPrint("Magic:                /             ",slLocate(0,5));
	slPrintFX(player.MagicP,slLocate(9,5));
	slPrintFX(get_at(&player, 11),slLocate(24,5));
	slPrint("Delta Time:                             ",slLocate(0,6));
	slPrintFX(delta_time,slLocate(13,6));
	slPrint("Intelligence:                             ",slLocate(0,7));
	slPrintFX(get_at(&player, 4),slLocate(13,7));
	slPrint("Product/4:                             ",slLocate(0,8));
	slPrintFX(((jo_fixed_mult(delta_time, 
		get_at(&player, 4)))>>2),slLocate(13,8));
	slPrintFX(get_at(&player, 0) ,slLocate(0,9));	
	slPrintFX(get_at(&player, 1) ,slLocate(13,9));
	slPrintFX(get_at(&player, 2) ,slLocate(26,9));
	slPrintFX(get_at(&player, 3) ,slLocate(0,10));	
	slPrintFX(get_at(&player, 4) ,slLocate(13,10));
	slPrintFX(get_at(&player, 5) ,slLocate(26,10));
	slPrintFX(get_at(&player, 6) ,slLocate(0,11));	
	slPrintFX(get_at(&player, 7) ,slLocate(13,11));
	slPrintFX(get_at(&player, 8) ,slLocate(26,11));
	*/
	
	return;
}

