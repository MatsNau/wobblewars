#include "player.h"
#include "../Managers/spriteManager.h"
#include <vector>

Player::Player(int startX, int startY, SpriteManager& manager) : 
    x(startX), y(startY),health(5), speed(2), score(0), weapon(x, y), isWalking(false), animationFrames(3),
    currentState(IDLE_WITH_WEAPON), spriteManager(manager), currentSpriteId(0), playerAnim(0), playerFrame(0), toFlip(false), 
    prevX(startX) {}

void Player::move(int direction) 
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

void Player::throwWeapon(int targetX, int targetY) 
{
    weapon.throw_(x, y, targetX, targetY);
}

void Player::updateWeapon() 
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

void Player::increaseScore()
{
    score++;
}

void Player::reduceHealth()
{
    health--;
}

void Player::updateState(State inputState)
{
    if (currentState != inputState) {
        currentState = inputState;
        updateSprite();
    }
}

void Player::updateSprite() {
    spriteManager.hideSprite(0, currentSpriteId);
    spriteManager.hideSprite(1, currentSpriteId);
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
        animationFrames = 9;
        spriteManager.showSprite(0, currentSpriteId, x, y);
        break;
    case DYING_WITHOUT_WEAPON:
        currentSpriteId = 8;
        animationFrames = 9;
        spriteManager.showSprite(0, currentSpriteId, x, y);
        break;
    case WALKING_WITH_WEAPON_SCREEN2:
        currentSpriteId = 9;
        animationFrames = 4;
        spriteManager.showSprite(1, currentSpriteId, x, y);
        break;
    case WINNING:
        currentSpriteId = 11;
        animationFrames = 3;
        spriteManager.showSprite(1, currentSpriteId, x, y);
        // Weitere Cases f�r andere States
    }
}

bool Player::calcDirection()
{
    bool shouldFlip = false;
    if (x < prevX)
    {
        shouldFlip = true;
    }
    else if (x > prevX)
    {
        shouldFlip = false;
    }
    return shouldFlip;
}

void Player::setWalking(bool walkingUpdate)
{
    isWalking = walkingUpdate;
}

std::vector<int> Player::getAnimationData() const
{
    return { playerAnim, playerFrame};
}

void Player::setAnimationData(int anim, int frame)
{
    playerAnim = anim;
    playerFrame = frame;
}

void Player::reset(int startX, int startY) {
    x = startX;
    y = startY;
    State inputState = IDLE_WITH_WEAPON;
    //spriteManager.hideSprite(0, currentSpriteId);
    health = 5;
    score = 0;
    playerAnim = 0;
    playerFrame = 0;
    updateState(currentState);
}

int Player::getScore() const { return score; }
int Player::getHealth() const { return health; }
int Player::getX() const { return x; }
int Player::getY() const { return y; }
const Weapon& Player::getWeapon() const { return weapon; }
bool Player::isWeaponVisible() const { return weapon.isVisible(); }
int Player::getCurrentSpriteId() const { return currentSpriteId; }
bool Player::getWalking() const { return isWalking; }
int Player::getAnimationFrames() const { return animationFrames; }