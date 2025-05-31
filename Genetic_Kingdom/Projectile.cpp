#include "Projectile.h"
#include <cmath>
#include <SFML/Graphics.hpp>


Projectile::Projectile(sf::Vector2f startPos, sf::Vector2f targetPos, int targetRow, int targetCol,
    DamageType type, float damage)
    : position(startPos),
    target(targetPos),
    speed(200.0f),
    targetRow(targetRow),
    targetCol(targetCol),
    damageType(type),
    damageAmount(damage)
{


    sf::Vector2f delta = target - position;

    float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);
    direction = delta / length;

    shape.setRadius(3);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(5, 5);
    shape.setPosition(position);

    switch (damageType) {
    case DamageType::Arrows:
        shape.setFillColor(sf::Color::Green);
        break;
    case DamageType::Magic:
        shape.setFillColor(sf::Color::Blue);
        break;
    case DamageType::Artillery:
        shape.setFillColor(sf::Color::Red);
        break;
    case DamageType::SpecialAtk:
        shape.setFillColor(sf::Color(255, 215, 0)); 
        break;
    }
}

void Projectile::update(float deltaTime) {
    position += direction * speed * deltaTime;
    shape.setPosition(position);
}

void Projectile::draw(sf::RenderWindow& window) const {
    window.draw(shape);
}

bool Projectile::hasReachedTarget() const {
    sf::Vector2f delta = target - position;
    return std::sqrt(delta.x * delta.x + delta.y * delta.y) < 5.0f;
}
