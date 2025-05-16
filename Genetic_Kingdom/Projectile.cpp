// Projectile.cpp
#include "Projectile.h"
#include <cmath>

// Projectile.cpp
Projectile::Projectile(sf::Vector2f startPos, sf::Vector2f targetPos, int targetRow, int targetCol)
    : position(startPos), target(targetPos), speed(200.0f), targetRow(targetRow), targetCol(targetCol)
{
    sf::Vector2f delta = target - position;
    float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);
    direction = delta / length;

    shape.setRadius(5);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(5, 5);
    shape.setPosition(position);
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
