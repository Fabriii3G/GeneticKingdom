#include "OgreEnemy.h"
#include <iostream>

OgreEnemy::OgreEnemy(sf::Vector2i startPos, float hp)
    : Enemy(startPos, hp, 1.0f, 0.3f, 0.1f, 0.2f) // resistencia ejemplo: flechas, magia, artillería
{
    if (!texture.loadFromFile("ogre.png")) {
        std::cerr << "Error cargando ogre.png\n";
    }
    sprite.setTexture(texture);
    sprite.setScale(0.5f, 0.5f); // ajusta según tamaño real de sprite
}

void OgreEnemy::move() {
    Enemy::move(); // lógica de movimiento

    // Actualiza posición del sprite visual según grid
    sprite.setPosition(
        static_cast<float>(position.x * 32), // tileSize puede venir por parámetro
        static_cast<float>(position.y * 32)
    );
}

std::string OgreEnemy::getType() const {
    return "Ogro";
}

void OgreEnemy::draw(sf::RenderWindow& window, int tileSize, sf::Vector2f offset) const {
    sf::Sprite s = sprite;
    s.setPosition(offset.x + position.x * tileSize, offset.y + position.y * tileSize);
    window.draw(s);
}
