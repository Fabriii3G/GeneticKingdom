#include "Tower.h"

class MidTower : public Tower {
public:
    MidTower();
    void update(float x, float y, int grid[25][25], int row, int col);
};
