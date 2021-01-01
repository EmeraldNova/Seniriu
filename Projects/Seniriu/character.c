/*
** Seniriu
** Matthew Suttinger & Emerald Nova (emeraldnovagames@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/


//	Include
#include "character.h"

//	Define
#define MAX_CHARACTERS (128)

//	Variables
plane test_plane;
character player;
character character_list[MAX_CHARACTERS];


//	Functions
void init_character_test(void)
{
	/*
		Initialize Characters
		
		Initialize variables for characters
	*/
	
	/*
	//	Plane
	test_plane.v1[X] =  (6<<16);
	test_plane.v1[Y] =  (0<<16);
	test_plane.v1[Z] = -(6<<16);
	
	test_plane.v2[X] =  (6<<16);
	test_plane.v2[Y] = -(0<<16);
	test_plane.v2[Z] =  (6<<16);
	
	test_plane.v3[X] = -(6<<16);
	test_plane.v3[Y] = -(0<<16);
	test_plane.v3[Z] =  (6<<16);
	
	test_plane.v4[X] = -(6<<16);
	test_plane.v4[Y] =  (0<<16);
	test_plane.v4[Z] = -(6<<16);
	
	generate_plane_info(&test_plane);
	*/
	
	//	Player
	player.alive = true;
	player.sneaking = false;
	
	player.theta[X] = 0;
	player.theta[Y] = 0;
	player.theta[Z] = 0;
	
	player.pos[X] =  (0<<16);
	player.pos[Y] = -(1<<16);
	player.pos[Z] = -(4<<16);
	
	player.vel[X] =  (0<<16);
	player.vel[Y] =  (0<<16);
	player.vel[Z] =  (0<<16);
	
	player.acc[X] =  (0<<16);
	player.acc[Y] =  (0<<16);
	player.acc[Z] =  (0<<16);
	
	player.pos_next[X] =  (0<<16);
	player.pos_next[Y] = -(1<<16);
	player.pos_next[Z] =  -(4<<16);
	
	player.vel_next[X] =  (0<<16);
	player.vel_next[Y] =  (0<<16);
	player.vel_next[Z] =  (0<<16);
	
	player.H2H = 1<<16;
	player.BlS = 1<<16;
	player.BlL = 1<<16;
	player.Blu = 1<<16;
	player.Pol = 1<<16;
	player.RAS = 1<<16;
	player.RAL = 1<<16;
	player.Shi = 1<<16;
	player.LAr = 1<<16;
	player.MAr = 1<<16;
	player.HAr = 1<<16;
	player.Ath = 1<<16;
	player.Ste = 1<<16;
	player.Itr = 1<<16;
	player.Com = 1<<16;
	player.Eng = 1<<16;
	player.Spe = 1<<16;
	player.MaH = 1<<16;
	player.MaC = 1<<16;
	player.MaE = 1<<16;
	player.MaR = 1<<16;
	player.MaP = 1<<16;
	player.MaG = 1<<16;
	player.Rel = 1<<16;
	player.HealthP = 1<<16;
	player.StaminaP = 0;
	player.MagicP = 0;
	
	return;
}

