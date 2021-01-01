/*
** Seniriu
** Matthew Suttinger & Emerald Nova (emeraldnovagames@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/



//	Includes
#include "item.h"

//	Variables
//	In game currency
unsigned int credits;				
//	Master list of items
item item_list[MAX_ITEMS];	
//	Player Inventory, first column item_list index, second column quantity
unsigned int inventory[MAX_INVENTORY][2];
//	Equipment flag, item_list index of equipped item, -1 is unequipped, index 0 left hand, index 1 right hand
int equip[2];

//	Functions
void init_items(void)
{
	/*
		Initialize Items
		
		Initializes item values
	*/
	
	int index = 0;
	
	//	Saber
		//	ID
		item_list[index].ID = 0;
		// Position offset when equipped
		item_list[index].pos_equip[X] = toFIXED(0.0);	
		item_list[index].pos_equip[Y] = toFIXED(1.5);	
		item_list[index].pos_equip[Z] = toFIXED(1.8);
		// Position offset when previewed on HUD
		item_list[index].pos_HUD[X] = toFIXED(0.0);	
		item_list[index].pos_HUD[Y] = toFIXED(1.5);	
		item_list[index].pos_HUD[Z] = toFIXED(1.8);
		// Rotation when equipped
		item_list[index].theta_equip[X] = DEGtoANG(0.0);	
		item_list[index].theta_equip[Y] = DEGtoANG(180.0);	
		item_list[index].theta_equip[Z] = DEGtoANG(0.0);
		// Rotation when previewed on HUD
		item_list[index].theta_HUD[X] = DEGtoANG(0.0);	
		item_list[index].theta_HUD[Y] = DEGtoANG(180.0);	
		item_list[index].theta_HUD[Z] = DEGtoANG(0.0);
		//  Set Scale
		item_list[index].scale[X] = toFIXED(1.0);	
		item_list[index].scale[Y] = toFIXED(1.0);	
		item_list[index].scale[Z] = toFIXED(1.0);
		//	Entity ID
		item_list[index].entity_ID = item_start + 2*0;
		//	Animation Control
		item_list[index].ani_con.currentAni = 1;
		item_list[index].ani_con.currentFrm = 0;
		item_list[index].ani_con.currentKeyFrm = 0;
		item_list[index].ani_start = toFIXED(0.0);
		//	Animation 0
		item_list[index].ani_con.last_frame[0] = (1)*3 - 1;
		item_list[index].ani_time[0] = toFIXED(0.0);
		for(int j = 0; j < 1; j++)
		{
			item_list[index].ani_con.frames[0][3*j] = j;
			item_list[index].ani_con.frames[0][3*j+1] = j;
			item_list[index].ani_con.frames[0][3*j+2] = j;
		}		
		//	Animation 1
		item_list[index].ani_con.last_frame[1] = (39)*3 - 1;
		item_list[index].ani_time[1] = toFIXED(3.9);
		for(int j = 0; j < 39; j++)
		{
			item_list[index].ani_con.frames[1][3*j] = j;
			item_list[index].ani_con.frames[1][3*j+1] = j;
			item_list[index].ani_con.frames[1][3*j+2] = j;
		}		
	
	//	Phaser
		index++;
		//	ID
		item_list[index].ID = 1;
		// Position offset when equipped
		item_list[index].pos_equip[X] = toFIXED(0.6);	
		item_list[index].pos_equip[Y] = toFIXED(0.15);	
		item_list[index].pos_equip[Z] = toFIXED(0.0);
		// Position offset when previewed on HUD
		item_list[index].pos_HUD[X] = toFIXED(0.6);	
		item_list[index].pos_HUD[Y] = toFIXED(0.15);	
		item_list[index].pos_HUD[Z] = toFIXED(0.0);
		// Rotation when equipped
		item_list[index].theta_equip[X] = DEGtoANG(0.0);	
		item_list[index].theta_equip[Y] = DEGtoANG(0.0);	
		item_list[index].theta_equip[Z] = DEGtoANG(0.0);
		// Rotation when previewed on HUD
		item_list[index].theta_HUD[X] = DEGtoANG(0.0);	
		item_list[index].theta_HUD[Y] = DEGtoANG(0.0);	
		item_list[index].theta_HUD[Z] = DEGtoANG(0.0);
		//  Set Scale
		item_list[index].scale[X] = toFIXED(1.0);	
		item_list[index].scale[Y] = toFIXED(1.0);	
		item_list[index].scale[Z] = toFIXED(1.0);
		//	Entity ID
		item_list[index].entity_ID = item_start + 2*1;
		//	Animation Control
		item_list[index].ani_con.currentAni = 1;
		item_list[index].ani_con.currentFrm = 0;
		item_list[index].ani_con.currentKeyFrm = 0;
		item_list[index].ani_start = toFIXED(0.0);
		//	Animation 0
		item_list[index].ani_con.last_frame[0] = (1)*3 - 1;
		item_list[index].ani_time[0] = toFIXED(0.0);
		for(int j = 0; j < 1; j++)
		{
			item_list[index].ani_con.frames[0][3*j] = j;
			item_list[index].ani_con.frames[0][3*j+1] = j;
			item_list[index].ani_con.frames[0][3*j+2] = j;
		}		
		//	Animation 0
		item_list[index].ani_con.last_frame[1] = (20)*3 - 1;
		item_list[index].ani_time[1] = toFIXED(2.0);
		for(int j = 0; j < 20; j++)
		{
			item_list[index].ani_con.frames[1][3*j] = j;
			item_list[index].ani_con.frames[1][3*j+1] = j;
			item_list[index].ani_con.frames[1][3*j+2] = j;
		}
		
	
	//	Hardcode equipment
	equip[0] = 1;
	equip[1] = 0;
		
		
	return;
}

