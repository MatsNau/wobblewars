#include <stdio.h>
#include <nds.h>
#include <nf_lib.h>

#include <ctime>
#include <vector>

#include "nina.h"
#include "enemy.h"
#include "Managers/spriteManager.h"
//

enum GameState
{
    INTRO,
    PLAYING,
    GAMEOVER,
    WINNING
};

GameState gameState = PLAYING;
#define MAX_ENEMIES 10
SpriteManager spriteManager; //Initializing SpriteManager
Nina nina(128, 96, spriteManager);  // Start Nina at the center of the screen
char score[32];
char health[32];
int weaponSpriteId = 0;
int enemySpawnTimer = 0;
std::vector<Enemy> enemies;
bool gameover = false;

void updateScore()
{
    sprintf(score, "SCORE: %d", nina.getScore());
    NF_ClearTextLayer(0, 0);
    NF_WriteText16(0, 0, 2, 2, score);
    NF_UpdateTextLayers();
}

void updateHealth()
{
    sprintf(health, "<3: %d", nina.getHealth());
    NF_ClearTextLayer(0, 1);
    NF_WriteText16(0, 1, 15, 2, health);
    NF_UpdateTextLayers();
}

void updateScoreAndHealth()
{
    NF_ClearTextLayer(0, 0);  // Clear nur einmal für beide Texte
    
    // Score und Health in Strings formatieren
    sprintf(score, "SCORE: %d", nina.getScore());
    sprintf(health, "<3: %d", nina.getHealth());
    
    // Beide Texte auf demselben Layer schreiben
    NF_WriteText16(0, 0, 2, 2, score);    // Links oben
    NF_WriteText16(0, 0, 15, 2, health);  // Rechts oben
    
    // Layer nur einmal updaten
    NF_UpdateTextLayers();
}

