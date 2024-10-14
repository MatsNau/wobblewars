#ifndef SPRITEMANAGER_H
#define SPRITEMANAGER_H

#include <string>
#include <vector>
#include <iostream>

class SpriteManager {
private:

public:
	SpriteManager();
	void initSprite(std::string path, int id, int size, int screen, int ram, int vram, bool keepframes, int slot);
	void createSprite(int screen, int id, int gfx, int pal, int x, int y);
	void removeSprite(int screen, int id);
	void moveSprite(int screen, int id, int x, int y);
	void hideSprite(int screen, int id);
	void showSprite(int screen, int id, int x, int y);
	std::vector<int> animateSprite(int spritAnim, int spriteFrame, int screen, int id, int maxFrames);
};

#endif
