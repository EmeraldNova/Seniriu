/*
** Seniriu
** Matthew Suttinger & Emerald Nova (emeraldnovagames@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

#include "room.h"

//	Variables
//	Pointer to copy of XPDATA 
XPDATA *pdataRoomMaster[ROOM_NUM_X][ROOM_NUM_Y][ROOM_NUM_Z];
//	Room container 
room roomMaster[ROOM_NUM_X][ROOM_NUM_Y][ROOM_NUM_Z];
//	Current room contianing camera
//int player.room[XYZ];
//	Grid spacing
FIXED room_grid[XYZ];
//	Total Room Count
int room_num = 0;


//	Functions
void initialize_rooms(void)
{
	/*
		Initilize Rooms
		
		Sets all rooms to nonexistant and writes coordinates
	*/
	
	//	Initialize settings
	room_grid[X] = 10<<16;
	room_grid[Y] = 6<<16;
	room_grid[Z] = 10<<16;
	//cam_default_height = toFIXED(1.8);
	//move_inc = slMulFX(8000,scale_factor);
	
	for(int i = 0; i < ROOM_NUM_X; i++)
	{
		for(int j = 0; j < ROOM_NUM_Y; j++)
		{
			for(int k = 0; k < ROOM_NUM_Z; k++)
			{
				//	Room doesn't exist yet
				roomMaster[i][j][k].alive = false;
				roomMaster[i][j][k].vis = false;
				
				//	Write coords
				roomMaster[i][j][k].coord[X] = i;
				roomMaster[i][j][k].coord[Y] = j;
				roomMaster[i][j][k].coord[Z] = k;
				roomMaster[i][j][k].theta = 0;
				
				//	Set adjacent doors to 0
				roomMaster[i][j][k].doors[0] = 0;
				roomMaster[i][j][k].doors[1] = 0;
				roomMaster[i][j][k].doors[2] = 0;
				roomMaster[i][j][k].doors[3] = 0;
				roomMaster[i][j][k].doors[4] = 0;
				roomMaster[i][j][k].doors[5] = 0;
				
				//  Set opacity
				roomMaster[i][j][k].opacity[0] = 0;
				roomMaster[i][j][k].opacity[1] = 0;
				roomMaster[i][j][k].opacity[2] = 0;
				roomMaster[i][j][k].opacity[3] = 0;
				roomMaster[i][j][k].opacity[4] = 0;
				roomMaster[i][j][k].opacity[5] = 0;
			}
		}
	}
	room_num = 0;
	
	//	Reset Current Room
	player.room[X] = 0;
	player.room[Y] = 0;
	player.room[Z] = 0;
	
	return;
}

void create_room(int position[XYZ])
{
	/*
		Create Room
		
		Set's room to true.
	*/
	
	//	Set to exist
	int i = position[X];
	int j = position[Y];
	int k = position[Z];
	roomMaster[i][j][k].alive = true;
	
	//	Adjust neighboring doors, existing or not
	if(i > 0)
	{
		//	Adjust room to west
		roomMaster[i-1][j][k].doors[1] = 1;
		roomMaster[i-1][j][k].opacity[1] = 1;
	}
	if(i < ROOM_NUM_X-1)
	{
		roomMaster[i+1][j][k].doors[3] = 1;
		roomMaster[i+1][j][k].opacity[3] = 1;
	}
	if(k > 0)
	{
		roomMaster[i][j][k-1].doors[0] = 1;
		roomMaster[i][j][k-1].opacity[0] = 1;
	}
	if(k < ROOM_NUM_Z-1)
	{
		roomMaster[i][j][k+1].doors[2] = 1;
		roomMaster[i][j][k+1].opacity[2] = 1;
	}
	
	//	Initialize window if not already set
	for(int l = 0; l < 4; l++)
	{
		if(roomMaster[i][j][k].doors[l] == 0)
		{
			//	Write door/window at location
			roomMaster[i][j][k].opacity[l] = 1;
		}
		//	Opaque ceiling/floor
		roomMaster[i][j][k].doors[4] = 2;
		roomMaster[i][j][k].doors[5] = 2;
	}
	
	//	Increment room number
	room_num++;
	
	return;
}

