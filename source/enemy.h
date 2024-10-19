#ifndef ENEMY_H
#define ENEMY_H

#include "Managers/spriteManager.h"
#include <vector>

class Enemy {
public:
        enum State {
        CHASING = 0,
        DYING = 1,
    };
private:
    int x, y;
    bool active;
    float speed;
    bool firstinit;
    State currentState;
    SpriteManager& spriteManager;
    int currentSpriteId;
    bool isWalking;
    int animationFrames;
    int enemyAnim;
    int enemyFrame;

public:
    Enemy(SpriteManager& manager);

    void firstInitialization();
    bool getInitializaionInfo(); 
    void spawn(int startX, int startY);
    void moveTowards(int targetX, int targetY);
    bool isActive() const;
    void setActive(bool status);
    int getX() const;
    int getY() const;
    void updateState(State inputState, int enemyNumber);
    void updateSprite(int enemyNumber);
    void setAnimationData(int anim, int frame);
    std::vector<int> getAnimationData();
    int getAnimationFrames();
    //void setSpriteManager(SpriteManager& manager);
};

#endif // ENEMY_H