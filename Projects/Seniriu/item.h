/*
** Seniriu
** Matthew Suttinger & Emerald Nova (emeraldnovagames@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

#ifndef __ITEM_H__
#define __ITEM_H__

//	Includes
#include <jo/jo.h>
#include "loading.h"
#include "display.h"
#include "animate.h"

//	Define
#define MAX_ITEMS (10)
#define MAX_INVENTORY (2)

//	Structs
typedef struct {
	/*
		Item
		
		Player usable item
	*/
	
	//	ID
	int ID;

    // Position offset when equipped
	FIXED pos_equip[XYZ];	
	// Position offset when previewed on HUD
	FIXED pos_HUD[XYZ];	
    // Rotation when equipped
	ANGLE theta_equip[XYZ];
	// Rotation when previewed on HUD
	ANGLE theta_HUD[XYZ];
	//  Set Scale
	FIXED scale[XYZ];
	
	//	Entity ID
	int entity_ID;
	
	//	Animation Control
	animationControl ani_con;
	//	Time to play animation before defaulting to animation 0 (0 loops)
	FIXED ani_time[MAX_ANI];
	//	Start time of current animation
	FIXED ani_start;
	
} item;

//	Variables
//	In game currency
extern unsigned int credits;				
//	Master list of items
extern item item_list[MAX_ITEMS];	
//	Player Inventory, first column item_list index, second column quantity
extern unsigned int inventory[MAX_INVENTORY][2];
//	Equipment flag, item_list index of equipped item, -1 is unequipped, index 0 left hand, index 1 right hand
extern int equip[2];

//	Functions
void init_items(void);
void draw_equipped(void);

#endif
