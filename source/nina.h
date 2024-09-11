#ifndef NINA_H
#define NINA_H

#include "weapon.h"

class Nina {
private:
    int x, y;
    int speed;
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

    static const int UP = 0;
    static const int DOWN = 1;
    static const int LEFT = 2;
    static const int RIGHT = 3;
};

#endif // NINA_H