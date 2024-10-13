#include <stdio.h>
#include <nds.h>
#include <nf_lib.h>
#include <string>
#include <iostream>
#include <ctime>
#include <vector>

#include "spriteManager.h"

SpriteManager::SpriteManager() {
    //Nothing here
}

void SpriteManager::initSprite(std::string path, int id, int size, int screen, int ram, int vram, bool keepframes, int slot) 
{
    NF_LoadSpriteGfx(path.c_str(), id, size, size);
    NF_LoadSpritePal(path.c_str(), id);
    NF_VramSpriteGfx(screen, ram, vram, keepframes);
    NF_VramSpritePal(screen, id, slot);
}

void SpriteManager::createSprite( int screen, int id, int gfx, int pal, int x, int y) {
    NF_CreateSprite(screen, id, gfx, pal, x, y);
}

void SpriteManager::removeSprite() {
    // Implementierung hinzufügen
}

void SpriteManager::moveSprite() {
    // Implementierung hinzufügen
}

std::vector<int> SpriteManager::animateSprite(int spriteAnim, int spriteFrame, int screen, int id, int maxFrames) 
{
    spriteAnim++;
    if (spriteAnim > 5)
    {
        spriteAnim = 0;
        spriteFrame++;
        if (spriteFrame > maxFrames)
            spriteFrame = 0;
        NF_SpriteFrame(screen, id, spriteFrame);
    }
    return { spriteAnim, spriteFrame };
}

