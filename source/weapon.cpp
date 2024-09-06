#include "weapon.h"
#include <cmath>

Weapon::Weapon(int startX, int startY) : x(startX), y(startY), thrown(false), returning(false), speed(3.0f) {}

void Weapon::throw_(int tX, int tY) {
    if (!thrown) {
        targetX = tX;
        targetY = tY;
        thrown = true;
        returning = false;
    }
}

void Weapon::updatePosition(int ninaX, int ninaY) {
    if (!thrown) return;

    int targetX = returning ? ninaX : this->targetX;
    int targetY = returning ? ninaY : this->targetY;

    float dx = targetX - x;
    float dy = targetY - y;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance < speed) {
        x = targetX;
        y = targetY;
        if (returning) {
            thrown = false;
            returning = false;
        }
        else {
            returning = true;
        }
    }
    else {
        x += (dx / distance) * speed;
        y += (dy / distance) * speed;
    }
}

bool Weapon::isReturning() const { return returning; }
int Weapon::getX() const { return x; }
int Weapon::getY() const { return y; }