void character_physics(character * ch)
{
	/*
		Character Physics
		
		Iterates through sources of physics interactions then applies it to character
	*/
	
	//	Set new position from previous frame physics
	ch->pos[X] = ch->pos_next[X];
	ch->pos[Y] = ch->pos_next[Y];
	ch->pos[Z] = ch->pos_next[Z];
	
	//	Set new velocity
	ch->vel[X] = ch->vel_next[X];
	ch->vel[Y] = ch->vel_next[Y];
	ch->vel[Z] = ch->vel_next[Z];
	
	//	Project new location
	ch->pos_next[X] = ch->pos[X] + jo_fixed_mult(delta_time, ch->vel[X]) + jo_fixed_mult((jo_fixed_mult(delta_time, delta_time)>>1), ch->acc[X]);
	ch->pos_next[Y] = ch->pos[Y] + jo_fixed_mult(delta_time, ch->vel[Y]) + jo_fixed_mult((jo_fixed_mult(delta_time, delta_time)>>1), ch->acc[Y]);
	ch->pos_next[Z] = ch->pos[Z] + jo_fixed_mult(delta_time, ch->vel[Z]) + jo_fixed_mult((jo_fixed_mult(delta_time, delta_time)>>1), ch->acc[Z]);
	
	//	New Velocity
	ch->vel_next[X] = ch->vel[X] + jo_fixed_mult(ch->acc[X], delta_time);
	ch->vel_next[Y] = ch->vel[Y] + jo_fixed_mult(ch->acc[Y], delta_time);
	ch->vel_next[Z] = ch->vel[Z] + jo_fixed_mult(ch->acc[Z], delta_time);
	
	//	Update current room
	if(ch->pos[X] < -(room_grid[X]>>1) && ch->room[X] > 0)
	{
		//	Going West (X-)
		ch->pos[X] += room_grid[X];
		ch->pos_next[X] += room_grid[X];
		ch->room[X]--;
	}
	else if(ch->pos[X] >= (room_grid[X]>>1) && ch->room[X] < ROOM_NUM_X-1)
	{
		//	Going East (X+)
		ch->pos[X] -= room_grid[X];
		ch->pos_next[X] -= room_grid[X];
		ch->room[X]++;
	}
	if(ch->pos[Y] < -(room_grid[Y]>>1) && ch->room[Y] < ROOM_NUM_Y-1)
	{
		//	Going up (Y-)
		ch->pos[Y] += room_grid[Y];
		ch->pos_next[Y] += room_grid[Y];
		ch->room[Y]++;
	}
	else if(ch->pos[Y] >= (room_grid[Y]>>1) && ch->room[Y] > 0)
	{
		//	Going down (Y+)
		ch->pos[Y] -= room_grid[Y];
		ch->pos_next[Y] -= room_grid[Y];
		ch->room[Y]--;
	}
	if(ch->pos[Z] < -(room_grid[Z]>>1) && ch->room[Z] > 0)
	{
		//	Going South (Z-)
		ch->pos[Z] += room_grid[Z];
		ch->pos_next[Z] += room_grid[Z];
		ch->room[Z]--;
	}
	else if(ch->pos[Z] >= (room_grid[Z]>>1) && ch->room[Z] < ROOM_NUM_Z-1)
	{
		//	Going North (Z+)
		ch->pos[Z] -= room_grid[Z];
		ch->pos_next[Z] -= room_grid[Z];
		ch->room[Z]++;
	}
	
	//	Collision with plane
	ch->colliding = false;
	/*
	for(int i = 0; i < 5; i++)
	{
		if(plane_intersect_check(ch->pos, ch->pos_next, &plane_list[i], ch->pos_collide, &ch->dt_collide) && (i != 2))
		{
			//	Set colliding to true
			ch->colliding = true;
			ch->p = &plane_list[i];
			
			//	Get projected point on plane and intermediate time
			FIXED dt_collide = project_to_plane(ch->pos, ch->vel, &plane_list[i], ch->pos_collide);
			
			//	Continue lateral movement on plane
			FIXED divisor = dot_product(ch->vel_next, plane_list[i].n);
			
			ch->vel_next[X] = ch->vel_next[X] - jo_fixed_mult(divisor, plane_list[i].n[X]);
			ch->vel_next[Y] = ch->vel_next[Y] - jo_fixed_mult(divisor, plane_list[i].n[Y]);
			ch->vel_next[Z] = ch->vel_next[Z] - jo_fixed_mult(divisor, plane_list[i].n[Z]);
			
			ch->pos_next[X] = ch->pos_collide[X] + jo_fixed_mult(ch->vel_next[X], delta_time - dt_collide);
			ch->pos_next[Y] = ch->pos_collide[Y] + jo_fixed_mult(ch->vel_next[Y], delta_time - dt_collide);
			ch->pos_next[Z] = ch->pos_collide[Z] + jo_fixed_mult(ch->vel_next[Z], delta_time - dt_collide);
		}
	}
	*/
	
	//	Hardcoded Axis Aligned Room Collision
	ch->colliding = false;
	FIXED floo_ext = 6<<16;
	FIXED ceil_ext = toFIXED(1.4);
	FIXED door_ext = toFIXED(0.5);
	FIXED wall_ext = toFIXED(2.3);
	//FIXED wall_ext = toFIXED(2.0);
	//	Retireve doorways
	/*
		Index
		0 - North
		1 - East
		2 - South
		3 - West
		4 - Up
		5 - Down
		
		Value
		0 - No Door
		1 - Door
		2 - Window
	*/
	int doors[4] = {0, 0, 0, 0};
	room * rm = &roomMaster[ch->room[X]][ch->room[Y]][ch->room[Z]];
	doors[0] = rm[X].doors[0];
	doors[1] = rm[X].doors[1];
	doors[2] = rm[X].doors[2];
	doors[3] = rm[X].doors[3];
	//	Floor
	if(ch->pos[Y] >= 0 || ch->pos_next[Y] >= 0)
	{
		ch->colliding = true;
		ch->pos[Y] = 0;
		ch->pos_next[Y] = 0;
		ch->vel[Y] = JO_MIN(0,ch->vel[Y]);
		ch->vel_next[Y] = JO_MIN(0,ch->vel_next[Y]);
	}
	//	Ceiling
	if(ch->pos[Y] <= -ceil_ext || ch->pos_next[Y] <= -ceil_ext)
	{
		//ch->colliding = true;
		ch->pos[Y] = -ceil_ext+1;
		ch->pos_next[Y] = -ceil_ext+1;
		ch->vel[Y] = JO_MAX(0,ch->vel[Y]);
		ch->vel_next[Y] = JO_MAX(0,ch->vel_next[Y]);
	}
	//	North
	if(ch->pos[Z] >= wall_ext || ch->pos_next[Z] >= wall_ext)
	{
		if(doors[0] == 1)
		{
			//	Doorway
			if(ch->pos[X] <= -door_ext && ch->pos_next[X] <= -door_ext )
			{
				//	Too Far West
				//ch->colliding = true;
				ch->pos[Z] = wall_ext-1;
				ch->pos_next[Z] = wall_ext-1;
				ch->vel[Z] = JO_MIN(0,ch->vel[Z]);
				ch->vel_next[Z] = JO_MIN(0,ch->vel_next[Z]);
			}
			else if(ch->pos[X] >= door_ext && ch->pos_next[X] >= door_ext )
			{
				//	Too Far East
				//ch->colliding = true;
				ch->pos[Z] = wall_ext-1;
				ch->pos_next[Z] = wall_ext-1;
				ch->vel[Z] = JO_MIN(0,ch->vel[Z]);
				ch->vel_next[Z] = JO_MIN(0,ch->vel_next[Z]);
			}
			else if(ch->pos[X] > -door_ext && ch->pos_next[X] <= -door_ext )
			{
				//	Heading West in Doorway
				//ch->colliding = true;
				ch->pos[X] = 1-door_ext;
				ch->pos_next[X] = 1-door_ext;
				ch->vel[X] = JO_MAX(0,ch->vel[X]);
				ch->vel_next[X] = JO_MAX(0,ch->vel_next[X]);
			}
			else if(ch->pos[X] < door_ext && ch->pos_next[X] >= door_ext )
			{
				//	Heading East in Doorway
				//ch->colliding = true;
				ch->pos[X] = door_ext-1;
				ch->pos_next[X] = door_ext-1;
				ch->vel[X] = JO_MIN(0,ch->vel[X]);
				ch->vel_next[X] = JO_MIN(0,ch->vel_next[X]);
			}
		}
		else
		{
			//	No Doorway
			ch->pos[Z] = wall_ext-1;
			ch->pos_next[Z] = wall_ext-1;
			ch->vel[Z] = 0;
			ch->vel_next[Z] = 0;
		}
	}
	//	East
	if(ch->pos[X] >= wall_ext || ch->pos_next[X] >= wall_ext)
	{
		if(doors[1] == 1)
		{
			//	Doorway
			if(ch->pos[Z] <= -door_ext && ch->pos_next[Z] <= -door_ext )
			{
				//	Too Far South
				//ch->colliding = true;
				ch->pos[X] = wall_ext-1;
				ch->pos_next[X] = wall_ext-1;
				ch->vel[X] = JO_MIN(0,ch->vel[X]);
				ch->vel_next[X] = JO_MIN(0,ch->vel_next[X]);
			}
			else if(ch->pos[Z] >= door_ext && ch->pos_next[Z] >= door_ext )
			{
				//	Too Far North
				//ch->colliding = true;
				ch->pos[X] = wall_ext-1;
				ch->pos_next[X] = wall_ext-1;
				ch->vel[X] = JO_MIN(0,ch->vel[X]);
				ch->vel_next[X] = JO_MIN(0,ch->vel_next[X]);
			}
			else if(ch->pos[Z] > -door_ext && ch->pos_next[Z] <= -door_ext )
			{
				//	Heading South in Doorway
				//ch->colliding = true;
				ch->pos[Z] = 1-door_ext;
				ch->pos_next[Z] = 1-door_ext;
				ch->vel[Z] = JO_MAX(0,ch->vel[Z]);
				ch->vel_next[Z] = JO_MAX(0,ch->vel_next[Z]);
			}
			else if(ch->pos[Z] < door_ext && ch->pos_next[Z] >= door_ext )
			{
				//	Heading North in Doorway
				//ch->colliding = true;
				ch->pos[Z] = door_ext-1;
				ch->pos_next[Z] = door_ext-1;
				ch->vel[Z] = JO_MIN(0,ch->vel[Z]);
				ch->vel_next[Z] = JO_MIN(0,ch->vel_next[Z]);
			}
		}
		else
		{
			//	No Doorway
			ch->pos[X] = wall_ext-1;
			ch->pos_next[X] = wall_ext-1;
			ch->vel[X] = 0;
			ch->vel_next[X] = 0;
		}
	}
	//	South
	if(ch->pos[Z] <= -wall_ext || ch->pos_next[Z] <= -wall_ext)
	{
		if(doors[2] == 1)
		{
			//	Doorway
			if(ch->pos[X] <= -door_ext && ch->pos_next[X] <= -door_ext )
			{
				//	Too Far West
				//ch->colliding = true;
				ch->pos[Z] = 1-wall_ext;
				ch->pos_next[Z] = 1-wall_ext;
				ch->vel[Z] = JO_MAX(0,ch->vel[Z]);
				ch->vel_next[Z] = JO_MAX(0,ch->vel_next[Z]);
			}
			else if(ch->pos[X] >= door_ext && ch->pos_next[X] >= door_ext )
			{
				//	Too Far East
				//ch->colliding = true;
				ch->pos[Z] = 1-wall_ext;
				ch->pos_next[Z] = 1-wall_ext;
				ch->vel[Z] = JO_MAX(0,ch->vel[Z]);
				ch->vel_next[Z] = JO_MAX(0,ch->vel_next[Z]);
			}
			else if(ch->pos[X] > -door_ext && ch->pos_next[X] <= -door_ext )
			{
				//	Heading West in Doorway
				//ch->colliding = true;
				ch->pos[X] = 1-door_ext;
				ch->pos_next[X] = 1-door_ext;
				ch->vel[X] = JO_MAX(0,ch->vel[X]);
				ch->vel_next[X] = JO_MAX(0,ch->vel_next[X]);
			}
			else if(ch->pos[X] < door_ext && ch->pos_next[X] >= door_ext )
			{
				//	Heading East in Doorway
				//ch->colliding = true;
				ch->pos[X] = door_ext-1;
				ch->pos_next[X] = door_ext-1;
				ch->vel[X] = JO_MIN(0,ch->vel[X]);
				ch->vel_next[X] = JO_MIN(0,ch->vel_next[X]);
			}
		}
		else
		{
			//	No Doorway
			ch->pos[Z] = 1-wall_ext;
			ch->pos_next[Z] = 1-wall_ext;
			ch->vel[Z] = 0;
			ch->vel_next[Z] = 0;
		}
	}
	//	West
	if(ch->pos[X] <= -wall_ext || ch->pos_next[X] <= -wall_ext)
	{
		if(doors[3] == 1)
		{
			//	Doorway
			if(ch->pos[Z] <= -door_ext && ch->pos_next[Z] <= -door_ext )
			{
				//	Too Far South
				//ch->colliding = true;
				ch->pos[X] = 1-wall_ext;
				ch->pos_next[X] = 1-wall_ext;
				ch->vel[X] = JO_MAX(0,ch->vel[X]);
				ch->vel_next[X] = JO_MAX(0,ch->vel_next[X]);
			}
			else if(ch->pos[Z] >= door_ext && ch->pos_next[Z] >= door_ext )
			{
				//	Too Far North
				//ch->colliding = true;
				ch->pos[X] = 1-wall_ext;
				ch->pos_next[X] = 1-wall_ext;
				ch->vel[X] = JO_MAX(0,ch->vel[X]);
				ch->vel_next[X] = JO_MAX(0,ch->vel_next[X]);
			}
			else if(ch->pos[Z] > -door_ext && ch->pos_next[Z] <= -door_ext )
			{
				//	Heading South in Doorway
				//ch->colliding = true;
				ch->pos[Z] = 1-door_ext;
				ch->pos_next[Z] = 1-door_ext;
				ch->vel[Z] = JO_MAX(0,ch->vel[Z]);
				ch->vel_next[Z] = JO_MAX(0,ch->vel_next[Z]);
			}
			else if(ch->pos[Z] < door_ext && ch->pos_next[Z] >= door_ext )
			{
				//	Heading North in Doorway
				//ch->colliding = true;
				ch->pos[Z] = door_ext-1;
				ch->pos_next[Z] = door_ext-1;
				ch->vel[Z] = JO_MIN(0,ch->vel[Z]);
				ch->vel_next[Z] = JO_MIN(0,ch->vel_next[Z]);
			}
		}
		else
		{
			//	No Doorway
			ch->pos[X] = 1-wall_ext;
			ch->pos_next[X] = 1-wall_ext;
			ch->vel[X] = 0;
			ch->vel_next[X] = 0;
		}
	}
	
	//	Printout Status
	/*
	slPrint("  Position     Velocity   (next)    ",slLocate(0,13));
	//	Position
	slPrintFX(ch->pos_next[X],slLocate(0,14));
	slPrintFX(ch->pos_next[Y],slLocate(0,15));
	slPrintFX(ch->pos_next[Z],slLocate(0,16));
	//	Velocity
	slPrintFX(ch->vel_next[X],slLocate(13,14));
	slPrintFX(ch->vel_next[Y],slLocate(13,15));
	slPrintFX(ch->vel_next[Z],slLocate(13,16));
	
	slPrint("  Vertex 1     Vertex 2     Vertex 3    ",slLocate(0,17));
	//	Vertex 1
	slPrintFX(test_plane.v1[X],slLocate(0,18));
	slPrintFX(test_plane.v1[Y],slLocate(0,19));
	slPrintFX(test_plane.v1[Z],slLocate(0,20));
	//	Vertex 2
	slPrintFX(test_plane.v2[X],slLocate(13,18));
	slPrintFX(test_plane.v2[Y],slLocate(13,19));
	slPrintFX(test_plane.v2[Z],slLocate(13,20));
	//	Vertex 3
	slPrintFX(test_plane.v3[X],slLocate(26,18));
	slPrintFX(test_plane.v3[Y],slLocate(26,19));
	slPrintFX(test_plane.v3[Z],slLocate(26,20));
	slPrint("  Vertex 4                    Normal      ",slLocate(0,21));
	//	Vertex 4
	slPrintFX(test_plane.v4[X],slLocate(0,22));
	slPrintFX(test_plane.v4[Y],slLocate(0,23));
	slPrintFX(test_plane.v4[Z],slLocate(0,24));
	//	Normal
	slPrintFX(test_plane.n[X],slLocate(26,22));
	slPrintFX(test_plane.n[Y],slLocate(26,23));
	slPrintFX(test_plane.n[Z],slLocate(26,24));
	
	if(ch->colliding)
	{
		slPrint("Colliding    ",slLocate(0,25));
	}
	else
	{
		slPrint("Not Colliding",slLocate(0,25));
	}
	slPrint("  Position     Velocity      Acceleration",slLocate(0,26));
	//	Position
	slPrintFX(ch->pos[X],slLocate(0,27));
	slPrintFX(ch->pos[Y],slLocate(0,28));
	slPrintFX(ch->pos[Z],slLocate(0,29));
	//	Velocity
	slPrintFX(ch->vel[X],slLocate(13,27));
	slPrintFX(ch->vel[Y],slLocate(13,28));
	slPrintFX(ch->vel[Z],slLocate(13,29));
	//	Acceleration
	slPrintFX(ch->acc[X],slLocate(26,27));
	slPrintFX(ch->acc[Y],slLocate(26,28));
	slPrintFX(ch->acc[Z],slLocate(26,29));
	*/
	
	//	Reset acceleration
	ch->acc[X] =  gravity[X];
	ch->acc[Y] =  gravity[Y];
	ch->acc[Z] =  gravity[Z];
	
	//	Apply friction
	if(ch->colliding)
	{
		ch->acc[X] -= 10*ch->vel_next[X];
		ch->acc[Y] -= 10*ch->vel_next[Y];
		ch->acc[Z] -= 10*ch->vel_next[Z];
	}
	
	//	Update exhertion
	ch->HealthP += ((jo_fixed_mult(delta_time, 
		get_at(&player, 0)))>>5);
	if(ch->HealthP > get_at(ch, 9))
	{
		ch->HealthP = get_at(ch, 9);
	}
	ch->StaminaP += ((jo_fixed_mult(delta_time, 
		get_at(&player, 2)))>>3);
	if(ch->StaminaP > get_at(ch, 10))
	{
		ch->StaminaP = get_at(ch, 10);
	}
	ch->MagicP += 
		((jo_fixed_mult(delta_time, 
		get_at(&player, 4)))>>4);
	if(ch->MagicP > get_at(ch, 11))
	{
		ch->MagicP = get_at(ch, 11);
	}
	
	return;
}

