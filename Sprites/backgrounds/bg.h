
//{{BLOCK(bg)

//======================================================================
//
//	bg, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 790 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 50560 + 2048 = 53120
//
//	Time-stamp: 2024-10-20, 00:52:30
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BG_H
#define GRIT_BG_H

#define bgTilesLen 50560
extern const unsigned short bgTiles[25280];

#define bgMapLen 2048
extern const unsigned short bgMap[1024];

#define bgPalLen 512
extern const unsigned short bgPal[256];

#endif // GRIT_BG_H

//}}BLOCK(bg)
