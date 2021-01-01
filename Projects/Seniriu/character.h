/*
** Seniriu
** Matthew Suttinger & Emerald Nova (emeraldnovagames@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

#ifndef __CHARACTER_H__
#define __CHARACTER_H__

//	Include
#include <jo/jo.h>
#include "physics.h"
#include "room.h"

//	Define
#define MAX_CHARACTERS (128)

//	Structs
typedef struct {
	/*
		Character
		
		Contains information relevant to character physics and statistics
	*/
	
	//	Flags
	bool alive;
	bool render;
	bool sneaking;
	
	//	Angle character is facing (0 = Z+)
	ANGLE theta[XYZ];
	//	Position (works as the "feet" of the character)
	FIXED pos[XYZ];
	//	Velocity
	FIXED vel[XYZ];
	//	Acceleration
	FIXED acc[XYZ];
	//	Next Position
	FIXED pos_next[XYZ];
	//	Next Velocity
	FIXED vel_next[XYZ];
	
	//	Current room
	int room[XYZ];
	
	//	Plane collision status
	bool colliding;
	plane * p;
	FIXED pos_collide[XYZ];
	FIXED dt_collide;
	
	//	Skills
	FIXED H2H;
	FIXED BlS;
	FIXED BlL;
	FIXED Blu;
	FIXED Pol;
	FIXED RAS;
	FIXED RAL;
	FIXED Shi;
	FIXED LAr;
	FIXED MAr;
	FIXED HAr;
	FIXED Ath;
	FIXED Ste;
	FIXED Itr;
	FIXED Com;
	FIXED Eng;
	FIXED Spe;
	FIXED MaH;
	FIXED MaC;
	FIXED MaE;
	FIXED MaR;
	FIXED MaP;
	FIXED MaG;
	FIXED Rel;
	
	//	Exertion Statistics
	FIXED HealthP;
	FIXED StaminaP;
	FIXED MagicP;
} character;


//	Variables
extern plane test_plane;
extern character player;
extern character character_list[MAX_CHARACTERS];


//	Functions
void init_characters(void);
void character_physics(character * ch);
void acc_character(character * ch, FIXED acc[XYZ]);
FIXED get_at(character * ch, int at);

#endif
