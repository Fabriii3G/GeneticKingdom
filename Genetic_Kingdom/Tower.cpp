#include "Tower.h"
#include "DamageType.h"
#include <Iostream>
#include "Enemy.h"
const int TILE_SIZE = 32;
const int ROWS = 25;
const int COLS = 25;

const int WINDOW_WIDTH = 1300;
const int WINDOW_HEIGHT = 1000;

const int MAP_WIDTH = COLS * TILE_SIZE;   // 800 px
const int MAP_HEIGHT = ROWS * TILE_SIZE;  // 800 px

const int GRID_OFFSET_X = (WINDOW_WIDTH - MAP_WIDTH) / 2;
const int GRID_OFFSET_Y = (WINDOW_HEIGHT - MAP_HEIGHT) / 2;

Tower::Tower(int dmg, float spd, float rng, float spCd, float atkCd)
    : damage(dmg), speed(spd), range(rng), specialCooldown(spCd), attackCooldown(atkCd) {
    attackEffect.setRadius(10);
    attackEffect.setFillColor(sf::Color::Red);
    attackEffect.setOrigin(10, 10);
}

Tower::~Tower() {}

void Tower::setTexture(const sf::Texture& texture, float tileSize) {
    sprite.setTexture(texture);
    sprite.setScale(tileSize / texture.getSize().x, tileSize / texture.getSize().y);
}

void Tower::draw(sf::RenderWindow& window, float x, float y) {
    sprite.setPosition(x, y);
    window.draw(sprite);

    if (isAttacking) {
        attackEffect.setPosition(x + 16, y + 16);
        window.draw(attackEffect);
    }
}

void Tower::update(float x, float y, int grid[25][25], int row, int col, std::vector<Projectile>& projectiles, float deltaTime, int rand) {

    if (rand == 10) {
        for (int dr = -range; dr <= range; ++dr) {
            for (int dc = -range; dc <= range; ++dc) {
                int newRow = row + dr;
                int newCol = col + dc;

                if (newRow >= 0 && newRow < 25 && newCol >= 0 && newCol < 25 && grid[newRow][newCol] == 10) {
                    float enemyX = GRID_OFFSET_X + newCol * TILE_SIZE + TILE_SIZE / 2;
                    float enemyY = GRID_OFFSET_Y + newRow * TILE_SIZE + TILE_SIZE / 2;

                    // Centro de la torre
                    sf::Vector2f start(x + TILE_SIZE / 2, y + TILE_SIZE / 2);
                    sf::Vector2f target(enemyX, enemyY);

                    projectiles.emplace_back(start, target, newRow, newCol, DamageType::SpecialAtk, 30);
                    return; // Solo dispara una vez por update
                }
            }
        }
    }
    else {
        for (int dr = -range; dr <= range; ++dr) {
            for (int dc = -range; dc <= range; ++dc) {
                int newRow = row + dr;
                int newCol = col + dc;

                if (newRow >= 0 && newRow < 25 && newCol >= 0 && newCol < 25 && grid[newRow][newCol] == 10) {
                    float enemyX = GRID_OFFSET_X + newCol * TILE_SIZE + TILE_SIZE / 2;
                    float enemyY = GRID_OFFSET_Y + newRow * TILE_SIZE + TILE_SIZE / 2;

                    // Centro de la torre
                    sf::Vector2f start(x + TILE_SIZE / 2, y + TILE_SIZE / 2);
                    sf::Vector2f target(enemyX, enemyY);

                    projectiles.emplace_back(start, target, newRow, newCol, damageType, damageAmount);
                    return; // Solo dispara una vez por update
                }
            }
        }
    }
}



void Tower::upgrade() {
    if (upgradeCounter < 3) {
        damage += 2;
        range += 1;
        upgradeCounter += 1;
        std::cout << "LowTower mejorada: dano=" << damage << ", rango=" << range << " mejoras realizadas: " << upgradeCounter << "\n";
    }
}