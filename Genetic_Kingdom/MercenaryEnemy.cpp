#include "MercenaryEnemy.h"
#include <iostream>

MercenaryEnemy::MercenaryEnemy(sf::Vector2i startPos, float hp)
    : Enemy(startPos, hp, 1.5f, 0.5f, 0.1f, 0.5f) // resiste flechas y artillerķa
{
    if (!texture.loadFromFile("mercenary.png")) {
        std::cerr << "Error cargando mercenary.png\n";
    }
    sprite.setTexture(texture);
    sprite.setScale(0.5f, 0.5f);
}

void MercenaryEnemy::move(float deltaTime) {
    Enemy::move(deltaTime);
}

std::string MercenaryEnemy::getType() const {
    return "Mercenario";
}

void MercenaryEnemy::draw(sf::RenderWindow& window, int tileSize, sf::Vector2f offset) const {
    sf::Sprite s = sprite;
    s.setScale(static_cast<float>(tileSize) / s.getTexture()->getSize().x,
        static_cast<float>(tileSize) / s.getTexture()->getSize().y);

    s.setPosition(offset.x + position.x * tileSize, offset.y + position.y * tileSize);
    window.draw(s);

    // Dibuja evoluciones encima
    drawEvolutions(window, tileSize, offset);
}
