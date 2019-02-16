/*
** Seniriu
** Matthew Suttinger & Emerald Nova (johannesfetz@gmail.com)
** 
** This work is licensed under a Attribution-NonCommercial 4.0 International License
** More info at: https://creativecommons.org/licenses/by-nc/4.0/legalcode
*/

#include <jo/jo.h>
#include "templates.h"

//  Game Object containing necessary items for rendering
typedef struct {
    //  Render flag
    bool alive;

    // Position
    int x;
    int y;
    int z;

    // Rotation
    int rx;
    int ry;
    int rz;

    //  3D Components
    jo_vertice* vertices;
    jo_3d_quad* quads;
}game_object;

//  Initialize Camera
jo_camera       cam;
int             cam_pos[3] = {0,0,0};
int             cam_angle[3] = {0,0,0};
int             cam_tar[3] = {0,0,1};

//  Array of game objects
game_object     objects[4];

void create_cube(game_object* cube, int x, int y, int z, int rx, int ry, int rz, jo_vertice* v, jo_3d_quad* q)
{
    //  Set alive
    cube->alive = true;

    //  Set Location
    cube->x = x;
    cube->y = y;
    cube->z = z;

    //  Set Rotation
    cube->rx = rx;
    cube->ry = ry;
    cube->rz = rz;

    //  Set quads and vertices
    cube->vertices = v;
    cube->quads = q;
}

void draw_object(game_object* go)
{
    //  Perform matrix manipulations and draw
    jo_3d_push_matrix();
    {
        jo_3d_translate_matrix(go->x, go->y, go->z);
        jo_3d_rotate_matrix(go->rx, go->ry, go->rz);
        jo_3d_draw_array(go->quads, 6);
    }

    //  Restore default matrix
    jo_3d_pop_matrix();
}

void my_draw(void)
{
    //  Set Camera
    jo_3d_camera_look_at(&cam);

    //  Draw 3D Objects
    draw_object(&objects[0]);
    draw_object(&objects[1]);
    draw_object(&objects[2]);
    draw_object(&objects[3]);

    //  Write Text to Screen
    jo_printf(0, 0, "              *Test Game*              ");
    jo_printf(0, 3, "Camera Angle: %4d Degrees", cam_angle[1]);
    jo_printf(0, 4, "Camera Position:%7d,%7d,%7d ", cam_pos[0], cam_pos[1], cam_pos[2]);
    jo_printf(0, 5, "Camera Target:  %7d,%7d,%7d ", cam_tar[0], cam_tar[1], cam_tar[2]);
    jo_printf(0, 27, "Polygons: %7d ", jo_3d_get_polygon_count());
    jo_printf(0, 28, "Vertices: %7d ", jo_3d_get_vertices_count());
    jo_printf(0, 29, "Displayed Polygons: %7d ", jo_3d_get_displayed_polygon_count());
}

//  Handle Input from Gamepad
void gamepad_input(void)
{
    int delta[3] = {0,0,0};

	if (!jo_is_pad1_available())
		return ;
	if (jo_is_pad1_key_pressed(JO_KEY_UP))
		delta[1] = -5;
	if (jo_is_pad1_key_pressed(JO_KEY_DOWN))
		delta[1] = 5;
    if (jo_is_pad1_key_pressed(JO_KEY_LEFT))
		delta[0] = -5;
	if (jo_is_pad1_key_pressed(JO_KEY_RIGHT))
		delta[0] = 5;
    if (jo_is_pad1_key_pressed(JO_KEY_C))
		delta[2] = -5;
	if (jo_is_pad1_key_pressed(JO_KEY_Z))
		delta[2] = 5;
	if (jo_is_pad1_key_pressed(JO_KEY_L))
		cam_angle[1] -= 1;
	if (jo_is_pad1_key_pressed(JO_KEY_R))
		cam_angle[1] += 1;
    if (jo_is_pad1_key_pressed(JO_KEY_START))
    {
        cam_pos[0] = 0;
        cam_pos[1] = 0;
        cam_pos[2] = 0;
        cam_angle[0] = 0;
        cam_angle[1] = 0;
        cam_angle[2] = 0;
    }

    //  Bound Camera Angle
    if (cam_angle[1] > 180)
        cam_angle[1] -= 360;
    else if (cam_angle[1] <= -180)
        cam_angle[1] += 360;

    //  Fix Camera Position
    cam_pos[0] += (delta[0]*jo_cos(cam_angle[1]) + delta[2]*jo_sin(cam_angle[1]))/32768;
    cam_pos[1] += delta[1];
    cam_pos[2] += (delta[2]*jo_cos(cam_angle[1]) - delta[0]*jo_sin(cam_angle[1]))/32768;

    //  Fix Camera Target/Orientation
    cam_tar[0] = (cam_pos[0]*32768 + 100*jo_sin(cam_angle[1]))/32768;
    cam_tar[1] = cam_pos[1];
    cam_tar[2] = (cam_pos[2]*32768 + 100*jo_cos(cam_angle[1]))/32768;
}

//  Main Logic Loop
void main_loop(void)
{
    //  Perform rotation
    objects[0].rx += 1;
    objects[2].rz += 2;

    //  Handle Input
    gamepad_input();

    //  Move & Rotate camera
    jo_3d_camera_set_viewpoint(&cam,cam_pos[0],cam_pos[1],cam_pos[2]);
    jo_3d_camera_set_target(&cam,cam_tar[0],cam_tar[1],cam_tar[2]);

    //  Draw Objects
    my_draw();
}

void jo_main(void)
{
    //  Initialize engine with black background
	jo_core_init(JO_COLOR_Black);

	//  Initialize camera
    jo_3d_camera_init(&cam);

    //  Load texture
    jo_sprite_add_tga(JO_ROOT_DIR, "BOX.TGA", JO_COLOR_Transparent);

    //  Initialize Templates
    initilize_geo("metal_crate_1");

    //  Create cube
    create_cube(&objects[0], -200, 0, 500, 20, 45, 0, metal_crate_1_v, metal_crate_1_q);
    create_cube(&objects[1], 0, 0, 500, 20, 45, 0, metal_crate_1_v, metal_crate_1_q);
    create_cube(&objects[2], 200, 0, 500, 20, 45, 0, metal_crate_1_v, metal_crate_1_q);
    create_cube(&objects[3], 0, 0, -500, 20, 45, 0, metal_crate_1_v, metal_crate_1_q);

    //  Set main loop
	jo_core_add_callback(main_loop);

	//  Start game
	jo_core_run();
}
