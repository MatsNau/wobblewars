
//{{BLOCK(startscreen)

//======================================================================
//
//	startscreen, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 118 tiles (t|f|p reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 512 + 7552 + 2048 = 10112
//
//	Time-stamp: 2024-11-21, 22:03:11
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_STARTSCREEN_H
#define GRIT_STARTSCREEN_H

#define startscreenTilesLen 7552
extern const unsigned short startscreenTiles[3776];

#define startscreenMapLen 2048
extern const unsigned short startscreenMap[1024];

#define startscreenPalLen 512
extern const unsigned short startscreenPal[256];

#endif // GRIT_STARTSCREEN_H

//}}BLOCK(startscreen)
