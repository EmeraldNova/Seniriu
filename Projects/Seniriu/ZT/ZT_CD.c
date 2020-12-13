//ZT CD
//File is compiled separately

#include "ZT_COMMON.h"

/**ZTP stands for Z-Treme polygonal model**/
void * currentAddress = (void*)(LWRAM);
void * HighEndAddr = (void*)(HIEND);

/**Added by XL2 to use my own CD loading functions**/
#define     OPEN_MAX    (Sint32)5
#define     DIR_MAX     (Sint32)25
#define     RD_UNIT     (10)
#define     SECT_SIZE   (2048)
GfsDirTbl gfsDirTbl;
GfsDirName gfsDirName[DIR_MAX];
Uint32 gfsLibWork[GFS_WORK_SIZE(OPEN_MAX)/sizeof(Uint32)];
Sint32 gfsDirN;
void    ztCDinit(void)
{
    GFS_DIRTBL_TYPE(&gfsDirTbl) = GFS_DIR_NAME;
    GFS_DIRTBL_DIRNAME(&gfsDirTbl) = gfsDirName;
    GFS_DIRTBL_NDIR(&gfsDirTbl) = DIR_MAX;
    gfsDirN = GFS_Init(OPEN_MAX, gfsLibWork, &gfsDirTbl);
}
//More XL2
void    ztCDsetDir(char * subDir)
{
    Sint32  fid;
    if (subDir != NULL)
    {
        fid = GFS_NameToId((Sint8 *)subDir);
        GFS_LoadDir(fid, &gfsDirTbl);
        GFS_SetDir(&gfsDirTbl);
    }

}
//More XL2
void    ztCDsetRoot()
{
    Sint32  fid;
    fid = GFS_NameToId((Sint8 *)"..");
    GFS_LoadDir(fid, &gfsDirTbl);
    GFS_SetDir(&gfsDirTbl);
}
//