void map_rooms(void)
{
	/*
		Map Rooms
		
		Connects rooms together and loads in corrrect model for
		entity and XPdata
	*/
	
	//	Loop through rooms to determine models to use
	int door_count = 0;
	for(int i = 0; i < ROOM_NUM_X; i++)
	{
		for(int j = 0; j < ROOM_NUM_Y; j++)
		{
			for(int k = 0; k < ROOM_NUM_Z; k++)
			{
				//	Turn empty doors to windows
				for(int l = 0; l < 4; l++)
				{
					if(roomMaster[i][j][k].doors[l] != 1)
					{
						roomMaster[i][j][k].doors[l] = 2;
					}
				}
				
				//	Count doors
				door_count = roomMaster[i][j][k].doors[0] +
							 roomMaster[i][j][k].doors[1] +
							 roomMaster[i][j][k].doors[2] +
							 roomMaster[i][j][k].doors[3];
				
				//	Find model
				roomMaster[i][j][k].entity_ID = 0;
				if(door_count == 1)
				{
					//	Single door
					roomMaster[i][j][k].entity_ID = 0;
				}
				else if(door_count == 2)
				{
					//	2 Door Straight
					if((roomMaster[i][j][k].doors[0] *
					   roomMaster[i][j][k].doors[2] == 1) ||
					   (roomMaster[i][j][k].doors[1] *
					   roomMaster[i][j][k].doors[3] == 1))
				    {
						roomMaster[i][j][k].entity_ID = 1;
					}
					else
					{
						//	2 Door Right Angle
						roomMaster[i][j][k].entity_ID = 2;
					}
				}
				else if(door_count == 3)
				{
					//	3 Door
					roomMaster[i][j][k].entity_ID = 3;
				}
				else if(door_count == 4)
				{
					//	4 Door
					roomMaster[i][j][k].entity_ID = 4;
				}
				
				//	Copy pData
				/*
				pdataRoomMaster[i][j][k] = jo_malloc((int)pDataSizes[roomMaster[i][j][k].entity_ID]);
				
				slDMACopy((XPDATA*)entities[roomMaster[i][j][k].entity_ID].pol[0],
							pdataRoomMaster[i][j][k],
							pDataSizes[roomMaster[i][j][k].entity_ID]);
				*/
				/*
				pdataRoomMaster[i][j][k] = (XPDATA*)entities[roomMaster[i][j][k].entity_ID].pol[0];
							
				roomMaster[i][j][k].pDataStart = pdataRoomMaster[i][j][k];
				*/
				//	Find angle, define opacity
				roomMaster[i][j][k].theta = 0;
				switch(roomMaster[i][j][k].entity_ID)
				{
					case 0:
						//	1 door, rotate to fit
						if(roomMaster[i][j][k].doors[2] == 1)
						{
							//	South, no rotation
							roomMaster[i][j][k].theta = 0;
						}
						else if(roomMaster[i][j][k].doors[0] == 1)
						{
							//	North, 180 deg
							roomMaster[i][j][k].theta = DEGtoANG(180.0);
						}
						else if(roomMaster[i][j][k].doors[1] == 1)
						{
							//	East, +90 deg
							roomMaster[i][j][k].theta = DEGtoANG(90.0);
						}
						else if(roomMaster[i][j][k].doors[3] == 1)
						{
							//	West, -90 deg
							roomMaster[i][j][k].theta = -DEGtoANG(90.0);
						}
						break;
						
					case 1:
						//	2 door straight
						if((roomMaster[i][j][k].doors[0] == 1) ||
						   (roomMaster[i][j][k].doors[2] == 1))
						{
							//	North, no rotation
							roomMaster[i][j][k].theta = 0;
						}
						else if((roomMaster[i][j][k].doors[1] == 1) ||
						   (roomMaster[i][j][k].doors[3] == 1))
						{
							//	East/West, 90 deg
							roomMaster[i][j][k].theta = DEGtoANG(90.0);
						}
						break;
						
					case 2:
						//	2 door right turn (default S-E)
						//	Find first door
						if(roomMaster[i][j][k].doors[0] == 1)
						{
							//	North, either west or east facing door
							if(roomMaster[i][j][k].doors[1] == 1)
							{
								//	East, +90 deg
								roomMaster[i][j][k].theta = DEGtoANG(90.0);
							}
							else if(roomMaster[i][j][k].doors[3] == 1)
							{
								//	West, 180 deg
								roomMaster[i][j][k].theta = DEGtoANG(180.0);
							}
						}
						else if(roomMaster[i][j][k].doors[1] == 1)
						{
							//	East/South, No rotation
							roomMaster[i][j][k].theta = 0;
						}
						else if(roomMaster[i][j][k].doors[2] == 1)
						{
							//	South/West, -90 deg
							roomMaster[i][j][k].theta = -DEGtoANG(90.0);
						}
						break;
						
					case 3:
						//	3 door, rotate to fit
						if(roomMaster[i][j][k].doors[0] == 0)
						{
							//	North empty, no rotation
							roomMaster[i][j][k].theta = 0;
						}
						else if(roomMaster[i][j][k].doors[1] == 0)
						{
							//	East empty, -90 deg
							roomMaster[i][j][k].theta = -DEGtoANG(90.0);
						}
						else if(roomMaster[i][j][k].doors[2] == 0)
						{
							//	South empty, 180 deg
							roomMaster[i][j][k].theta = DEGtoANG(180.0);
						}
						else if(roomMaster[i][j][k].doors[3] == 0)
						{
							//	West empty, 90 deg
							roomMaster[i][j][k].theta = DEGtoANG(90.0);
						}
						break;
						
					case 4:
						//	4 door, no rotation
						roomMaster[i][j][k].theta = 0;
						break;
				}
			}
		}
	}
		
	return;
}

