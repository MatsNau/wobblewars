#include "weapon.h"
#include <cmath>

Weapon::Weapon(int startX, int startY) : x(startX), y(startY), thrown(false), returning(false), visible(false), speed(5.0f) {}

void Weapon::throw_(int startX, int startY, int tX, int tY) {
    if (!thrown) {
        x = startX;
        y = startY;
        targetX = tX;
        targetY = tY;
        thrown = true;
        returning = false;
        visible = true;
    }
}

void Weapon::updatePosition(int ninaX, int ninaY) {
    if (!thrown || !visible) return;

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
            visible = false;
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
bool Weapon::isVisible() const { return visible; }
void Weapon::setVisible(bool vis) { visible = vis; }
int Weapon::getX() const { return x; }
int Weapon::getY() const { return y; }