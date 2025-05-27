#include "Tower.h"

class HighTower : public Tower {
public:
    int upgradeCounter = 0;
    HighTower();
    void update(float x, float y, int grid[25][25], int row, int col);
    void upgrade();
};