void resetGame()
{
    for (auto &enemy : enemies)
    {
        if (enemy.isActive())
        {
            enemy.setActive(false);
            NF_ShowSprite(0, 6 + (&enemy - &enemies[0]), false);
        }
    }
    nina.reset(128, 96);
    enemySpawnTimer = 0;
    NF_ClearTextLayer16(0, 1);
    NF_UpdateTextLayers();
    updateScoreAndHealth();
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
    // Initialize DS audio hardware
    soundEnable();
    //Load Background
    NF_LoadTiledBg("backgrounds/bg", "bg", 256, 256);
    NF_CreateTiledBg(0, 3, "bg");
    NF_LoadTiledBg("backgrounds/bg2", "bg2", 256, 256);
    NF_CreateTiledBg(1, 3, "bg2");
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
    spriteManager.initSprite("sprites/nina/Death", 4, 32, 0, 4, 4, false, 4);
    spriteManager.createSprite(0, 4, 4, 4, nina.getX(), nina.getY());
    spriteManager.hideSprite(0, 4);
    //Dying without weapon
    spriteManager.initSprite("sprites/nina/DeathWithoutWeapon", 8, 32, 0, 8, 8, false, 8);
    spriteManager.createSprite(0, 8, 8, 8, nina.getX(), nina.getY());
    spriteManager.hideSprite(0, 8);
    //Walking with Weapon on Screen2
    spriteManager.initSprite("sprites/nina/walking", 9, 32, 1, 9, 9, false, 9);
    spriteManager.createSprite(1, 9, 9, 9, nina.getX(), nina.getY());
    spriteManager.hideSprite(1, 9);

    //Load Weapon Sprites
    //Flying 
    spriteManager.initSprite("sprites/weapon/throw", 5, 32, 0, 5, 5, false, 5);
    spriteManager.createSprite(0, 5, 5, 5, nina.getWeapon().getX(), nina.getWeapon().getY());
    spriteManager.hideSprite(0, 5);
    //Load Enemies
    //Walking
    spriteManager.initSprite("sprites/Enemy/chase", 6, 32, 0, 6, 6, false, 6);
    spriteManager.createSprite(0, 6, 6, 6, 0, 0);
    spriteManager.hideSprite(0, 6);
    //Dying
    spriteManager.initSprite("sprites/Enemy/GhostDying", 7, 32, 0, 7, 7, false, 7);
    spriteManager.createSprite(0, 7, 7, 7, 0, 0);
    spriteManager.hideSprite(0, 7);
    //Load Mats
    //Crying
    //Happy

    //Score Initialization//
    NF_InitTextSys(0);
    NF_LoadTextFont16("fonts/font16", "default", 256, 256, 0);
    NF_CreateTextLayer16(0, 0, 0, "default");
    //Health Initialization
    NF_CreateTextLayer16(0, 1, 0, "default");

    //Enemy Initialization
    for (int i = 0; i < MAX_ENEMIES; ++i) {
        Enemy enemy(spriteManager);
        enemies.push_back(enemy);
    }
    const int ENEMY_SPAWN_INTERVAL = 180; // 3 seconds at 60 FPS

    std::srand(std::time(0)); // Seed for random number generation

    updateScoreAndHealth();

    //Weapon Animation parameters
    int weapon_frame = 0;
    int weapon_anim = 0;

    //finish coordinates
    int finishx = 0;
    int finishy = 0;

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
        if(gameover)
        {
            if (keysHeld() & KEY_A)
            {
                gameover = false;
                gameState = PLAYING;
                resetGame();
            }
        }

        if (keysHeld() & KEY_START)
        {
            break;
        }
        // Update weapon's sprites
        const Weapon& weapon = nina.getWeapon();
        // Update Nina's sprite
        if(gameState == PLAYING)
        {
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
            auto animationData = nina.getAnimationData();
            auto newAnimationData = spriteManager.animateSprite(animationData[0], animationData[1], 0, nina.getCurrentSpriteId(), nina.getAnimationFrames());
            nina.setAnimationData(newAnimationData[0], newAnimationData[1]);
            //ANIMTAION TEST

            // Handle weapon throwing
            if (keysDown() & KEY_TOUCH)
            {
                nina.throwWeapon(touch.px, touch.py);            
            }


            // Update weapon position
            nina.updateWeapon();
            spriteManager.flipSprite(0, nina.getCurrentSpriteId(), nina.calcDirection());
            spriteManager.moveSprite(0, nina.getCurrentSpriteId(), nina.getX(), nina.getY());
        
        
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
                        if(enemy.getInitializaionInfo())
                        {
                            enemy.firstInitialization();
                            NF_CreateSprite(0, 6 + (&enemy - &enemies[0]), 6, 6, enemy.getX(), enemy.getY());
                            NF_CreateSprite(0, 16 + (&enemy - &enemies[0]), 7, 7, enemy.getX(), enemy.getY());
                            NF_ShowSprite(0, 16 + (&enemy - &enemies[0]), false);
                        }
                        else
                        {
                            NF_ShowSprite(0, 6 + (&enemy - &enemies[0]), true);
                        }
                        break;
                    }
                }
                enemySpawnTimer = 0;
            }

            // Enemy movement and collision detection
            for (auto& enemy : enemies) {
                if (enemy.isActive() && enemy.getState() != Enemy::DYING) {
                    enemy.updateState(Enemy::CHASING, 6 + (&enemy - &enemies[0]));
                    enemy.moveTowards(nina.getX(), nina.getY());
                    spriteManager.moveSprite(0, 6 + (&enemy - &enemies[0]), enemy.getX(), enemy.getY());
                    //ANIMATE HERE
                    spriteManager.flipSprite(0, 6 + (&enemy - &enemies[0]), enemy.calcDirection());
                    auto animationData = enemy.getAnimationData();
                    auto newAnimationData = spriteManager.animateSprite(animationData[0], animationData[1], 0, 6 + (&enemy - &enemies[0]), enemy.getAnimationFrames());
                    enemy.setAnimationData(newAnimationData[0], newAnimationData[1]);
                    // Weapon collision
                    const Weapon& weapon = nina.getWeapon();
                    if (weapon.isVisible()) {
                        int dx = enemy.getX() - weapon.getX();
                        int dy = enemy.getY() - weapon.getY();
                        if (dx * dx + dy * dy < 64 && enemy.getState() != Enemy::DYING) { // Assuming 8x8 sprite, so 8*8 = 64
                            enemy.updateState(Enemy::DYING, 16 + (&enemy - &enemies[0]));
                            NF_ShowSprite(0, 6 + (&enemy - &enemies[0]), false);
                            //Increase Score
                            nina.increaseScore();
                            if(nina.getScore() >= 2)
                            {
                                gameState = WINNING;
                                finishx = nina.getX();
                                finishy = nina.getY();

                            }
                        }
                    }

                    // Nina colission
                    int dx = enemy.getX() - nina.getX();
                    int dy = enemy.getY() - nina.getY();
                    if (dx * dx + dy * dy < 64) { // Assuming 8x8 sprite, so 8*8 = 64
                        enemy.setActive(false);
                        NF_ShowSprite(0, 6 + (&enemy - &enemies[0]), false);
                        //Reduce Health
                        nina.reduceHealth();
                        if(nina.getHealth() <= 0)
                        {
                            gameState=GAMEOVER;
                        }
                    }
                }
                else if(enemy.getState() == Enemy::DYING)
                {
                    NF_ShowSprite(0, 16 + (&enemy - &enemies[0]), true);
                    auto animationData = enemy.getAnimationData();
                    auto newAnimationData = spriteManager.animateSprite(animationData[0], animationData[1], 0, 16 + (&enemy - &enemies[0]), enemy.getAnimationFrames());
                    enemy.setAnimationData(newAnimationData[0], newAnimationData[1]);
                    spriteManager.moveSprite(0, 16 + (&enemy - &enemies[0]), enemy.getX(), enemy.getY());
                    if(animationData[1] == 6)
                    {
                        enemy.updateState(Enemy::CHASING, 16 + (&enemy - &enemies[0]));
                        NF_ShowSprite(0, 16 + (&enemy - &enemies[0]), false);
                        enemy.setActive(false);
                    }

                }
            }
        }

        if (gameState == GAMEOVER)
        {
            //TODO: CHECK WHY PLAYER DISAPPEARS FOR ONE FRAME
            if(weapon.isVisible())
            {
                nina.updateState(Nina::DYING_WITHOUT_WEAPON);
            }
            else
            {
                nina.updateState(Nina::DYING);
            }
            if(!gameover)
            {
                auto animationData = nina.getAnimationData();
                auto newAnimationData = spriteManager.animateSprite(animationData[0], animationData[1], 0, nina.getCurrentSpriteId(), nina.getAnimationFrames());
                nina.setAnimationData(newAnimationData[0], newAnimationData[1]);
                spriteManager.flipSprite(0, nina.getCurrentSpriteId(), nina.calcDirection());
                            //spriteManager.moveSprite(0, 16 + (&enemy - &enemies[0]), enemy.getX(), enemy.getY());
                if(animationData[1] == 9 && !gameover)
                {
                    gameover = true;
                    //Hide Dying Sprite
                    spriteManager.hideSprite(0, nina.getCurrentSpriteId());
                    // Game Over Nachricht anzeigen
                    NF_ClearTextLayer(0,1);
                    NF_WriteText16(0, 1, 10, 10, "GAME OVER!");
                    NF_WriteText16(0, 1, 8, 12, "Press A to restart");
                    NF_UpdateTextLayers();
                    //STOP THE GAME GO TO RESTART SCREEN
                    //resetGame();
                }
            }
        }

        if(gameState == WINNING)
        {
            //MAKE ALL ENEMIES DIE
            /*for (auto& enemy : enemies) 
            {
                if (!enemy.isActive()) 
                {
                    enemy.updateState(Enemy::DYING, 16 + (&enemy - &enemies[0]));
                    NF_ShowSprite(0, 6 + (&enemy - &enemies[0]), false);
                    NF_ShowSprite(0, 16 + (&enemy - &enemies[0]), true);
                    auto animationData = enemy.getAnimationData();
                    auto newAnimationData = spriteManager.animateSprite(animationData[0], animationData[1], 0, 16 + (&enemy - &enemies[0]), enemy.getAnimationFrames());
                    enemy.setAnimationData(newAnimationData[0], newAnimationData[1]);
                    spriteManager.moveSprite(0, 16 + (&enemy - &enemies[0]), enemy.getX(), enemy.getY());
                    if(animationData[1] == 6)
                    {
                        enemy.updateState(Enemy::CHASING, 16 + (&enemy - &enemies[0]));
                        NF_ShowSprite(0, 16 + (&enemy - &enemies[0]), false);
                        enemy.setActive(false);
                    }
                }
            }*/
            //UPDATE NINA SPRITE
            if(weapon.isVisible())
            {
                nina.updateState(Nina::WALKING_WITHOUT_WEAPON);
            }
            else
            {
                nina.updateState(Nina::WALKING_WITH_WEAPON);
            }

            spriteManager.moveSprite(0, nina.getCurrentSpriteId(), finishx +2, finishy);
            //ANIMATE
            finishx = finishx + 2;
            //TODO: WHY IS IT NOT DISAPPEARING?
            if(nina.getX() >= 256)
            {
                //weapon.setVisible(false);
                spriteManager.hideSprite(0, nina.getCurrentSpriteId());
                //Spawn on second Screen
                nina.updateState(Nina::WALKING_WITH_WEAPON_SCREEN2);

            }
            //TODO: ADD WINNING LOGIC HERE
            // NINA MOVES TO THE RIGHT SIDE OF THE SCREEN DISAPPEARS AND COMES FROM THE RIGHT SIDE ON THE SECOND SCREEN
        }

        nina.setWalking(false);

        //updateScore();
        //updateHealth();
        updateScoreAndHealth();

        NF_SpriteOamSet(0);
        swiWaitForVBlank();
        oamUpdate(&oamMain);
    }
    return 0;
}