#ifndef ENEMY_H
#define ENEMY_H

class Enemy {
private:
    int x, y;
    bool active;
    float speed;

public:
    Enemy();
    void spawn(int startX, int startY);
    void moveTowards(int targetX, int targetY);
    bool isActive() const;
    void setActive(bool status);
    int getX() const;
    int getY() const;
};

#endif // ENEMY_H