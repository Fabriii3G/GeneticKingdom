#include "Tower.h"

class MidTower : public Tower {
public:
    int upgradeCounter = 0;
    MidTower();
    void update(float x, float y, int grid[25][25], int row, int col);
    void upgrade();
};
