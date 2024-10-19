#include "enemy.h"
#include <cmath>

Enemy::Enemy(SpriteManager& manager) : x(0), y(0), prevX(x), firstinit(true), active(false), speed(1.5f), isWalking(false), animationFrames(3),
    currentState(CHASING), currentSpriteId(6), enemyAnim(0), enemyFrame(0), spriteManager(manager), toFlip(false) {}

void Enemy::spawn(int startX, int startY) {
    x = startX;
    y = startY;
    active = true;
}

void Enemy::moveTowards(int targetX, int targetY) {
    if (!active) return;

    prevX = x;

    float dx = targetX - x;
    float dy = targetY - y;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance > speed) {
        x += (dx / distance) * speed;
        y += (dy / distance) * speed;
    }
}

void Enemy::updateState(State inputState, int enemyNumber)
{
    if (currentState != inputState) {
        currentState = inputState;
        updateSprite(enemyNumber);
    }
}

void Enemy::updateSprite( int enemyNumber ) {
    spriteManager.hideSprite(0, currentSpriteId);
    switch (currentState) {
    case CHASING:
        currentSpriteId = enemyNumber;
        animationFrames = 3;
        spriteManager.showSprite(0, currentSpriteId, x, y);
        break;
    case DYING:
        currentSpriteId = enemyNumber+10;
        animationFrames = 6;
        spriteManager.showSprite(0, currentSpriteId, x, y);
        break;
    }
}

void Enemy::setAnimationData(int anim, int frame)
{
    enemyAnim = anim;
    enemyFrame = frame;
}

std::vector<int> Enemy::getAnimationData()
{
    return {enemyAnim, enemyFrame};
}

bool Enemy::calcDirection()
{
    bool shouldFlip = false;
    if (x > prevX)
    {
        shouldFlip = true;
    }
    else if (x < prevX)
    {
        shouldFlip = false;
    }
    return shouldFlip;
}



void Enemy::firstInitialization() { firstinit = false; }
bool Enemy::getInitializaionInfo() { return firstinit; }
bool Enemy::isActive() const { return active; }
void Enemy::setActive(bool status) { active = status; }
int Enemy::getX() const { return x; }
int Enemy::getY() const { return y; }
int Enemy::getAnimationFrames() {return animationFrames; }
Enemy::State Enemy::getState() {return currentState;}