void acc_character(character * ch, FIXED acc[XYZ])
{
	/*
		Accelerate Character
		
		Adds acceleration to character
	*/
	
	ch->acc[X] += acc[X];
	ch->acc[Y] += acc[Y];
	ch->acc[Z] += acc[Z];
	
	return;
}

FIXED get_at(character * ch, int at)
{
	/*
		Get Attribute
		
		Returns character's calculated attribute
		
		0	Strength
		1	Toughness
		2	Perception
		3	Dexterity
		4	Intelligence
		5	Memory
		6	Empathy
		7	Guile
		8	Luck
		9	Max Health
		10	Max Stamina
		11	Max Magic
	*/
	
	FIXED val = 0;
	
	switch(at) {
		case 0:
			//	Strength
			val = ch->H2H + ch->BlL + ch->Blu + (( ch->Shi + ch->HAr + ch->Ath )>>1);
			break;
		case 1:
			//	Toughness
			val = ch->Shi + ch->MAr + ch->HAr + (( ch->Blu + ch->RAL + ch->LAr )>>1);
			break;
		case 2:
			//	Perception
			val = ch->BlS + ch->Pol + ch->RAS + ch->RAL + (( ch->BlL )>>1);
			break;
		case 3:
			//	Dexterity
			val = ch->LAr + ch->Ath + (( ch->H2H + ch->Pol + ch->RAS + ch->MAr + ch->Ste )>>1);
			break;
		case 4:
			//	Intelligence
			val = (ch->Itr + ch->Com + ch->MaH + ch->MaC + (( ch->Eng )>>1));
			break;
		case 5:
			//	Memory
			val = ch->Eng + ch->MaE + (( ch->Com + ch->MaC + ch->MaR + ch->MaG + ch->Rel )>>1);
			break;
		case 6:
			//	Empathy
			val = ch->Spe + ch->MaR + ch->MaP + ch->Rel + (( ch->MaH )>>1);
			break;
		case 7:
			//	Guile
			val = ch->Ste + ch->MaG + (( ch->BlS + ch->Itr + ch->Spe + ch->MaE + ch->MaP )>>1);
			break;
		case 8:
			//	Luck
			val = ((get_at(ch, 0) + get_at(ch, 1) + get_at(ch, 2) + get_at(ch, 3) + get_at(ch, 4) + get_at(ch, 5) + get_at(ch, 6) + get_at(ch, 7) )>>3);
			break;
		case 9:
			//	Max Health
			val = 5*get_at(ch, 1);
			break;
		case 10:
			//	Max Stamina
			val = 4*get_at(ch, 3);
			break;
		case 11:
			//	Max Magic
			val = 3*get_at(ch, 5);
			break;
		default:
			val = 0;
	}
	
	return val;
}

