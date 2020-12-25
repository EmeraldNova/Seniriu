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
FIXED room_grid[XYZ];
//	Total Room Count
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
	room_grid[X] = slMulFX(10<<16,scale_factor);
	room_grid[Y] = slMulFX( 6<<16,scale_factor);
	room_grid[Z] = slMulFX(10<<16,scale_factor);
	cam_default_height = slMulFX(toFIXED(1.8),scale_factor);
	move_inc = slMulFX(8000,scale_factor);
	
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
				pdataRoomMaster[i][j][k] = (XPDATA*)entities[roomMaster[i][j][k].entity_ID].pol[0];
							
				roomMaster[i][j][k].pDataStart = pdataRoomMaster[i][j][k];
				
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
	
	//	Update current room
	if(pl_position[X] < -(room_grid[X]>>1) && current_room[X] > 0)
	{
		pl_position[X] += room_grid[X];
		current_room[X]--;
	}
	else if(pl_position[X] >= (room_grid[X]>>1) && current_room[X] < ROOM_NUM_X-1)
	{
		pl_position[X] -= room_grid[X];
		current_room[X]++;
	}
	if(pl_position[Y] < -(room_grid[Y]>>1) && current_room[Y] < ROOM_NUM_Y-1)
	{
		//	Going up 1 floor
		pl_position[Y] += room_grid[Y];
		current_room[Y]++;
	}
	else if(pl_position[Y] >= (room_grid[Y]>>1) && current_room[Y] > 0)
	{
		//	Going down 1 floor
		pl_position[Y] -= room_grid[Y];
		current_room[Y]--;
	}
	if(pl_position[Z] < -(room_grid[Z]>>1) && current_room[Z] > 0)
	{
		pl_position[Z] += room_grid[Z];
		current_room[Z]--;
	}
	else if(pl_position[Z] >= (room_grid[Z]>>1) && current_room[Z] < ROOM_NUM_Z-1)
	{
		pl_position[Z] -= room_grid[Z];
		current_room[Z]++;
	}
	//	Print Current Room
	jo_set_printf_color_index(JO_COLOR_INDEX_Red);
	jo_printf(0,print_line++,"Current Room %d,%d,%d",
		current_room[X], current_room[Y], current_room[Z]);
	
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
					roomMaster[i][j][k].dist[X] = i - current_room[X];
					roomMaster[i][j][k].dist[Y] = j - current_room[Y];
					roomMaster[i][j][k].dist[Z] = k -current_room[Z];
					
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
						slAng2FX(theta[Y]) <= 90<<16 ||
						slAng2FX(theta[Y]) >= 270<<16
						)
						{
							//	Draw +Z direction
							if(roomMaster[i][j][k].dist[Z] > 0)
							{
								roomMaster[i][j][k].vis = true;
							}
						}
						
						if(
						slAng2FX(theta[Y]) <= 180<<16 && 
						slAng2FX(theta[Y]) >= 0<<16
						)
						{
							//	Draw +X direction
							if(roomMaster[i][j][k].dist[X] > 0)
							{
								roomMaster[i][j][k].vis = true;
							}
						}
						
						if(
						slAng2FX(theta[Y]) <= 270<<16 && 
						slAng2FX(theta[Y]) >= 90<<16
						)
						{
							//	Draw -Z direction
							if(roomMaster[i][j][k].dist[Z] < 0)
							{
								roomMaster[i][j][k].vis = true;
							}
						}
						
						if(
						slAng2FX(theta[Y]) >= 180<<16
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
					}
				}
			}
		}
	}
	//	Print visible room info
	jo_printf(0,print_line++,"Visible rooms: %d, Max Rooms: %d       ", num_vis, MAX_ROOM_DISPLAY);
	num_vis = JO_MIN(num_vis, MAX_ROOM_DISPLAY);
	jo_printf(0,print_line++,"                                    ");
	jo_printf(0,print_line++,"                                    ");
	jo_printf(0,print_line++,"                                    ");
	jo_printf(0,print_line++,"                                    ");
	jo_printf(0,print_line++,"                                    ");
	jo_printf(0,print_line++,"                                    ");
	jo_printf(0,print_line++,"                                    ");
	
	//	Generate list of rooms to draw
	int room_draw_coords[MAX_ROOM_DISPLAY][3];
	//	Current room
	int room_draw_count = 0;
	if(roomMaster[current_room[X]][current_room[Y]][current_room[Z]].vis)
	{
		room_draw_coords[0][0] = current_room[X];
		room_draw_coords[0][1] = current_room[Y];
		room_draw_coords[0][2] = current_room[Z];
		room_draw_count++;
	}
	//	Loop through rooms
	int dist_compare = 1;
	int min_room[XYZ];
	int max_room[XYZ];
	while(room_draw_count < num_vis)
	{
		//	Find room limits
		min_room[X] = JO_MAX(current_room[X]-dist_compare, 0);
		max_room[X] = JO_MIN(current_room[X]+dist_compare, ROOM_NUM_X-1);
		min_room[Y] = JO_MAX(current_room[Y]-dist_compare, 0);
		max_room[Y] = JO_MIN(current_room[Y]+dist_compare, ROOM_NUM_Y-1);
		min_room[Z] = JO_MAX(current_room[Z]-dist_compare, 0);
		max_room[Z] = JO_MIN(current_room[Z]+dist_compare, ROOM_NUM_Z-1);
		
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
		slRotY(-theta[Y]);	
		//	Translate draw location
		slTranslate(room_grid[X] * roomMaster[i][j][k].dist[X] - pl_position[X],
			-room_grid[Y] * roomMaster[i][j][k].dist[Y] - pl_position[Y] + cam_height,
			room_grid[Z] * roomMaster[i][j][k].dist[Z] - pl_position[Z]);		
		
		//	Rotate object
		slRotY(-roomMaster[i][j][k].theta);	
		//	Sets scale of 3D draw, larger numbers draw bigger		
		slScale(scale_factor, scale_factor, scale_factor);	
	
		//	Draw Interior
		slPutPolygon((XPDATA*)entities[roomMaster[i][j][k].entity_ID].pol[0]);
		//	Draw Hull
		slPutPolygon((XPDATA*)entities[roomMaster[i][j][k].entity_ID + 5].pol[0]);
		
		
		slPopMatrix();		//	Return matrix buffer pointer back 1 step
	}
	
	//	Draw Skybox
	//	Purge cache
	slCashPurge();
	
	//	Set coords
	i = room_draw_coords[room_draw_count][0];
	j = room_draw_coords[room_draw_count][1];
	k = room_draw_coords[room_draw_count][2];
	
	slPushMatrix();		//	Advance matrix buffer pointer 1 step

	//	Rotate perspective for player angle 
	slRotY(-theta[Y]);	
	
	//	Rotate object
	slRotY(-roomMaster[i][j][k].theta);	
	//	Sets scale of 3D draw, larger numbers draw bigger		
	slScale(scale_factor>>5, scale_factor>>5, scale_factor>>5);	

	//	Draw
	slPutPolygon((XPDATA*)entities[10].pol[0]);
	
	slPopMatrix();		//	Return matrix buffer pointer back 1 step
	
	return;
}