void draw_rooms(void)
{
	/*
		Draw Rooms
		
		Draws all existing rooms
	*/
	
	//	Print counter
	int print_line = 7;
	
	//	Print Current Room
	jo_set_printf_color_index(JO_COLOR_INDEX_Red);
	//jo_printf(0,print_line++,"Current Room %d,%d,%d",
	//	player.room[X], player.room[Y], player.room[Z]);
	
	//	Find visible rooms using integer angles
	//	ANGLE / 182.04 (4 repeating) = degrees as ints
	int room_hor;	//	Horizontal angle of room from current room
	int room_ver;	//	Vertical angle of room from current room
	int lower_lim;	//	Lower angle limit check
	int upper_lim;	//	Upper angle limit check
	int num_vis = 0;	//	Number of visible rooms
	for(int j = 0; j < ROOM_NUM_Y; j++)
	{
		for(int i = 0; i < ROOM_NUM_X; i++)
		{
			for(int k = 0; k < ROOM_NUM_Z; k++)
			{
				//	Set to not visible
				roomMaster[i][j][k].vis = false;
				
				//	Only calculate real rooms
				if(roomMaster[i][j][k].alive)
				{
					//	Build deltas
					roomMaster[i][j][k].dist[X] = i - player.room[X];
					roomMaster[i][j][k].dist[Y] = j - player.room[Y];
					roomMaster[i][j][k].dist[Z] = k -player.room[Z];
					
					//	Always draw current room
					if(JO_ABS(roomMaster[i][j][k].dist[X]) +
					   JO_ABS(roomMaster[i][j][k].dist[Y]) +
					   JO_ABS(roomMaster[i][j][k].dist[Z]) == 0)
					{
						roomMaster[i][j][k].vis = true;
					}
					else if(
						JO_ABS(roomMaster[i][j][k].dist[Y]) == 0
						)
					{
						if(
						slAng2FX(player.theta[Y]) <= 90<<16 ||
						slAng2FX(player.theta[Y]) >= 270<<16
						)
						{
							//	Draw +Z direction
							if(roomMaster[i][j][k].dist[Z] > 0)
							{
								roomMaster[i][j][k].vis = true;
							}
						}
						
						if(
						slAng2FX(player.theta[Y]) <= 180<<16 && 
						slAng2FX(player.theta[Y]) >= 0<<16
						)
						{
							//	Draw +X direction
							if(roomMaster[i][j][k].dist[X] > 0)
							{
								roomMaster[i][j][k].vis = true;
							}
						}
						
						if(
						slAng2FX(player.theta[Y]) <= 270<<16 && 
						slAng2FX(player.theta[Y]) >= 90<<16
						)
						{
							//	Draw -Z direction
							if(roomMaster[i][j][k].dist[Z] < 0)
							{
								roomMaster[i][j][k].vis = true;
							}
						}
						
						if(
						slAng2FX(player.theta[Y]) >= 180<<16
						)
						{
							//	Draw -X direction
							if(roomMaster[i][j][k].dist[X] < 0)
							{
								roomMaster[i][j][k].vis = true;
							}
						}
					}
					
					//	Iterate number visible
					if(roomMaster[i][j][k].vis)
					{
						num_vis++;
						
						//	Print current room info
						/*
						jo_printf(0,print_line++,"Room %d,%d,%d; delta: %d,%d,%d; vis: %d          ",
						i, j, k,
						roomMaster[i][j][k].dist[X],
						roomMaster[i][j][k].dist[Y],
						roomMaster[i][j][k].dist[Z],
						roomMaster[i][j][k].vis);
						*/
						
						//	Draw Exterior Hulls if within 3 units
						if(JO_ABS(roomMaster[i][j][k].dist[X]) +
							JO_ABS(roomMaster[i][j][k].dist[Y]) +
							JO_ABS(roomMaster[i][j][k].dist[Z]) <= 3)
						{
							//	Determine surroundings
							bool surrounding[3][3] = {
								{false, false, false},
								{false, true, false},
								{false, false, false}
							};
							if( (i > 0) && (k > 0) )
							{
								//	SouthWest
								surrounding[0][0] = roomMaster[i-1][j][k-1].alive;
							}
							if( (i > 0) )
							{
								//	West
								surrounding[0][1] = roomMaster[i-1][j][k].alive;
							}
							if( (i > 0) && (k < ROOM_NUM_Z) )
							{
								//	NorthWest
								surrounding[0][2] = roomMaster[i-1][j][k+1].alive;
							}
							if( (k > 0) )
							{
								//	South
								surrounding[1][0] = roomMaster[i][j][k-1].alive;
							}
							if( (k < ROOM_NUM_Z) )
							{
								//	North
								surrounding[1][2] = roomMaster[i][j][k+1].alive;
							}
							if( (i < ROOM_NUM_X) && (k > 0) )
							{
								//	SouthEast
								surrounding[2][0] = roomMaster[i+1][j][k-1].alive;
							}
							if( (i < ROOM_NUM_X) )
							{
								//	East
								surrounding[2][1] = roomMaster[i+1][j][k].alive;
							}
							if( (i < ROOM_NUM_X) && (k < ROOM_NUM_Z) )
							{
								//	NorthEast
								surrounding[2][2] = roomMaster[i+1][j][k+1].alive;
							}
							
							
							//	Draw each manually	
							
							//	Purge cache
							slCashPurge();
							//	Reset matrix
							slUnitMatrix(0);
							//	Advance matrix buffer pointer 1 step
							slPushMatrix();
							//	Set Scale
							slScale((1<<16), (1<<16), (1<<16));
							//	Rotate perspective for player angle 
							slRotY(-player.theta[Y]);	
							//	Translate draw location
							slTranslate(
								MODEL_SCALE*(room_grid[X] * roomMaster[i][j][k].dist[X] - player.pos[X]),
								MODEL_SCALE*(-room_grid[Y] * roomMaster[i][j][k].dist[Y] - player.pos[Y] + cam_height),
								MODEL_SCALE*(room_grid[Z] * roomMaster[i][j][k].dist[Z] - player.pos[Z]));					
							bool dummy;
							
							//	SouthWest
							if( (!surrounding[1][0]) && surrounding[0][1] )
							{
								//	Straight Hull
								slPutPolygon((XPDATA*)entities[
									exter_start + 0].pol[0]);
								dummy = false;
							}
							if( (!surrounding[1][0]) && surrounding[2][1] )
							{
								//	Straight Hull
								slScale(-(1<<16), (1<<16), (1<<16));
								slPutPolygon((XPDATA*)entities[
									exter_start + 0].pol[0]);
								slScale(-(1<<16), (1<<16), (1<<16));
								dummy = false;
							}
							if( (!surrounding[1][0]) && (!surrounding[0][1]) )
							{
								//	Exterior Hull
								slPutPolygon((XPDATA*)entities[
									exter_start + 1].pol[0]);
								dummy = false;
							}
							if( surrounding[1][0] && surrounding[0][1] && (!surrounding[0][0]) )
							{
								//	Interior Hull
								slPutPolygon((XPDATA*)entities[
									exter_start + 2].pol[0]);
								dummy = false;
							}
							//	Iterate Rotation
							slRotY(DEGtoANG(90.0));
							
							
							//	NorthWest
							if( !surrounding[0][1] && surrounding[1][2] )
							{
								//	Straight Hull
								slPutPolygon((XPDATA*)entities[
									exter_start + 0].pol[0]);
								dummy = false;
							}
							if( !surrounding[0][1] && surrounding[1][0] )
							{
								//	Straight Hull
								slScale(-(1<<16), (1<<16), (1<<16));
								slPutPolygon((XPDATA*)entities[
									exter_start + 0].pol[0]);
								slScale(-(1<<16), (1<<16), (1<<16));
								dummy = false;
							}
							if( !surrounding[1][2] && !surrounding[0][1] )
							{
								//	Exterior Hull
								slPutPolygon((XPDATA*)entities[
									exter_start + 1].pol[0]);
								dummy = false;
							}
							if( surrounding[1][2] && surrounding[0][1] && !surrounding[0][2] )
							{
								//	Interior Hull
								slPutPolygon((XPDATA*)entities[
									exter_start + 2].pol[0]);
								dummy = false;
							}
							
							//	Iterate Rotation
							slRotY(DEGtoANG(90.0));
							
							//	NorthEast
							if( !surrounding[1][2] && surrounding[2][1] )
							{
								//	Straight Hull
								slPutPolygon((XPDATA*)entities[
									exter_start + 0].pol[0]);
								dummy = false;
							}
							if( !surrounding[1][2] && surrounding[0][1] )
							{
								//	Straight Hull
								slScale(-(1<<16), (1<<16), (1<<16));
								slPutPolygon((XPDATA*)entities[
									exter_start + 0].pol[0]);
								slScale(-(1<<16), (1<<16), (1<<16));
								dummy = false;
							}
							
							if( !surrounding[2][1] && !surrounding[1][2] )
							{
								//	Exterior Hull
								slPutPolygon((XPDATA*)entities[
									exter_start + 1].pol[0]);
								dummy = false;
							}
							if( surrounding[1][2] && surrounding[2][1] && !surrounding[2][2] )
							{
								//	Interior Hull
								slPutPolygon((XPDATA*)entities[
									exter_start + 2].pol[0]);
								dummy = false;
							}
							
							//	Iterate Rotation
							slRotY(DEGtoANG(90.0));
							
							//	SouthEast
							if( !surrounding[2][1] && surrounding[1][0] )
							{
								//	Straight Hull
								slPutPolygon((XPDATA*)entities[
									exter_start + 0].pol[0]);
								dummy = false;
							}
							if( !surrounding[2][1] && surrounding[1][2] )
							{
								//	Straight Hull
								slScale(-(1<<16), (1<<16), (1<<16));
								slPutPolygon((XPDATA*)entities[
									exter_start + 0].pol[0]);
								slScale(-(1<<16), (1<<16), (1<<16));
								dummy = false;
							}
							if( !surrounding[1][0] && !surrounding[2][1] )
							{
								//	Exterior Hull
								slPutPolygon((XPDATA*)entities[
									exter_start + 1].pol[0]);
								dummy = false;
							}
							if( surrounding[1][0] && surrounding[2][1] && !surrounding[2][2] )
							{
								//	Interior Hull
								slPutPolygon((XPDATA*)entities[
									exter_start + 2].pol[0]);
								dummy = false;
							}
							
							//	Iterate Rotation
							slRotY(DEGtoANG(90.0));	
							
							//	Return matrix buffer pointer back 1 step
							slPopMatrix();	
						}							
					}
				}
			}
		}
	}
	num_vis = JO_MIN(num_vis, MAX_ROOM_DISPLAY);
	//	Print visible room info
	/*
	jo_printf(0,print_line++,"Visible rooms: %d, Max Rooms: %d       ", num_vis, MAX_ROOM_DISPLAY);
	jo_printf(0,print_line++,"                                        ");
	jo_printf(0,print_line++,"                                        ");
	jo_printf(0,print_line++,"                                        ");
	jo_printf(0,print_line++,"                                        ");
	jo_printf(0,print_line++,"                                        ");
	jo_printf(0,print_line++,"                                        ");
	jo_printf(0,print_line++,"                                        ");
	*/
	
	
	//	Generate list of rooms to draw
	int room_draw_coords[MAX_ROOM_DISPLAY][3];
	//	Current room
	int room_draw_count = 0;
	if(roomMaster[player.room[X]][player.room[Y]][player.room[Z]].vis)
	{
		room_draw_coords[0][0] = player.room[X];
		room_draw_coords[0][1] = player.room[Y];
		room_draw_coords[0][2] = player.room[Z];
		room_draw_count++;
	}
	//	Loop through rooms
	int dist_compare = 1;
	int min_room[XYZ];
	int max_room[XYZ];
	while(room_draw_count < num_vis)
	{
		//	Find room limits
		min_room[X] = JO_MAX(player.room[X]-dist_compare, 0);
		max_room[X] = JO_MIN(player.room[X]+dist_compare, ROOM_NUM_X-1);
		min_room[Y] = JO_MAX(player.room[Y]-dist_compare, 0);
		max_room[Y] = JO_MIN(player.room[Y]+dist_compare, ROOM_NUM_Y-1);
		min_room[Z] = JO_MAX(player.room[Z]-dist_compare, 0);
		max_room[Z] = JO_MIN(player.room[Z]+dist_compare, ROOM_NUM_Z-1);
		
		//	Full search
		min_room[X] = 0;
		min_room[Y] = 0;
		min_room[Z] = 0;
		max_room[X] = ROOM_NUM_X-1;
		max_room[Y] = ROOM_NUM_Y-1;
		max_room[Z] = ROOM_NUM_Z-1;
		
		for(int j = min_room[Y]; j <= max_room[Y]; j++)
		{
			for(int i = min_room[X]; i <= max_room[X]; i++)
			{
				for(int k = min_room[Z]; k <= max_room[Z]; k++)
				{
					//	Only add visible rooms to draw list and only if number of drawns rooms is within limit
					if(roomMaster[i][j][k].vis && (room_draw_count < num_vis))
					{
						//	Check distance
						if(
							dist_compare == 
							JO_ABS(roomMaster[i][j][k].dist[X]) +
							JO_ABS(roomMaster[i][j][k].dist[Y]) +
							JO_ABS(roomMaster[i][j][k].dist[Z])
						)
						{
							//	Distance matches, add room to render list, then iterate draw count
							room_draw_coords[room_draw_count][0] = i;
							room_draw_coords[room_draw_count][1] = j;
							room_draw_coords[room_draw_count][2] = k;
							room_draw_count++;
						}
					}
				}
			}
		}
		
		//	Iterate comapred distance
		dist_compare++;
	}
	
	//	Loop through rooms to draw
	int i,j,k;
	for(room_draw_count = 0; room_draw_count < num_vis; room_draw_count++)
	{
		//	Purge cache
		slCashPurge();
		
		//	Set coords
		i = room_draw_coords[room_draw_count][0];
		j = room_draw_coords[room_draw_count][1];
		k = room_draw_coords[room_draw_count][2];
		
		slPushMatrix();		//	Advance matrix buffer pointer 1 step

		//	Rotate perspective for player angle 
		slRotY(-player.theta[Y]);	
		//	Translate draw location
		slTranslate(
			MODEL_SCALE*(room_grid[X] * roomMaster[i][j][k].dist[X] - player.pos[X]),
			MODEL_SCALE*(-room_grid[Y] * roomMaster[i][j][k].dist[Y] - player.pos[Y] + cam_height),
			MODEL_SCALE*(room_grid[Z] * roomMaster[i][j][k].dist[Z] - player.pos[Z]));		
		
		//	Rotate object
		//slRotY(-roomMaster[i][j][k].theta);	
		
		//	Sets scale of 3D draw, larger numbers draw bigger		
		//slScale(scale_factor, scale_factor, scale_factor);	
		
		//	Draw Floor
		slPutPolygon((XPDATA*)entities[inter_start].pol[0]);
		//	Draw Ceiling
		slPutPolygon((XPDATA*)entities[inter_start + 1].pol[0]);
		
		//	Draw Corners
		for(int l = 0; l < 4; l++)
		{
			slPutPolygon((XPDATA*)entities[inter_start + 2].pol[0]);
			slRotY(-DEGtoANG(90.0));	
		}
		
		//	Draw Doors/Windows
		slRotY(-DEGtoANG(180.0));
		for(int l = 0; l < 4; l++)
		{
			if(roomMaster[i][j][k].doors[l] > 0)
			{
				//	If door or window, draw
				slPutPolygon((XPDATA*)entities[
					inter_start + 2 + roomMaster[i][j][k].doors[l]
					].pol[0]);
			}
			slRotY(DEGtoANG(90.0));	
		}
		
		slPopMatrix();		//	Return matrix buffer pointer back 1 step
	}
	
	//	Draw Skybox
	/*
	//	Purge cache
	slCashPurge();
	
	//	Set coords
	i = room_draw_coords[room_draw_count][0];
	j = room_draw_coords[room_draw_count][1];
	k = room_draw_coords[room_draw_count][2];
	
	slPushMatrix();		//	Advance matrix buffer pointer 1 step

	//	Rotate perspective for player angle 
	slRotY(-player.theta[Y]);	
	
	//	Rotate object
	slRotY(-roomMaster[i][j][k].theta);	
	//	Sets scale of 3D draw, larger numbers draw bigger		
	slScale(scale_factor>>5, scale_factor>>5, scale_factor>>5);	

	//	Draw Skybox
	//slPutPolygon((XPDATA*)entities[10].pol[0]);
	
	slPopMatrix();		//	Return matrix buffer pointer back 1 step
	*/
	
	return;
}


