//ZT_CD.h
//File is compiled separately
//Include this for ZT CD functions (and possibly some of my own extensions)
#ifndef __ZT_CD_H__
# define __ZT_CD_H__

extern void * currentAddress;
extern void * HighEndAddr;

extern GfsDirTbl gfsDirTbl;
extern GfsDirName gfsDirName[];
extern Uint32 gfsLibWork[];
extern Sint32 gfsDirN;

void    ztCDinit(void);
void    ztCDsetDir(char * subDir);
void    ztCDsetRoot();

#endif

