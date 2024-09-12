#include "enemy.h"
#include <cmath>

Enemy::Enemy() : x(0), y(0), active(false), speed(1.5f) {}

void Enemy::spawn(int startX, int startY) {
    x = startX;
    y = startY;
    active = true;
}

void Enemy::moveTowards(int targetX, int targetY) {
    if (!active) return;

    float dx = targetX - x;
    float dy = targetY - y;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance > speed) {
        x += (dx / distance) * speed;
        y += (dy / distance) * speed;
    }
}

bool Enemy::isActive() const { return active; }
void Enemy::setActive(bool status) { active = status; }
int Enemy::getX() const { return x; }
int Enemy::getY() const { return y; }