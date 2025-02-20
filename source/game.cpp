#include <nds.h>
#include <nf_lib.h>
#include <ctime>
#include <vector>
#include "nina.h"
#include "enemy.h"
#include "victim.h"
#include "Managers/spriteManager.h"
#include "game.h"

void Game::PlayerIsPlaying()
{
    touchPosition touch;
    touchRead(&touch);
    // Handle Nina's movement
    if (keysHeld() & KEY_UP)
    {
        nina.move(Nina::UP);
        nina.setWalking(true);
        //u8 sound_id = NF_PlayRawSound(1, 127, 64,  false, 0);
    }
    else if (keysHeld() & KEY_DOWN)
    {
        nina.move(Nina::DOWN);
        nina.setWalking(true);
        //u8 sound_id = NF_PlayRawSound(1, 127, 64,  false, 0);
    }
    else if (keysHeld() & KEY_LEFT)
    {
        nina.move(Nina::LEFT);
        nina.setWalking(true);
        //u8 sound_id = NF_PlayRawSound(1, 127, 64,  false, 0);
    }
    else if (keysHeld() & KEY_RIGHT)
    {
        nina.move(Nina::RIGHT);
        nina.setWalking(true);
        //u8 sound_id = NF_PlayRawSound(1, 127, 64,  false, 0);
    }
    //
    //Walking sound
    /*if(nina.isWalking() && !playWalkingSound)
    {
        //u8 sound_id = NF_PlayRawSound(1, 64, true, 0);
        playWalkingSound = true;
    }
    else if(!nina.isWalking() && playWalkingSound)
    {
    }*/

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
        NF_MoveSprite(0, 5, nina.getWeapon().getX(), nina.getWeapon().getY());
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
    NF_MoveSprite(0, 5, nina.getWeapon().getX(), nina.getWeapon().getY());

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
            if (nina.getWeapon().isVisible()) {
                int dx = enemy.getX() - nina.getWeapon().getX();
                int dy = enemy.getY() - nina.getWeapon().getY();
                if (dx * dx + dy * dy < 64 && enemy.getState() != Enemy::DYING) { // Assuming 8x8 sprite, so 8*8 = 64
                    enemy.updateState(Enemy::DYING, 16 + (&enemy - &enemies[0]));
                    NF_ShowSprite(0, 6 + (&enemy - &enemies[0]), false);
                    //Increase Score
                    nina.increaseScore();
                    if(nina.getScore() >= MAX_SCORE)
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
                    gameState = GAMEOVER;
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

void Game::PlayerIsDead()
{
    if(nina.getWeapon().isVisible())
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
            NF_WriteText16(0, 1, 8, 8, "GAME OVER!\n");
            NF_WriteText16(0, 1, 8,  10, "Press A to restart");
        }
    }
}

void Game::PlayerIsWinning()
{
    if(nina.getX() >= 255 && !playerLeftScreenOne) 
    {
        playerLeftScreenOne = true;
    }    
    //UPDATE NINA SPRITE
    if(!playerLeftScreenOne)
    {
        if(nina.getWeapon().isVisible())
        {
            nina.updateState(Nina::WALKING_WITHOUT_WEAPON);
        }
        else
        {
            nina.updateState(Nina::WALKING_WITH_WEAPON);
        }
        nina.move(Nina::RIGHT);
        // Update weapon position
        nina.updateWeapon();
        if (nina.isWeaponVisible()) 
        {
            NF_ShowSprite(0, 5, true);
            NF_MoveSprite(0, 5, nina.getWeapon().getX(), nina.getWeapon().getY());
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
        NF_MoveSprite(0, 5, nina.getWeapon().getX(), nina.getWeapon().getY());
        spriteManager.flipSprite(0, nina.getCurrentSpriteId(), nina.calcDirection());
        spriteManager.moveSprite(0, nina.getCurrentSpriteId(), nina.getX(), nina.getY());
        //ANIMATION TEST
        auto animationData = nina.getAnimationData();
        auto newAnimationData = spriteManager.animateSprite(animationData[0], animationData[1], 0, nina.getCurrentSpriteId(), nina.getAnimationFrames());
        nina.setAnimationData(newAnimationData[0], newAnimationData[1]);
        //ANIMTAION TEST

    } 
    else
    {        
        if(nina.getX() >= 130)
        {
            nina.updateState(Nina::WALKING_WITH_WEAPON_SCREEN2);
            nina.move(Nina::LEFT);
        }else{
            nina.updateState(Nina::WINNING);
            victim.updateState(Victim::WINNING);
        }
        nina.updateWeapon();
        spriteManager.flipSprite(1, nina.getCurrentSpriteId(), nina.calcDirection());
        spriteManager.moveSprite(1, nina.getCurrentSpriteId(), nina.getX(), 128);
        
        NF_ClearTextLayer(0, 1);
        NF_WriteText16(0, 1, 8, 8, "GEWONNEN! \n");
        NF_WriteText16(0, 1, 8, 10, "Mats wurde gerettet!");

        //ANIMATION TEST
        auto animationData = nina.getAnimationData();
        auto newAnimationData = spriteManager.animateSprite(animationData[0], animationData[1], 1, nina.getCurrentSpriteId(), nina.getAnimationFrames());
        nina.setAnimationData(newAnimationData[0], newAnimationData[1]);
        //ANIMTAION TEST
    }   
}

void Game::PlayerInMenu()
{
    if(keysHeld() & KEY_A)
    {
        gameState = PLAYING;
        // Start background music
        u8 sound_id = NF_PlayRawSound(0, 127, 64, true, 0);
    }
}

Game::Game() : gameState(INTRO),
               spriteManager(),
               nina(128, 128, spriteManager),
               victim(110, 128, spriteManager),
               weaponSpriteId(0),
               enemySpawnTimer(0),
               gameover(false),
               playerLeftScreenOne(false),
               weapon_frame(0),
               weapon_anim(0),
               finishx(0),
               finishy(0)
{
}

int Game::Initialize()
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
    NF_LoadTiledBg("backgrounds/startscreen", "startscreen", 256, 256);
    NF_CreateTiledBg(0, 0, "startscreen");
    NF_LoadTiledBg("backgrounds/bg", "bg", 256, 256);
    NF_CreateTiledBg(0, 3, "bg");
    NF_LoadTiledBg("backgrounds/bg2", "bg2", 256, 256);
    NF_CreateTiledBg(1, 0, "bg2");
    
    //Sound//
    // Initialize audio buffers
    NF_InitRawSoundBuffers();
    // Load audio files from NitroFS
    NF_LoadRawSound("sounds/output", 0, 5512, 0);
    //NF_LoadRawSound("sounds/walking", 1, 44100, 0);

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
    //Win Screen Two
    spriteManager.initSprite("sprites/nina/Win", 11, 32, 1, 11, 11, false, 11);
    spriteManager.createSprite(1, 11, 11, 11, nina.getX(), nina.getY());
    spriteManager.hideSprite(1, 11);

    //Load Weapon Sprites
    //Flying 
    spriteManager.initSprite("sprites/weapon/throw", 5, 32, 0, 5, 5, false, 5);
    spriteManager.createSprite(0, 5, 5, 5, nina.getWeapon().getX(), nina.getWeapon().getY());
    spriteManager.hideSprite(0, 5);
    //Load Enemies
    //Walking
    spriteManager.initSprite("sprites/Enemy/chase", 6, 32, 0, 6, 6, false, 6);
    spriteManager.createSprite(0, 6, 6, 6, victim.getX(), victim.getY());
    spriteManager.hideSprite(0, 6);
    //Dying
    spriteManager.initSprite("sprites/Enemy/GhostDying", 7, 32, 0, 7, 7, false, 7);
    spriteManager.createSprite(0, 7, 7, 7, 0, 0);
    spriteManager.hideSprite(0, 7);
    
    //Load Mats
    //Crying
    spriteManager.initSprite("sprites/victim/crying", 10, 32, 1, 10, 10, false, 10);
    spriteManager.createSprite(1, 10, 10, 10, 0, 0);
    //Happy
    spriteManager.initSprite("sprites/victim/Win", 12, 32, 1, 12, 12, false, 12);
    spriteManager.createSprite(1, 12, 12, 12, 0, 0);
    spriteManager.hideSprite(1,12);

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

    std::srand(std::time(0)); // Seed for random number generation

    UpdateScoreAndHealth();

    playerLeftScreenOne = false;

    //Weapon Animation parameters
    weapon_frame = 0;
    weapon_anim = 0;

    //finish coordinates//
    finishx = 0;
    finishy = 0;
    
    return 0;
}

int Game::Run()
{
    while (1)
    {
        // TODO: Add for each state one method
        scanKeys();
        if(gameState == GAMEOVER || gameState == WINNING)
        {
            if (keysHeld() & KEY_A)
            {
                gameover = false;
                gameState = PLAYING;
                ResetGame();
            }
        }

        if (keysHeld() & KEY_START)
        {
            exit(0);
        }

        if(gameState == INTRO)
        {
            PlayerInMenu();
        }

        //Update Victim Sprite
        spriteManager.moveSprite(1, victim.getCurrentSpriteId(), victim.getX(), victim.getY());
        //ANIMATION TEST
        auto animationData = victim.getAnimationData();
        auto newAnimationData = spriteManager.animateSprite(animationData[0], animationData[1], 1, victim.getCurrentSpriteId(), victim.getAnimationFrames());
        victim.setAnimationData(newAnimationData[0], newAnimationData[1]);
        //ANIMTAION TES

        // Update Nina's sprite
        if(gameState == PLAYING)
        {
            PlayerIsPlaying();
        }

        if (gameState == GAMEOVER)
        {
            //TODO: CHECK WHY PLAYER DISAPPEARS FOR ONE FRAME
            PlayerIsDead();
        }

        if(gameState == WINNING)
        {
            PlayerIsWinning();
        }

        nina.setWalking(false);
        UpdateScoreAndHealth();

        NF_SpriteOamSet(0);
        NF_SpriteOamSet(1);
        swiWaitForVBlank();
        oamUpdate(&oamMain);
        oamUpdate(&oamSub);
    }
    return 0;
}

void Game::UpdateScore()
{
    sprintf(score, "SCORE: %d", nina.getScore());
    NF_ClearTextLayer(0, 0);
    NF_WriteText16(0, 0, 2, 2, score);
    NF_UpdateTextLayers();
}

void Game::UpdateHealth()
{
    sprintf(health, "<3: %d", nina.getHealth());
    NF_ClearTextLayer(0, 1);
    NF_WriteText16(0, 1, 15, 2, health);
    NF_UpdateTextLayers();
}

void Game::UpdateScoreAndHealth()
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

void Game::ResetGame()
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
    UpdateScoreAndHealth();
}