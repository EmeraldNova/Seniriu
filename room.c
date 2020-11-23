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
int current_room[XYZ];
//	Grid spacing
FIXED room_grid;
//	Room Count
int room_num = 0;
//	Scaling factor
FIXED scale_factor = toFIXED(1.0);

//	Functions
void initialize_rooms(void)
{
	/*
		Initilize Rooms
		
		Sets all rooms to nonexistant and writes coordinates
	*/
	
	//	Initialize settings
	room_grid = slMulFX(10<<16,scale_factor);
	cam_default_height = slMulFX(toFIXED(1.8),scale_factor);
	move_inc = slMulFX(8000,scale_factor);
	
	for(int i = 0; i < ROOM_NUM_X; i++)
	{
		for(int j = 0; j < ROOM_NUM_Y; j++)
		{
			for(int k = 0; k < ROOM_NUM_Z; k++)
			{
				//	Room exists
				roomMaster[i][j][k].alive = false;
				
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
			}
		}
	}
	room_num = 0;
	
	//	Reset Current Room
	current_room[X] = 0;
	current_room[Y] = 0;
	current_room[Z] = 0;
	
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
		roomMaster[i-1][j][k].doors[1] = 1;
	if(i < ROOM_NUM_X-1)
		roomMaster[i+1][j][k].doors[3] = 1;
	if(k > 0)
		roomMaster[i][j][k-1].doors[0] = 1;
	if(k < ROOM_NUM_Z-1)
		roomMaster[i][j][k+1].doors[2] = 1;
	
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
				pdataRoomMaster[i][j][k] = jo_malloc((int)pDataSizes[roomMaster[i][j][k].entity_ID]);
				slDMACopy((XPDATA*)entities[roomMaster[i][j][k].entity_ID].pol[0],
							pdataRoomMaster[i][j][k],
							pDataSizes[roomMaster[i][j][k].entity_ID]);
				roomMaster[i][j][k].pDataStart = pdataRoomMaster[i][j][k];
				
				//	Find angle
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
							//	West empty, -90 deg
							roomMaster[i][j][k].theta = -DEGtoANG(90.0);
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
	
	//	Update current room
	if(pl_position[X] < -(room_grid>>1) && current_room[X] > 0)
	{
		pl_position[X] += room_grid;
		current_room[X]--;
	}
	else if(pl_position[X] >= (room_grid>>1) && current_room[X] < ROOM_NUM_X-1)
	{
		pl_position[X] -= room_grid;
		current_room[X]++;
	}
	if(pl_position[Y] < -(room_grid>>1) && current_room[Y] < ROOM_NUM_Y-1)
	{
		//	Going up 1 floor
		pl_position[Y] += room_grid;
		current_room[Y]++;
	}
	else if(pl_position[Y] >= (room_grid>>1) && current_room[Y] > 0)
	{
		//	Going down 1 floor
		pl_position[Y] -= room_grid;
		current_room[Y]--;
	}
	if(pl_position[Z] < -(room_grid>>1) && current_room[Z] > 0)
	{
		pl_position[Z] += room_grid;
		current_room[Z]--;
	}
	else if(pl_position[Z] >= (room_grid>>1) && current_room[Z] < ROOM_NUM_Z-1)
	{
		pl_position[Z] -= room_grid;
		current_room[Z]++;
	}
	
	//	Find room limits
	int room_range = 1;
	int min_room[XYZ];
	int max_room[XYZ];
	min_room[X] = JO_MAX(current_room[X]-room_range, 0);
	max_room[X] = JO_MIN(current_room[X]+room_range, ROOM_NUM_X-1);
	min_room[Y] = JO_MAX(current_room[Y]-room_range, 0);
	max_room[Y] = JO_MIN(current_room[Y]+room_range, ROOM_NUM_Y-1);
	min_room[Z] = JO_MAX(current_room[Z]-room_range, 0);
	max_room[Z] = JO_MIN(current_room[Z]+room_range, ROOM_NUM_Z-1);
	
	//	Draw
	int print_line = 7;
	int map_delta[XYZ];
	//	Print Current Room
	jo_printf(0,print_line,"Current Room %d,%d,%d",
		current_room[X], current_room[Y], current_room[Z]);
	jo_printf(20,print_line++,"Min-Max %d-%d,%d-%d,%d-%d",
		min_room[X], max_room[X],
		min_room[Y], max_room[Y],
		min_room[Z], max_room[Z]);
	
	//	Loop through adjacent rooms
	for(int j = min_room[Y]; j <= max_room[Y]; j++)
	{
		for(int i = min_room[X]; i <= max_room[X]; i++)
		{
			for(int k = min_room[Z]; k <= max_room[Z]; k++)
			{
				if(roomMaster[i][j][k].alive)
				{
					//	Purge cache
					slCashPurge();
					
					//	Determine map coord difference
					map_delta[X] = roomMaster[i][j][k].coord[X] - current_room[X];
					map_delta[Y] = roomMaster[i][j][k].coord[Y] - current_room[Y];
					map_delta[Z] = roomMaster[i][j][k].coord[Z] - current_room[Z];
					
					slPushMatrix();		//	Advance matrix buffer pointer 1 step
			
					//	Rotate perspective for player angle 
					slRotY(-theta[Y]);	
					//	Translate draw location
					slTranslate(room_grid * map_delta[X] - pl_position[X],
						-room_grid * map_delta[Y] - pl_position[Y] + cam_height,
						room_grid * map_delta[Z] - pl_position[Z]);		
					//	Rotate object
					slRotY(-roomMaster[i][j][k].theta);	
					//	Sets scale of 3D draw, larger numbers draw bigger		
					slScale(scale_factor, scale_factor, scale_factor);	
				
					//	Draw
					slPutPolygon((XPDATA*)roomMaster[i][j][k].pDataStart);
					
					slPopMatrix();		//	Return matrix buffer pointer back 1 step
					
					//	Print Rooms Status
					jo_printf(0,print_line++,"Room %d,%d,%d: %d",
						i, j, k, roomMaster[i][j][k].entity_ID);
				}
			}
		}
	}
	
	return;
}

