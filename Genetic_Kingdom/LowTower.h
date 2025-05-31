#pragma once
#include "Tower.h"

class LowTower : public Tower {
public:
    int attackCount = 0;
    int upgradeCounter = 0;
    LowTower();
    void update(float x, float y, int grid[25][25], int row, int col);
    void upgrade();
};