void draw_equipped(void)
{
	/*
		Draw Equipped
		
		Draws equipment on screen
	*/
	
	//	Left Hand
	if(equip[0] >= 0)
	{
		//	Advance matrix buffer pointer 1 step
		slPushMatrix();		
			
		//	Translate draw location
		slTranslate(-item_list[equip[0]].pos_equip[X]*MODEL_SCALE,
					item_list[equip[0]].pos_equip[Y]*MODEL_SCALE,
					item_list[equip[0]].pos_equip[Z]*MODEL_SCALE);		
		//	Rotate object
		slRotY(-item_list[equip[0]].theta_equip[Y]);	
		//	Sets scale of 3D draw, larger numbers draw bigger		
		slScale(-item_list[equip[0]].scale[X], item_list[equip[0]].scale[Y], item_list[equip[0]].scale[Z]);	

		//	Animation
		display_animated(item_list[equip[0]].entity_ID, &item_list[equip[0]].ani_con);
		
		slPopMatrix();		//	Return matrix buffer pointer back 1 step
	}
	
	//	Right Hand
	if(equip[1] >= 0)
	{
		//	Advance matrix buffer pointer 1 step
		slPushMatrix();		
			
		//	Translate draw location
		slTranslate(item_list[equip[1]].pos_equip[X]*MODEL_SCALE,
					item_list[equip[1]].pos_equip[Y]*MODEL_SCALE,
					item_list[equip[1]].pos_equip[Z]*MODEL_SCALE);		
		//	Rotate object
		slRotY(-item_list[equip[1]].theta_equip[Y]);	
		//	Sets scale of 3D draw, larger numbers draw bigger		
		slScale(item_list[equip[1]].scale[X], item_list[equip[1]].scale[Y], item_list[equip[1]].scale[Z]);	

		//	Animation
		display_animated(item_list[equip[1]].entity_ID, &item_list[equip[1]].ani_con);
		
		slPopMatrix();		//	Return matrix buffer pointer back 1 step
	}
	
	return;
}


