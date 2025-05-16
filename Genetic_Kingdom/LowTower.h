#pragma once
#include "Tower.h"

class LowTower : public Tower {
public:
    LowTower();
    void update(float x, float y, int grid[25][25], int row, int col);
};
