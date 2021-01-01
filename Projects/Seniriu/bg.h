/*
** Seniriu
** Matthew Suttinger & Emerald Nova (emeraldnovagames@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

#ifndef __BG_H__
#define __BG_H__

//	Includes
#include <jo/jo.h>
#include "input.h"
#include "character.h"

//	Variables
extern jo_img_8bits bg_img;
extern jo_palette bg_pal;
extern int HUD;

//	Functions
jo_palette *bg_tga_palette_handling(void);
void bg_init(int index);
void load_bg(int index);
void draw_bg(void);
void load_HUD(void);
void draw_HUD(void);

#endif
