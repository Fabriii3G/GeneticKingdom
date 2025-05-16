#include "MidTower.h"

MidTower::MidTower()
    : Tower(15, 1.0f, 2, 5.0f, 1.5f) {}

void MidTower::update(float x, float y, int grid[25][25], int row, int col) {
    isAttacking = false;
    int r = static_cast<int>(range); // En este caso, 2

    for (int dr = -r; dr <= r; ++dr) {
        for (int dc = -r; dc <= r; ++dc) {
            int nr = row + dr;
            int nc = col + dc;

            if (nr >= 0 && nr < 25 && nc >= 0 && nc < 25) {
                if (grid[nr][nc] == 10) {  // Valor que representa enemigo
                    isAttacking = true;
                    return;
                }
            }
        }
    }
}