#ifndef VICTIM_H
#define VICTIM_H

#include "weapon.h"
#include "Managers/spriteManager.h"
#include <vector>

class victim{
public:
    enum State {
        CRYING = 0,
        WINNING = 1
    };

private:
    private:
    int x, y;
    State currentState;
    SpriteManager& spriteManager;
    int currentSpriteId;
    int animationFrames;
    int victimAnim;
    int victimFrame;

public:
    victim(int startX, int startY, SpriteManager& manager);

    int getX() const;
    int getY() const;

    void updateState(State inputState);
    void updateSprite();
    void setDirection(int inputDirection);

    int getCurrentSpriteId() const;

    int getAnimationFrames() const;
    std::vector<int> getAnimationData() const;
    void setAnimationData(int anim, int frame);

};


#endif // VICTIM_H