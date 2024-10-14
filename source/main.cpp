#include <stdio.h>
#include <nds.h>
#include <nf_lib.h>

#include <ctime>
#include <vector>

#include "nina.h"
#include "enemy.h"
#include "Managers/spriteManager.h"
//

#define MAX_ENEMIES 10
SpriteManager spriteManager; //Initializing SpriteManager
Nina nina(128, 96, spriteManager);  // Start Nina at the center of the screen
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
    nina.reset(128, 96);
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
    //Load Backgrounds
    NF_LoadTiledBg("backgrounds/bg", "bg", 256, 256);
    NF_CreateTiledBg(0, 3, "bg");

    NF_InitSpriteBuffers();
    NF_InitSpriteSys(0);
    NF_InitSpriteSys(1);
    
    //Load Character Sprites
    //Idle with Weapon
    spriteManager.initSprite("sprites/nina/idle", 0, 32, 0, 0, 0, false, 0);
    spriteManager.createSprite(0, 0, 0, 0, nina.getX(), nina.getY());
    //Idle Without Weapon
    spriteManager.initSprite("sprites/nina/idleWithoutWeapon", 1, 32, 0, 1, 1, false, 1);
    spriteManager.createSprite(0, 1, 1, 1, nina.getX(), nina.getY());
    spriteManager.hideSprite(0, 1);
    //Walking with Weapon
    spriteManager.initSprite("sprites/nina/walking", 2, 32, 0, 2, 2, false, 2);
    spriteManager.createSprite(0, 2, 2, 2, nina.getX(), nina.getY());
    spriteManager.hideSprite(0, 2);
    //Walking without Weapon
    spriteManager.initSprite("sprites/nina/walkingWithoutWeapon", 3, 32, 0, 3, 3, false, 3);
    spriteManager.createSprite(0, 3, 3, 3, nina.getX(), nina.getY());
    spriteManager.hideSprite(0, 3);
    //Weapon Throw
    //Dying
    //4

    //Load Weapon Sprites
    //Flying 
    spriteManager.initSprite("sprites/weapon/throw", 5, 32, 0, 5, 5, false, 5);
    spriteManager.createSprite(0, 5, 5, 5, nina.getWeapon().getX(), nina.getWeapon().getY());
    spriteManager.hideSprite(0, 5);
    //Load Enemies
    //Walking
    spriteManager.initSprite("sprites/Enemy/Ghost", 6, 32, 0, 6, 6, false, 6);
    spriteManager.createSprite(0, 6, 6, 6, 0, 0);
    spriteManager.hideSprite(0, 6);
    //Dying

    //Score Initialization
    NF_InitTextSys(0);
    NF_LoadTextFont("fonts/font", "default", 256, 256, 0);
    NF_CreateTextLayer(0, 0, 0, "default");
    //Health Initialization
    NF_CreateTextLayer(0, 1, 0, "default");

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

    int ninaSpriteId = 0;
    int ninaAnimationFrames = 0;

    while (1)
    {
        //TODO: ADD START UP SCREEN WITH SOME INITIAL DIALOG
        //TODO: ADD DEATH SCREEN AND RESET OF THE GAME
        //TODO: ADD WINNING SCREEN

        //TODO: CREATE MANAGER FOR SPRITES THAT TAKES CARE OF ANIMATIONS
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

        ninaSpriteId = nina.getCurrentSpriteId();
        ninaAnimationFrames = nina.getAnimationFrames();

        // Handle Nina's movement
        if (keysHeld() & KEY_UP)
        {
            nina.move(Nina::UP);
            nina.setWalking(true);
        }
        else if (keysHeld() & KEY_DOWN)
        {
            nina.move(Nina::DOWN);
            nina.setWalking(true);
        }
        else if (keysHeld() & KEY_LEFT)
        {
            nina.move(Nina::LEFT);
            nina.setWalking(true);
        }
        else if (keysHeld() & KEY_RIGHT)
        {
            nina.move(Nina::RIGHT);
            nina.setWalking(true);
        }

        //ANIMATION TEST
        std::vector<int> charAnimResult = spriteManager.animateSprite(char_anim, char_frame, 0, ninaSpriteId, ninaAnimationFrames);
        char_anim = charAnimResult[0];
        char_frame = charAnimResult[1];
        //ANIMTAION TEST

        // Handle weapon throwing
        if (keysDown() & KEY_TOUCH)
        {
            nina.throwWeapon(touch.px, touch.py);            
        }


        // Update weapon position
        nina.updateWeapon();

        // Update Nina's sprite
        spriteManager.moveSprite(0, ninaSpriteId, nina.getX(), nina.getY());
        
        if (nina.isWeaponVisible() && nina.getWalking())
        {
            nina.updateState(Nina::WALKING_WITHOUT_WEAPON);
        }
        else if (nina.isWeaponVisible() && !nina.getWalking())
        {
            nina.updateState(Nina::IDLE_WITHOUT_WEAPON);
        }
        else if (!nina.isWeaponVisible() && nina.getWalking())
        {
            nina.updateState(Nina::WALKING_WITH_WEAPON);
        }
        else
        {
            nina.updateState(Nina::IDLE_WITH_WEAPON);
        }

        // Update weapon's sprite
        const Weapon& weapon = nina.getWeapon();
        if (nina.isWeaponVisible()) 
        {
            NF_ShowSprite(0, 5, true);
            NF_MoveSprite(0, 5, weapon.getX(), weapon.getY());
            //WEAPON ANIMATION TEST
            weapon_anim++;
            if (weapon_anim > 5)
            {
                weapon_anim = 0;
                weapon_frame++;
                if (weapon_frame > 3)
                    weapon_frame = 0;
                NF_SpriteFrame(0, 5, weapon_frame);
            }
            //WEAPON ANIMATION TEST

        }
        else 
        {
            NF_ShowSprite(0, 5, false);
        }
        NF_MoveSprite(0, 5, weapon.getX(), weapon.getY());

        // Enemy spawning
        enemySpawnTimer++;
        if (enemySpawnTimer >= ENEMY_SPAWN_INTERVAL) {
            for (auto& enemy : enemies) {
                if (!enemy.isActive()) {
                    int startX = std::rand() % 256; // Random x position
                    int startY = std::rand() % 192; // Random y position
                    enemy.spawn(startX, startY);
                    NF_CreateSprite(0, 6 + (&enemy - &enemies[0]), 6, 6, enemy.getX(), enemy.getY());
                    break;
                }
            }
            enemySpawnTimer = 0;
        }

        // Enemy movement and collision detection
        for (auto& enemy : enemies) {
            if (enemy.isActive()) {
                enemy.moveTowards(nina.getX(), nina.getY());
                NF_MoveSprite(0, 6 + (&enemy - &enemies[0]), enemy.getX(), enemy.getY());

                // Weapon collision
                const Weapon& weapon = nina.getWeapon();
                if (weapon.isVisible()) {
                    int dx = enemy.getX() - weapon.getX();
                    int dy = enemy.getY() - weapon.getY();
                    if (dx * dx + dy * dy < 64) { // Assuming 8x8 sprite, so 8*8 = 64
                        enemy.setActive(false);
                        NF_DeleteSprite(0, 6 + (&enemy - &enemies[0]));
                        //Increase Score
                        nina.increaseScore();
                    }
                }

                // Nina colission
                int dx = enemy.getX() - nina.getX();
                int dy = enemy.getY() - nina.getY();
                if (dx * dx + dy * dy < 64) { // Assuming 8x8 sprite, so 8*8 = 64
                    enemy.setActive(false);
                    NF_DeleteSprite(0, 6 + (&enemy - &enemies[0]));
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

        nina.setWalking(false);

        updateScore();
        updateHealth();

        NF_SpriteOamSet(0);
        swiWaitForVBlank();
        oamUpdate(&oamMain);
    }
    return 0;
}