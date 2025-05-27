#include "HighTower.h"
#include <iostream>
HighTower::HighTower()
    : Tower(25, 1.0f, 1, 5.0f, 1.5f) {
    damageType = DamageType::Artillery;
    damageAmount = 1.0f;
}

void HighTower::update(float x, float y, int grid[25][25], int row, int col) {
    isAttacking = false;
    int r = static_cast<int>(range); // En este caso, 1

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

void HighTower::upgrade() {
    if (upgradeCounter < 3) {
        damage += 2;
        range += 1;
        upgradeCounter += 1;
        std::cout << "HighTower mejorada: dano=" << damage << ", rango=" << range << " mejoras realizadas: " << upgradeCounter << "\n";
    }
}
