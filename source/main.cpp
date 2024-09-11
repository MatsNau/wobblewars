#include <stdio.h>
#include <nds.h>
#include <nf_lib.h>
#include "nina.h"

Nina nina(128, 96);  // Start Nina at the center of the screen
char score[32];
int weaponSpriteId = 0;

/*void updateScore(int score)
{
    sprintf(score, "SCORE: %d", score);
    NF_ClearTextLayer(0, 0);
    NF_WriteText(0, 0, 2, 2, score);
    NF_UpdateTextLayers();
}*/

int main(int argc, char** argv)
{
    consoleDemoInit();
    consoleClear();
    NF_Set2D(0, 0);
    NF_SetRootFolder("NITROFS");

    NF_InitTiledBgBuffers();
    NF_InitTiledBgSys(0);

    NF_LoadTiledBg("backgrounds/bg", "bg", 256, 256);
    NF_CreateTiledBg(0, 3, "bg");

    NF_InitSpriteBuffers();
    NF_InitSpriteSys(0);

    NF_LoadSpriteGfx("sprites/tile", 0, 8, 8);
    NF_LoadSpritePal("palettes/apple",0);  // Assuming Nina is 8x8 pixels
    NF_LoadSpritePal("palettes/apple",1);  // Assuming weapon is 8x8 pixels
    /*NF_LoadSpritePal("palettes/nina", 0);
    NF_LoadSpritePal("palettes/weapon", 1);*/

    //NF_VramSpriteGfx(0, 0, 0, true);
    //NF_VramSpriteGfx(0, 1, 1, true);
    NF_VramSpriteGfx(0, 0, 0, true);
    NF_VramSpritePal(0, 0, 0);
    NF_VramSpritePal(0, 1, 1);

    NF_InitTextSys(0);

    NF_LoadTextFont("fonts/font", "default", 256, 256, 0);
    NF_CreateTextLayer(0, 0, 0, "default");
    //updateScore(0);  // Initialize score display

    // Create sprites for Nina and the weapon
    NF_CreateSprite(0, 0, 0, 0, nina.getX(), nina.getY());  // Nina sprite
    NF_CreateSprite(0, 1, 0, 1, nina.getWeapon().getX(), nina.getWeapon().getY());  // Weapon sprite

    while (1)
    {
        scanKeys();
        touchPosition touch;
        touchRead(&touch);

        if (keysHeld() & KEY_START)
        {
            break;
        }

        // Handle Nina's movement
        //TODO: ADD Diagonal Movement
        if (keysHeld() & KEY_UP)
        {
            nina.move(Nina::UP);
        }
        else if (keysHeld() & KEY_DOWN)
        {
            nina.move(Nina::DOWN);
        }
        else if (keysHeld() & KEY_LEFT)
        {
            nina.move(Nina::LEFT);
        }
        else if (keysHeld() & KEY_RIGHT)
        {
            nina.move(Nina::RIGHT);
        }

        // Handle weapon throwing
        if (keysDown() & KEY_TOUCH)
        {
            nina.throwWeapon(touch.px, touch.py);
        }

        // Update weapon position
        nina.updateWeapon();

        // Update Nina's sprite
        NF_MoveSprite(0, 0, nina.getX(), nina.getY());

        // Update weapon's sprite
        const Weapon& weapon = nina.getWeapon();
        if (nina.isWeaponVisible()) {
            NF_ShowSprite(0, 1, true);
            NF_MoveSprite(0, 1, weapon.getX(), weapon.getY());
        }
        else {
            NF_ShowSprite(0, 1, false);
        }
        NF_MoveSprite(0, 1, weapon.getX(), weapon.getY());

        //TODO: ADD ENEMIES
        // THEY SHOULD SPAWN RANDOMLY IN AN INTERVALL OF 3 Seconds (Increasing?)
        // THEY SHOUD TARGET THE PLAYER BY JUST MOVING TOWARDS THEM
        // IF THEY COLLIDE WITH PLAYER => DEATH OF PLAYER
        // IF THEY COLLIDE WITH AXE => DEATH OF NPC

        NF_SpriteOamSet(0);
        swiWaitForVBlank();
        oamUpdate(&oamMain);
    }
    return 0;
}