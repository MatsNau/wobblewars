#ifndef GAME_H
#define GAME_H

#include <nds.h>
#include <nf_lib.h>
#include <ctime>
#include <vector>
#include "nina.h"
#include "enemy.h"
#include "victim.h"
#include "Managers/spriteManager.h"

class Game {
private:
    enum GameState {
        INTRO,
        PLAYING,
        GAMEOVER,
        WINNING
    };

    GameState gameState;
    SpriteManager spriteManager;
    Nina nina;
    Victim victim;
    std::vector<Enemy> enemies;
    
    // Game variables
    static const int MAX_ENEMIES = 10;
    static const int MAX_SCORE = 50;
    static const int MAX_HP = 5;
    static const int ENEMY_SPAWN_INTERVAL = 180;
    
    char score[32];
    char health[32];
    int weaponSpriteId;
    int enemySpawnTimer;
    bool gameover;
    bool playerLeftScreenOne;
    
    // Animation parameters
    int weapon_frame;
    int weapon_anim;
    int finishx;
    int finishy;

    // Private methods for internal game logic
    void UpdateScore();
    void UpdateHealth();
    void UpdateScoreAndHealth();
    void ResetGame();
    void HandleInput();
    void UpdateGameState();
    void SpawnEnemies();
    void UpdateEnemies();
    void UpdateNina();
    void UpdateWeapon();
    void PlayerIsPlaying();
    void PlayerIsDead();
    void PlayerIsWinning();
    void PlayerInMenu();
public:
    Game();
    int Initialize();
    int Run();
};

#endif