void load_rooms()
{
	/*
		Load Rooms
		
		Loads in entity data from ZTP
	*/
	
	//	Start loading in LWRAM beginning
    void * currentAddress = (void*)LWRAM;
	
	//	Load rooms
	int ID = 0;
	currentAddress = ztLoad3Dmodel((Sint8*)"COR1W.ZTP", currentAddress, ID++, false);
	currentAddress = ztLoad3Dmodel((Sint8*)"COR2SW.ZTP", currentAddress, ID++, false);
	currentAddress = ztLoad3Dmodel((Sint8*)"COR2AW.ZTP", currentAddress, ID++, false);
	currentAddress = ztLoad3Dmodel((Sint8*)"COR3.ZTP", currentAddress, ID++, false);
	currentAddress = ztLoad3Dmodel((Sint8*)"COR4.ZTP", currentAddress, ID++, false);
	
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
	position[X] = 0;
	position[Y] = 0;
	position[Z] = 1;
	create_room(position);
	
	// Room
	position[X] = 0;
	position[Y] = 1;
	position[Z] = 0;
	create_room(position);
	
	// Room
	position[X] = 1;
	position[Y] = 1;
	position[Z] = 0;
	create_room(position);
	
	// Room
	position[X] = 0;
	position[Y] = 1;
	position[Z] = 1;
	create_room(position);
	
	// Room
	position[X] = 0;
	position[Y] = 2;
	position[Z] = 0;
	create_room(position);
	
	// Room
	position[X] = 1;
	position[Y] = 2;
	position[Z] = 0;
	create_room(position);
	
	// Room
	position[X] = 0;
	position[Y] = 2;
	position[Z] = 1;
	create_room(position);
	
	//	Set current room
	current_room[X] = 1;
	current_room[Y] = 0;
	current_room[Z] = 0;
	
	return;
}
