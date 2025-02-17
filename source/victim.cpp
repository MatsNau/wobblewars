#include "victim.h"
#include "Managers/spriteManager.h"
#include <vector>

Victim::Victim(int startX, int startY, SpriteManager& manager) : 
    x(startX), y(startY), animationFrames(3),
    currentState(CRYING), spriteManager(manager), currentSpriteId(10), victimAnim(0), victimFrame(0) {}

void Victim::updateState(State inputState)
{
    if (currentState != inputState) {
        currentState = inputState;
        updateSprite();
    }
}

void Victim::updateSprite() {
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

std::vector<int> Victim::getAnimationData() const
{
    return { victimAnim, victimFrame};
}

void Victim::setAnimationData(int anim, int frame)
{
    victimAnim = anim;
    victimFrame = frame;
}

int Victim::getX() const { return x; }
int Victim::getY() const { return y; }
int Victim::getCurrentSpriteId() const { return currentSpriteId; }
int Victim::getAnimationFrames() const { return animationFrames; }