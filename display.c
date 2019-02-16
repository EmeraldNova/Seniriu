/*
** Seniriu
** Matthew Suttinger & Emerald Nova (johannesfetz@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

#include <jo/jo.h>
#include "game_object.h"

//  Initialize Camera
static jo_camera cam;

//  Initialize Camera Positional Variables
static int cam_pos[3] = {0,0,0};
static int cam_angle[3] = {0,0,0};
static int cam_tar[3] = {0,0,1};

//	Camera Initializtion function to be run on startup
void start_cam(void)
{
	jo_3d_camera_init(&cam);
}

//	Draws object according to coordinated and rotation
void draw_object(game_object* go)
{
    //  Perform matrix manipulations and draw
    jo_3d_push_matrix();
    {
        jo_3d_translate_matrix(go->x, go->y, go->z);
        jo_3d_rotate_matrix(go->rx, go->ry, go->rz);
        jo_3d_draw_array(go->quads, go->num_quad);
    }

    //  Restore default matrix
    jo_3d_pop_matrix();
}

//	Draws information on screen
void my_draw(void)
{
    //  Set Camera
    jo_3d_camera_look_at(&cam);

    //  Draw 3D Objects
	int index;
	for(index = 0; index < num_object; index++)
	{
		draw_object(&object[index]);
	}

    //  Write Text to Screen
    jo_printf(0, 27, "Polygons: %7d ", jo_3d_get_polygon_count());
    jo_printf(0, 28, "Vertices: %7d ", jo_3d_get_vertices_count());
    jo_printf(0, 29, "Displayed Polygons: %7d ", jo_3d_get_displayed_polygon_count());
}

//  Move & Rotate camera
void move_cam(void)
{
    jo_3d_camera_set_viewpoint(&cam,cam_pos[0],cam_pos[1],cam_pos[2]);
    jo_3d_camera_set_target(&cam,cam_tar[0],cam_tar[1],cam_tar[2]);
}
