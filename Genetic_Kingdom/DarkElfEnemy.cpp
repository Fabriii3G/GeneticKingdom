#include "DarkElfEnemy.h"
#include <iostream>

DarkElfEnemy::DarkElfEnemy(sf::Vector2i startPos, float hp)
    : Enemy(startPos, hp, 2.0f, 0.2f, 0.6f, 0.2f) // rápido, resiste magia
{
    if (!texture.loadFromFile("darkelf.png")) {
        std::cerr << "Error cargando darkelf.png\n";
    }
    sprite.setTexture(texture);
    sprite.setScale(0.5f, 0.5f);
}

void DarkElfEnemy::move() {
    Enemy::move();
}

std::string DarkElfEnemy::getType() const {
    return "Elfo Oscuro";
}

void DarkElfEnemy::draw(sf::RenderWindow& window, int tileSize, sf::Vector2f offset) const {
    sf::Sprite s = sprite;
    s.setPosition(offset.x + position.x * tileSize, offset.y + position.y * tileSize);
    window.draw(s);
}
