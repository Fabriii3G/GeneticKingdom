#include "Tower.h"

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

void Tower::update(float x, float y, int grid[25][25], int row, int col) {
    isAttacking = false;

    int r = static_cast<int>(range);
    for (int dr = -r; dr <= r; ++dr) {
        for (int dc = -r; dc <= r; ++dc) {
            int nr = row + dr;
            int nc = col + dc;
            if (nr >= 0 && nr < 25 && nc >= 0 && nc < 25) {
                if (grid[nr][nc] == 10) {
                    isAttacking = true;
                    return;
                }
            }
        }
    }
}
