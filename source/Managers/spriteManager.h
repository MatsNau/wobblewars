#include <stdio.h>
#include <nds.h>
#include <nf_lib.h>

#include <ctime>
#include <vector>

#include "nina.h"
#include "enemy.h"

#ifndef SPRITEMANAGER_H
#define SPRITEMANAGER_H

class spriteManager {
private:

public:
	void initSprites();
	void createSprite();
	void removeSprite();
	void moveSprite();
	void animateSprite();
};

#endif
