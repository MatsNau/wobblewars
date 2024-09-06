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

    NF_LoadSpritePal("palettes/nina",0);  // Assuming Nina is 8x8 pixels
    NF_LoadSpritePal("palettes/weapon",1);  // Assuming weapon is 8x8 pixels
    /*NF_LoadSpritePal("palettes/nina", 0);
    NF_LoadSpritePal("palettes/weapon", 1);*/

    //NF_VramSpriteGfx(0, 0, 0, true);
    //NF_VramSpriteGfx(0, 1, 1, true);
    NF_VramSpritePal(0, 0, 0);
    NF_VramSpritePal(0, 1, 1);

    NF_InitTextSys(0);

    NF_LoadTextFont("fonts/font", "default", 256, 256, 0);
    NF_CreateTextLayer(0, 0, 0, "default");
    //updateScore(0);  // Initialize score display

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
        NF_MoveSprite(0, 1, weapon.getX(), weapon.getY());

        NF_SpriteOamSet(0);
        swiWaitForVBlank();
        oamUpdate(&oamMain);
    }
    return 0;
}