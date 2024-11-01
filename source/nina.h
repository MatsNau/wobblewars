#ifndef NINA_H
#define NINA_H

#include "weapon.h"
#include "Managers/spriteManager.h"
#include <vector>

class Nina {

public:
    enum State {
        IDLE_WITH_WEAPON = 0,
        IDLE_WITHOUT_WEAPON = 1,
        WALKING_WITH_WEAPON = 2,
        WALKING_WITHOUT_WEAPON = 3,
        DYING = 4,
        DYING_WITHOUT_WEAPON = 5,
        WALKING_WITH_WEAPON_SCREEN2 = 6,
        WINNING = 7,
        // Weitere States hier hinzuf�gen
    };

private:
    int x, y;
    int prevX;
    int health;
    int speed;
    int score;
    Weapon weapon;
    int directon;
    State currentState;
    SpriteManager& spriteManager;
    int currentSpriteId;
    bool isWalking;
    int animationFrames;
    int playerAnim;
    int playerFrame;
    bool toFlip;


public:
    Nina(int startX, int startY, SpriteManager& manager);

    void move(int direction);
    void throwWeapon(int targetX, int targetY);
    void updateWeapon();

    int getX() const;
    int getY() const;
    const Weapon& getWeapon() const;
    bool isWeaponVisible() const;

    int getScore() const;
    void increaseScore();

    int getHealth() const;
    void reduceHealth();

    void updateState(State inputState);
    void updateSprite();
    void setDirection(int inputDirection);

    int getCurrentSpriteId() const;

    void setWalking(bool walkingUpdate);
    bool getWalking() const;

    int getAnimationFrames() const;
    std::vector<int> getAnimationData() const;
    void setAnimationData(int anim, int frame);

    void reset(int startX, int startY);

    bool calcDirection();

    static const int UP = 0;
    static const int DOWN = 1;
    static const int LEFT = 2;
    static const int RIGHT = 3;
};

#endif // NINA_H