void load_rooms(void)
{
	/*
		Load Rooms
		
		Loads in entity data from ZTP
	*/
	
	//	Start loading in LWRAM beginning
    void * currentAddress = (void*)LWRAM;
	
	//	Load images
	
	//	Load rooms
	int ID = 0;
	
	//	Interiors
	currentAddress = ztLoad3Dmodel((Sint8*)"COR1W.ZTP", currentAddress, ID++, false);
	currentAddress = ztLoad3Dmodel((Sint8*)"COR2SW.ZTP", currentAddress, ID++, false);
	currentAddress = ztLoad3Dmodel((Sint8*)"COR2AW.ZTP", currentAddress, ID++, false);
	currentAddress = ztLoad3Dmodel((Sint8*)"COR3.ZTP", currentAddress, ID++, false);
	currentAddress = ztLoad3Dmodel((Sint8*)"COR4.ZTP", currentAddress, ID++, false);
	
	//	Exteriors
	currentAddress = ztLoad3Dmodel((Sint8*)"COR1W_W.ZTP", currentAddress, ID++, false);
	currentAddress = ztLoad3Dmodel((Sint8*)"COR2SW_W.ZTP", currentAddress, ID++, false);
	currentAddress = ztLoad3Dmodel((Sint8*)"COR2AW_W.ZTP", currentAddress, ID++, false);
	currentAddress = ztLoad3Dmodel((Sint8*)"COR3_W.ZTP", currentAddress, ID++, false);
	currentAddress = ztLoad3Dmodel((Sint8*)"COR4_W.ZTP", currentAddress, ID++, false);
	
	//	Skybox
	currentAddress = ztLoad3Dmodel((Sint8*)"SKY.ZTP", currentAddress, ID++, false);
	
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
	position[X] = 1;
	position[Y] = 0;
	position[Z] = 1;
	create_room(position);
	
	// Room
	position[X] = 0;
	position[Y] = 0;
	position[Z] = 1;
	create_room(position);
	
	// Room
	position[X] = 0;
	position[Y] = 0;
	position[Z] = 0;
	create_room(position);
	
	// Room
	position[X] = 2;
	position[Y] = 0;
	position[Z] = 1;
	create_room(position);
	
	// Room
	position[X] = 2;
	position[Y] = 0;
	position[Z] = 0;
	create_room(position);
	
	// Room
	position[X] = 3;
	position[Y] = 0;
	position[Z] = 1;
	create_room(position);
	
	// Room
	position[X] = 4;
	position[Y] = 0;
	position[Z] = 1;
	create_room(position);
	
	// Room
	position[X] = 5;
	position[Y] = 0;
	position[Z] = 1;
	create_room(position);
	
	// Room
	position[X] = 6;
	position[Y] = 0;
	position[Z] = 1;
	create_room(position);
	
	// Room
	position[X] = 7;
	position[Y] = 0;
	position[Z] = 1;
	create_room(position);
	
	
	// Room
	position[X] = 1;
	position[Y] = 0;
	position[Z] = 0;
	create_room(position);
	
	// Room
	position[X] = 1;
	position[Y] = 0;
	position[Z] = 2;
	create_room(position);
	
	// Room
	position[X] = 1;
	position[Y] = 0;
	position[Z] = 3;
	create_room(position);
	
	// Room
	position[X] = 1;
	position[Y] = 0;
	position[Z] = 4;
	create_room(position);
	
	// Room
	position[X] = 1;
	position[Y] = 0;
	position[Z] = 5;
	create_room(position);
	
	// Room
	position[X] = 1;
	position[Y] = 0;
	position[Z] = 6;
	create_room(position);
	
	// Room
	position[X] = 1;
	position[Y] = 0;
	position[Z] = 7;
	create_room(position);
	
	// Room
	position[X] = 1;
	position[Y] = 0;
	position[Z] = 8;
	create_room(position);
	
	// Room
	position[X] = 3;
	position[Y] = 0;
	position[Z] = 2;
	create_room(position);
	
	// Room
	position[X] = 3;
	position[Y] = 0;
	position[Z] = 3;
	create_room(position);
	
	// Room
	position[X] = 2;
	position[Y] = 0;
	position[Z] = 3;
	create_room(position);
	
	//	Set current room
	current_room[X] = 1;
	current_room[Y] = 0;
	current_room[Z] = 1;
	
	return;
}
