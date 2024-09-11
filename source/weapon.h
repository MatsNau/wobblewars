#ifndef WEAPON_H
#define WEAPON_H

class Weapon {
private:
    int x, y;
    int targetX, targetY;
    bool thrown;
    bool returning;
    bool visible;
    float speed;

public:
    Weapon(int startX, int startY);

    void throw_(int x, int y, int tX, int tY);
    void updatePosition(int ninaX = -1, int ninaY = -1);
    bool isReturning() const;
    bool isVisible() const;
    void setVisible(bool vis);

    int getX() const;
    int getY() const;
};

#endif // WEAPON_H