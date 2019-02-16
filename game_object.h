#include <jo/jo.h>

#ifndef game_object_h
#define game_object_h
//  Game Object containing necessary items for rendering
typedef struct {
	//	ID
	int ID;
	
    //  Render flag
    bool alive;

    // Position
    int x;
    int y;
    int z;
	
	// Velocity
	int vx;
    int vy;
    int vz;

    // Rotation
    int rx;
    int ry;
    int rz;
	
	// Rotation Speed
	int vrx;
    int vry;
    int vrz;
	
	//  Set Scale
	int sx;
	int sy;
	int sz;
		
	//  Set Size (Hitboxes)
	int x_size;
	int y_size;
	int z_size;

    //  3D Components
    jo_vertice* vertices;
    jo_3d_quad* quads;
	int num_quad;
}game_object;

//  Array of game objects
const int max_objects = 100;
static int num_object = 0;
static game_object object[100];


//	Creates a game_object and add it to the objects array
void create_object(int x, int y, int z, jo_vertice* v, jo_3d_quad* q, int num_quad)
{
	if(num_object < max_objects)
	{
		//	Increment object number
		num_object++;
		
		//	ID
		object[num_object-1].ID = num_object-1;
		
		//  Set alive
		object[num_object-1].alive = true;

		//  Set Position
		object[num_object-1].x = x;
		object[num_object-1].y = y;
		object[num_object-1].z = z;
		
		//  Set Velocity
		object[num_object-1].vx = 0;
		object[num_object-1].vy = 0;
		object[num_object-1].vz = 0;

		//  Set Rotation
		object[num_object-1].rx = 0;
		object[num_object-1].ry = 0;
		object[num_object-1].rz = 0;
		
		//  Set Rotation Speed
		object[num_object-1].vrx = 0;
		object[num_object-1].vry = 0;
		object[num_object-1].vrz = 0;
		
		//  Set Scale
		object[num_object-1].sx = 1;
		object[num_object-1].sy = 1;
		object[num_object-1].sz = 1;
		
		//  Set Size (Hitboxes)
		object[num_object-1].x_size = -1;
		object[num_object-1].y_size = -1;
		object[num_object-1].z_size = -1;

		//  Set quads and vertices
		object[num_object-1].vertices = v;
		object[num_object-1].quads = q;
		object[num_object-1].num_quad = num_quad;
	}
}

//	Clone Object from ID to destination ID in object array
void clone_object(int ID, int destination)
{
	//  Set Position
    object[destination].x = object[ID].x;
    object[destination].y = object[ID].y;
    object[destination].z = object[ID].z;
	
	//  Set Velocity
    object[destination].vx = object[ID].vx;
    object[destination].vy = object[ID].vy;
    object[destination].vz = object[ID].vz;

    //  Set Rotation
    object[destination].rx = object[ID].rx;
    object[destination].ry = object[ID].ry;
    object[destination].rz = object[ID].rz;
	
	//  Set Rotation Speed
    object[destination].vrx = object[ID].vrx;
    object[destination].vry = object[ID].vry;
    object[destination].vrz = object[ID].vrz;

    //  Set quads and vertices
    object[destination].vertices = object[ID].vertices;
    object[destination].quads = object[ID].quads;
}

//	Destroy Object
void destroy_object(int ID)
{
	if(num_object > 0)
	{
		// Clone last object into destroyed object's lcoation
		clone_object(num_object-1, ID);
		
		// Resize objects
		num_object--;
	}
}

//	Find closest object and return object ID
int closest_object(int x, int y, int z, int threshold)
{
	int closest_ID = -1;
	int sqr_dist;
	int max_sqr_dist = threshold*threshold;
	if(num_object > 0)
	{
		int index;
		for(index = 0; index < num_object; index++)
		{
			sqr_dist = (object[index].x - x)*(object[index].x - x) +
						(object[index].y - y)*(object[index].y - y) +
						(object[index].z - z)*(object[index].z - z);
			if(sqr_dist < max_sqr_dist)
			{
				closest_ID = index;
				max_sqr_dist = sqr_dist;
			}
		}
	}
	
	return closest_ID;
}

//	Basic bounding box colission
bool is_colliding(int x, int y, int z, int x_size, int y_size, int z_size)
{
	int index;
	int x_collide = false;
	int y_collide = false;
	int z_collide = false;

	for(index = 0; index < num_object; index++)
	{
		//	Reset values
		x_collide = false;
		y_collide = false;
		z_collide = false;
		
		//	Check x
		if(object[index].sx >= 0 && (JO_ABS(x-object[index].x) < (object[index].x_size*object[index].sx + x_size)/2))
		{
			x_collide = true;
		}
		
		//	Check y
		if(object[index].sy >= 0 && (JO_ABS(y-object[index].y) < (object[index].y_size*object[index].sy + y_size)/2))
		{
			y_collide = true;
		}
		
		//	Check z
		if(object[index].sz >= 0 && (JO_ABS(z-object[index].z) < (object[index].z_size*object[index].sz + z_size)/2))
		{
			z_collide = true;
		}
		
		jo_printf(0, 2, "Collisions:\t%d\t%d\t%d ",(int) x_collide, (int) y_collide, (int) z_collide);
		
		//	Return if colliding
		if(x_collide && y_collide && z_collide)
		{
			return true;
		}
	}
	return false;
}
#endif
