#include <jo/jo.h>
#include <string.h>

jo_vertice    metal_crate_1_v[] = JO_3D_CUBE_VERTICES(32);
jo_3d_quad    metal_crate_1_q[6];
int		      metal_crate_1_nq;

//  Initialize geometry for selected object for copying
void initilize_geo(char *name)
{
    if (strcmp("metal_crate_1", name) == 0)
    {
		//	Load Texture
		jo_sprite_add_tga(JO_ROOT_DIR, "BOX.TGA", JO_COLOR_Transparent);
		
        //  Write textures
        jo_3d_create_cube(metal_crate_1_q, metal_crate_1_v);
		metal_crate_1_nq = 6;
        for (int i = 0; i < metal_crate_1_nq; ++i)
        {
            jo_3d_set_texture(&metal_crate_1_q[i], 0);
        }
    }
}
