#include "Tower.h"

class HighTower : public Tower {
public:
    HighTower();
    void update(float x, float y, int grid[25][25], int row, int col);
};
