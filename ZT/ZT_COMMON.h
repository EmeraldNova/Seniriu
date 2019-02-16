#ifndef ZT_COMMON_H
#define ZT_COMMON_H


#include <jo/jo.h>

/* MACROS FOR PALETTE LOOKUP*/
//VDP1 Color Lookup Table
#define     LUTaddr             (0x25C7A960)
#define     returnLUTaddr(n)    (LUTaddr + (0x20 * n))
#define     LUTidx(n)           (Uint16)(62764 + (4 * n))
//VDP2 color RAM
#define     CRAM_Base           (0x25f00200)
#define     returnCRAMaddr(n)   (CRAM_Base + (0x20 * n))
#define     LUTcramIdx(n)       ((256 + (16 * n)))

#define     LWRAM 0x00200000

typedef struct
{
    Uint16      width;
    Uint16      height;
    Uint16	    cmode ;	//Color type : 16 or 256
    Uint16      nbcol;
    Uint16  *   data;  //The palette index for the pixels
} img_t;

#include "ZT_TOOLS.h"
#include "ZT_LOAD_MODEL.h"



#endif // ZT_COMMON_H
