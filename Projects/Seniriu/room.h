/*
** Seniriu
** Matthew Suttinger & Emerald Nova (emeraldnovagames@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

#ifndef __ROOM_H__
#define __ROOM_H__

//	Includes
#include <jo/jo.h>
#include "ZT/ZT_COMMON.h"
#include "display.h"
#include "input.h"

//	Definitions
#define ROOM_NUM_X (10)
#define ROOM_NUM_Y (1)
#define ROOM_NUM_Z (10)

//	Struct
//  Game Object containing necessary items for rendering
typedef struct {
	//	Room Exists
	bool alive;
	
	//	Coordinate
	int coord[XYZ];
	
	//	Angle to draw
	ANGLE theta;
	
	//	Entity ID
	int entity_ID;
	/*
		0 - 1 door
		1 - 2 door straight
		2 - 2 door angled
		3 - 3 door
		4 - 4 door
	*/
	
	//	XPData pointer
	XPDATA *pDataStart;
	
	//	Adjacet doors
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
	int doors[6];
	
	//	Opacity
	/*
		Direction same indices as doors
	
		0	-	Full view
		1	-	Restricted view (I.E. Window/door, reduced FOV)
		2	-	Wall (no draws beyond that)
	*/
	int opacity[6];
	
	//	Visible (for drawing, rewritten every frame)
	bool vis;
	//	Distance from current room (Manhatten)
	int dist[XYZ];
}room;

//	Variables
//	Pointer to copy of XPDATA 
extern XPDATA *pdataRoomMaster[ROOM_NUM_X][ROOM_NUM_Y][ROOM_NUM_Z];
//	Room container 
extern room roomMaster[ROOM_NUM_X][ROOM_NUM_Y][ROOM_NUM_Z];
//	Current room contianing camera
extern int current_room[XYZ];
//	Grid spacing
extern FIXED room_grid[XYZ];
//	Room Count
extern int room_num;
//	Scaling factor
extern FIXED scale_factor;

//	Functions
void initialize_rooms(void);
void create_room(int position[XYZ]);
void map_rooms(void);
void draw_rooms(void);
void load_rooms(void);
void generate_rooms(void);


#endif
