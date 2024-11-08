#include "victim.h"
#include "Managers/spriteManager.h"
#include <vector>

victim::victim(int startX, int startY, SpriteManager& manager) : 
    x(startX), y(startY), animationFrames(3),
    currentState(CRYING), spriteManager(manager), currentSpriteId(10), victimAnim(0), victimFrame(0) {}

void victim::updateState(State inputState)
{
    if (currentState != inputState) {
        currentState = inputState;
        updateSprite();
    }
}

void victim::updateSprite() {
    spriteManager.hideSprite(1, currentSpriteId);
    switch (currentState) {
    case CRYING:
        currentSpriteId = 10;
        animationFrames = 3;
        spriteManager.showSprite(1, currentSpriteId, x, y);
        break;
    case WINNING:
        currentSpriteId = 12;
        animationFrames = 3;
        spriteManager.showSprite(1, currentSpriteId, x, y);
        break;
    }
}

std::vector<int> victim::getAnimationData() const
{
    return { victimAnim, victimFrame};
}

void victim::setAnimationData(int anim, int frame)
{
    victimAnim = anim;
    victimFrame = frame;
}

int victim::getX() const { return x; }
int victim::getY() const { return y; }
int victim::getCurrentSpriteId() const { return currentSpriteId; }
int victim::getAnimationFrames() const { return animationFrames; }