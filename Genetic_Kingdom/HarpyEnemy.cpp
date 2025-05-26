#include "HarpyEnemy.h"
#include <iostream>

HarpyEnemy::HarpyEnemy(sf::Vector2i startPos, float hp)
    : Enemy(startPos, hp, 1.5f, 0.6f, 0.6f, 0.0f) // solo débil a flechas (0 resistencia)
{
    if (!texture.loadFromFile("harpy.png")) {
        std::cerr << "Error cargando harpy.png\n";
    }
    sprite.setTexture(texture);
    sprite.setScale(0.5f, 0.5f);
}

void HarpyEnemy::move(float deltaTime) {
    Enemy::move(deltaTime);
}

std::string HarpyEnemy::getType() const {
    return "Harpia";
}

void HarpyEnemy::draw(sf::RenderWindow& window, int tileSize, sf::Vector2f offset) const {
    sf::Sprite s = sprite;
    s.setScale(static_cast<float>(tileSize) / s.getTexture()->getSize().x,
        static_cast<float>(tileSize) / s.getTexture()->getSize().y);

    s.setPosition(offset.x + position.x * tileSize, offset.y + position.y * tileSize);
    window.draw(s);

    // Dibuja evoluciones encima
    drawEvolutions(window, tileSize, offset);
}
