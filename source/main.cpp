#include <stdio.h>
#include <nds.h>
#include <nf_lib.h>

#include <ctime>
#include <vector>

#include "nina.h"
#include "enemy.h"

#define MAX_ENEMIES 10
Nina nina(128, 96);  // Start Nina at the center of the screen
char score[32];
char health[32];
int weaponSpriteId = 0;
int enemySpawnTimer = 0;
std::vector<Enemy> enemies(MAX_ENEMIES);

void updateScore()
{
    sprintf(score, "SCORE: %d", nina.getScore());
    NF_ClearTextLayer(0, 0);
    NF_WriteText(0, 0, 2, 2, score);
    NF_UpdateTextLayers();
}

void updateHealth()
{
    sprintf(health, "<3: %d", nina.getHealth());
    NF_ClearTextLayer(0, 1);
    NF_WriteText(0, 1, 15, 2, health);
    NF_UpdateTextLayers();
}

void resetGame()
{
    for (auto &enemy : enemies)
    {
        if (enemy.isActive())
        {
            enemy.setActive(false);
            NF_DeleteSprite(0, 2 + (&enemy - &enemies[0]));
        }
    }
    nina = Nina(128, 96);
    enemySpawnTimer = 0;
    updateScore();
    updateHealth();
}

int main(int argc, char** argv)
{
    consoleDemoInit();
    consoleClear();
    NF_Set2D(0, 0);
    NF_Set2D(1, 0);
    NF_SetRootFolder("NITROFS");

    NF_InitTiledBgBuffers();
    NF_InitTiledBgSys(0);
    NF_InitTiledBgSys(1);
    //
    NF_LoadTiledBg("backgrounds/bg", "bg", 256, 256);
    NF_CreateTiledBg(0, 3, "bg");

    NF_InitSpriteBuffers();
    NF_InitSpriteSys(0);
    NF_InitSpriteSys(1);
    
    //Load Character Sprites
    //Idle with Weapon
    NF_LoadSpriteGfx("sprites/nina/idle", 0, 32, 32); // Assuming Nina is 32x32 pixels
    NF_LoadSpritePal("sprites/nina/idle",0);  
    NF_VramSpriteGfx(0, 0, 0, false);
    NF_VramSpritePal(0, 0, 0);
    //Walking with Weapon
    //Weapon Throw
    //Idle Without Weapon
    //Walking without Weapon
    //Dying

    //Load Weapon Sprites
    //Flying 
    NF_LoadSpriteGfx("sprites/weapon/throw", 1, 32, 32); // Assuming Nina is 32x32 pixels
    NF_LoadSpritePal("sprites/weapon/throw", 1);
    NF_VramSpriteGfx(0, 1, 1, false);
    NF_VramSpritePal(0, 1, 1);

    //Load Enemies
    //Walking
    //Dying
    NF_LoadSpritePal("palettes/apple", 2);
    /*NF_LoadSpritePal("palettes/nina", 0);
    NF_LoadSpritePal("palettes/weapon", 1);*/

    NF_VramSpritePal(0, 1, 1);
    NF_VramSpritePal(0, 2, 2);

    //Score Initialization
    NF_InitTextSys(0);
    NF_LoadTextFont("fonts/font", "default", 256, 256, 0);
    NF_CreateTextLayer(0, 0, 0, "default");
    //Health Initialization
    NF_CreateTextLayer(0, 1, 0, "default");

    // Create sprites for Nina and the weapon
    NF_CreateSprite(0, 0, 0, 0, nina.getX(), nina.getY());  // Nina sprite
    NF_CreateSprite(0, 1, 1, 1, nina.getWeapon().getX(), nina.getWeapon().getY());  // Weapon sprite

    //Enemy Initialization
    const int ENEMY_SPAWN_INTERVAL = 180; // 3 seconds at 60 FPS

    std::srand(std::time(0)); // Seed for random number generation

    updateHealth();

    //Character Animation parameters
    int char_frame = 0;
    int char_anim = 0;

    //Weapon Animation parameters
    int weapon_frame = 0;
    int weapon_anim = 0;

    while (1)
    {
        //TODO: ADD START UP SCREEN WITH SOME INITIAL DIALOG
        //TODO: ADD DEATH SCREEN AND RESET OF THE GAME
        //TODO: ADD WINNING SCREEN

        //TODO: CREATE MANAGER FOR SPRITES THAT TAKES CAR OF ANIMATIONS
        //TODO: Extend classes by their sprites

        //TODO: ADD SPRITES OF NINA
        //TODO: ADD SPRITES OF AXE
        //TODO: ADD SPRITES OF ENEMY
        //TODO: ADD Background
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

        //ANIMATION TEST
        char_anim++;
        if (char_anim > 5)
        {
            char_anim = 0;
            char_frame++;
            if (char_frame > 3)
                char_frame = 0;
            NF_SpriteFrame(0, 0, char_frame);
            
        }
        //ANIMTAION TEST

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
            //WEAPON ANIMATION TEST
            weapon_anim++;
            if (weapon_anim > 5)
            {
                weapon_anim = 0;
                weapon_frame++;
                if (weapon_frame > 3)
                    weapon_frame = 0;
                NF_SpriteFrame(0, 1, weapon_frame);
            }
            //WEAPON ANIMATION TEST
        }
        else {
            NF_ShowSprite(0, 1, false);
        }
        NF_MoveSprite(0, 1, weapon.getX(), weapon.getY());

        // Enemy spawning
        enemySpawnTimer++;
        if (enemySpawnTimer >= ENEMY_SPAWN_INTERVAL) {
            for (auto& enemy : enemies) {
                if (!enemy.isActive()) {
                    int startX = std::rand() % 256; // Random x position
                    int startY = std::rand() % 192; // Random y position
                    enemy.spawn(startX, startY);
                    NF_CreateSprite(0, 2 + (&enemy - &enemies[0]), 0, 2, enemy.getX(), enemy.getY());
                    break;
                }
            }
            enemySpawnTimer = 0;
        }

        // Enemy movement and collision detection
        for (auto& enemy : enemies) {
            if (enemy.isActive()) {
                enemy.moveTowards(nina.getX(), nina.getY());
                NF_MoveSprite(0, 2 + (&enemy - &enemies[0]), enemy.getX(), enemy.getY());

                // Weapon collision
                const Weapon& weapon = nina.getWeapon();
                if (weapon.isVisible()) {
                    int dx = enemy.getX() - weapon.getX();
                    int dy = enemy.getY() - weapon.getY();
                    if (dx * dx + dy * dy < 64) { // Assuming 8x8 sprite, so 8*8 = 64
                        enemy.setActive(false);
                        NF_DeleteSprite(0, 2 + (&enemy - &enemies[0]));
                        //Increase Score
                        nina.increaseScore();
                    }
                }

                // Nina colission
                int dx = enemy.getX() - nina.getX();
                int dy = enemy.getY() - nina.getY();
                if (dx * dx + dy * dy < 64) { // Assuming 8x8 sprite, so 8*8 = 64
                    enemy.setActive(false);
                    NF_DeleteSprite(0, 2 + (&enemy - &enemies[0]));
                    //Reduce Health
                    nina.reduceHealth();
                }
            }
        }


        if (nina.getHealth() <= 0)
        {
            //STOP THE GAME GO TO RESTART SCREEN
            resetGame();
        }

        updateScore();
        updateHealth();

        NF_SpriteOamSet(0);
        swiWaitForVBlank();
        oamUpdate(&oamMain);
    }
    return 0;
}