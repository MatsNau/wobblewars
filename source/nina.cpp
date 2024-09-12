#include "nina.h"
#include <nds.h>

Nina::Nina(int startX, int startY) : x(startX), y(startY), speed(2), score(0), weapon(x, y)  {}

void Nina::move(int direction) 
{
    switch (direction) 
    {
        case UP:
            y -= speed;
            if (y < 0) y = 0;
            break;
        case DOWN:
            y += speed;
            if (y > 191) y = 191;  // Assuming screen height is 192
            break;
        case LEFT:
            x -= speed;
            if (x < 0) x = 0;
            break;
        case RIGHT:
            x += speed;
            if (x > 255) x = 255;  // Assuming screen width is 256
            break;
    }
}

void Nina::throwWeapon(int targetX, int targetY) 
{
    weapon.throw_(x, y, targetX, targetY);
}

void Nina::updateWeapon() 
{
    if (weapon.isReturning()) 
    {
        weapon.updatePosition(x, y);
    }
    else 
    {
        weapon.updatePosition();
    }
}

void Nina::increaseScore()
{
    score++;
}

int Nina::getScore() const { return score; }
int Nina::getX() const { return x; }
int Nina::getY() const { return y; }
const Weapon& Nina::getWeapon() const { return weapon; }
bool Nina::isWeaponVisible() const { return weapon.isVisible(); }