#ifndef NINA_H
#define NINA_H

#include "weapon.h"

class Nina {
private:
    int x, y;
    int health;
    int speed;
    int score;
    Weapon weapon;

public:
    Nina(int startX, int startY);

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

    static const int UP = 0;
    static const int DOWN = 1;
    static const int LEFT = 2;
    static const int RIGHT = 3;
};

#endif // NINA_H