#include "nina.h"
#include "Managers/spriteManager.h"
#include <nds.h>

Nina::Nina(int startX, int startY, SpriteManager& manager) : 
    x(startX), y(startY),health(5), speed(2), score(0), weapon(x, y), isWalking(false), animationFrames(3),
    currentState(IDLE_WITH_WEAPON), spriteManager(manager), currentSpriteId(0), playerAnim(0), playerFrame(0), toFlip(false), 
    facingRight(true), prevX(startX) {}

void Nina::move(int direction) 
{
    prevX = x;
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

void Nina::reduceHealth()
{
    health--;
}

void Nina::updateState(State inputState)
{
    if (currentState != inputState) {
        currentState = inputState;
        updateSprite();
    }
}

void Nina::updateSprite() {
    spriteManager.hideSprite(0, currentSpriteId);
    switch (currentState) {
    case IDLE_WITH_WEAPON:
        currentSpriteId = 0;
        animationFrames = 3;
        spriteManager.showSprite(0, currentSpriteId, x, y);
        break;
    case IDLE_WITHOUT_WEAPON:
        currentSpriteId = 1;
        animationFrames = 3;
        spriteManager.showSprite(0, currentSpriteId, x, y);
        break;
    case WALKING_WITH_WEAPON:
        currentSpriteId = 2;
        animationFrames = 4;
        spriteManager.showSprite(0, currentSpriteId, x, y);
        break;
    case WALKING_WITHOUT_WEAPON:
        currentSpriteId = 3;
        animationFrames = 4;
        spriteManager.showSprite(0, currentSpriteId, x, y);
        break;
    case DYING:
        currentSpriteId = 4;
        spriteManager.showSprite(0, currentSpriteId, x, y);
        break;
        // Weitere Cases f�r andere States
    }
}

bool Nina::calcDirection()
{
    bool shouldFlip = false;
    if (x < prevX && facingRight)
    {
        // Wenn wir uns nach links bewegen und bisher nach rechts schauten
        facingRight = false;
        shouldFlip = true;
    }
    else if (x >= prevX && !facingRight)
    {
        facingRight = true;
        shouldFlip = true;
    }
    return shouldFlip;
}

void Nina::setWalking(bool walkingUpdate)
{
    isWalking = walkingUpdate;
}

void Nina::reset(int startX, int startY) {
    x = startX;
    y = startY;
    currentState = IDLE_WITH_WEAPON;
    currentSpriteId = 0;
    health = 5;
    score = 0;
    updateState(currentState);
}

int Nina::getScore() const { return score; }
int Nina::getHealth() const { return health; }
int Nina::getX() const { return x; }
int Nina::getY() const { return y; }
const Weapon& Nina::getWeapon() const { return weapon; }
bool Nina::isWeaponVisible() const { return weapon.isVisible(); }
int Nina::getCurrentSpriteId() const { return currentSpriteId; }
bool Nina::getWalking() const { return isWalking; }
int Nina::getAnimationFrames() const { return animationFrames; }