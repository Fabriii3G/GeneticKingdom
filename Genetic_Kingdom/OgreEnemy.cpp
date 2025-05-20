#include "OgreEnemy.h"
#include <iostream>

OgreEnemy::OgreEnemy(sf::Vector2i startPos, float hp)
    : Enemy(startPos, hp, 1.0f, 0.3f, 0.1f, 0.2f) // resistencias: flechas, magia, artillería
{
    if (!texture.loadFromFile("ogre.png")) {
        std::cerr << "Error cargando ogre.png\n";
    }
    sprite.setTexture(texture);
    //sprite.setScale(0.5f, 0.5f); // ajustar según tamaño
}

void OgreEnemy::move(float deltaTime) {
    Enemy::move(deltaTime); // lógica de grid
}

std::string OgreEnemy::getType() const {
    return "Ogro";
}

void OgreEnemy::draw(sf::RenderWindow& window, int tileSize, sf::Vector2f offset) const {
    sf::Sprite s = sprite;
    s.setPosition(offset.x + position.x * tileSize, offset.y + position.y * tileSize);
    window.draw(s);
}