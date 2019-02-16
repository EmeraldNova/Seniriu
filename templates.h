#include <jo/jo.h>
#include <string.h>

jo_vertice    metal_crate_1_v[] = JO_3D_CUBE_VERTICES(32);
jo_3d_quad    metal_crate_1_q[6];

//  Initialize geometry for selected object for copying
void initilize_geo(char *name)
{
    jo_printf(0, 1, "Comparing string...");
    if (strcmp("metal_crate_1", name) == 0)
    {
        jo_printf(0, 2, "metal_crate_1");

        //  Write textures
        jo_3d_create_cube(metal_crate_1_q, metal_crate_1_v);
        for (int i = 0; i < 6; ++i)
        {
            jo_3d_set_texture(&metal_crate_1_q[i], 0);
        }
    }
}