void generate_rooms(void)
{
	/*
		Generate Rooms
		
		Generates rooms in map
	*/
	
	//	Prepare position
	int position[XYZ];
	
	// Room
	position[X] = 0;
	position[Y] = 0;
	position[Z] = 0;
	create_room(position);
	
	// Room
	position[X] = 1;
	position[Y] = 0;
	position[Z] = 0;
	create_room(position);
	
	// Room
	position[X] = 2;
	position[Y] = 0;
	position[Z] = 0;
	create_room(position);
	
	// Room
	position[X] = 2;
	position[Y] = 0;
	position[Z] = 1;
	create_room(position);
	
	// Room
	position[X] = 7;
	position[Y] = 0;
	position[Z] = 1;
	create_room(position);
	
	// Room
	position[X] = 0;
	position[Y] = 0;
	position[Z] = 2;
	create_room(position);
	
	// Room
	position[X] = 1;
	position[Y] = 0;
	position[Z] = 2;
	create_room(position);
	
	// Room
	position[X] = 2;
	position[Y] = 0;
	position[Z] = 2;
	create_room(position);
	
	// Room
	position[X] = 3;
	position[Y] = 0;
	position[Z] = 2;
	create_room(position);
	
	// Room
	position[X] = 4;
	position[Y] = 0;
	position[Z] = 2;
	create_room(position);
	
	// Room
	position[X] = 5;
	position[Y] = 0;
	position[Z] = 2;
	create_room(position);
	
	// Room
	position[X] = 7;
	position[Y] = 0;
	position[Z] = 2;
	create_room(position);
	
	// Room
	position[X] = 0;
	position[Y] = 0;
	position[Z] = 3;
	create_room(position);
	
	// Room
	position[X] = 5;
	position[Y] = 0;
	position[Z] = 3;
	create_room(position);
	
	// Room
	position[X] = 6;
	position[Y] = 0;
	position[Z] = 3;
	create_room(position);
	
	// Room
	position[X] = 7;
	position[Y] = 0;
	position[Z] = 3;
	create_room(position);
	
	// Room
	position[X] = 0;
	position[Y] = 0;
	position[Z] = 4;
	create_room(position);
	
	// Room
	position[X] = 2;
	position[Y] = 0;
	position[Z] = 4;
	create_room(position);
	
	// Room
	position[X] = 5;
	position[Y] = 0;
	position[Z] = 4;
	create_room(position);
	
	// Room
	position[X] = 0;
	position[Y] = 0;
	position[Z] = 5;
	create_room(position);
	
	// Room
	position[X] = 1;
	position[Y] = 0;
	position[Z] = 5;
	create_room(position);
	
	// Room
	position[X] = 2;
	position[Y] = 0;
	position[Z] = 5;
	create_room(position);
	
	// Room
	position[X] = 5;
	position[Y] = 0;
	position[Z] = 5;
	create_room(position);
	
	// Room
	position[X] = 6;
	position[Y] = 0;
	position[Z] = 5;
	create_room(position);
	
	// Room
	position[X] = 7;
	position[Y] = 0;
	position[Z] = 5;
	create_room(position);
	
	// Room
	position[X] = 8;
	position[Y] = 0;
	position[Z] = 5;
	create_room(position);
	
	// Room
	position[X] = 9;
	position[Y] = 0;
	position[Z] = 5;
	create_room(position);
	
	// Room
	position[X] = 0;
	position[Y] = 0;
	position[Z] = 6;
	create_room(position);
	
	// Room
	position[X] = 5;
	position[Y] = 0;
	position[Z] = 6;
	create_room(position);
	
	// Room
	position[X] = 9;
	position[Y] = 0;
	position[Z] = 6;
	create_room(position);
	
	// Room
	position[X] = 0;
	position[Y] = 0;
	position[Z] = 7;
	create_room(position);
	
	// Room
	position[X] = 2;
	position[Y] = 0;
	position[Z] = 7;
	create_room(position);
	
	// Room
	position[X] = 3;
	position[Y] = 0;
	position[Z] = 7;
	create_room(position);
	
	// Room
	position[X] = 5;
	position[Y] = 0;
	position[Z] = 7;
	create_room(position);
	
	// Room
	position[X] = 6;
	position[Y] = 0;
	position[Z] = 7;
	create_room(position);
	
	// Room
	position[X] = 7;
	position[Y] = 0;
	position[Z] = 7;
	create_room(position);
	
	// Room
	position[X] = 9;
	position[Y] = 0;
	position[Z] = 7;
	create_room(position);
	
	// Room
	position[X] = 0;
	position[Y] = 0;
	position[Z] = 8;
	create_room(position);
	
	// Room
	position[X] = 3;
	position[Y] = 0;
	position[Z] = 8;
	create_room(position);
	
	// Room
	position[X] = 5;
	position[Y] = 0;
	position[Z] = 8;
	create_room(position);
	
	// Room
	position[X] = 7;
	position[Y] = 0;
	position[Z] = 8;
	create_room(position);
	
	// Room
	position[X] = 0;
	position[Y] = 0;
	position[Z] = 9;
	create_room(position);
	
	// Room
	position[X] = 1;
	position[Y] = 0;
	position[Z] = 9;
	create_room(position);
	
	// Room
	position[X] = 2;
	position[Y] = 0;
	position[Z] = 9;
	create_room(position);
	
	// Room
	position[X] = 3;
	position[Y] = 0;
	position[Z] = 9;
	create_room(position);
	
	// Room
	position[X] = 4;
	position[Y] = 0;
	position[Z] = 9;
	create_room(position);
	
	// Room
	position[X] = 5;
	position[Y] = 0;
	position[Z] = 9;
	create_room(position);
	
	
	
	//	Set current room
	player.room[X] = 0;
	player.room[Y] = 0;
	player.room[Z] = 0;
	
	return;
}
