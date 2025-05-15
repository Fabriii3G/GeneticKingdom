#include "Projectile.h"
#include <cmath>

Projectile::Projectile(sf::Vector2f start, sf::Vector2f targetPos)
    : position(start), target(targetPos) {

    shape.setRadius(5);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(5, 5);
    shape.setPosition(position);

    sf::Vector2f direction = target - start;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0)
        velocity = direction / length * speed;
    else
        velocity = { 0.f, 0.f };
}

void Projectile::update(float deltaTime) {
    position += velocity * deltaTime;
    shape.setPosition(position);
}

void Projectile::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

bool Projectile::hasReachedTarget() const {
    float dx = position.x - target.x;
    float dy = position.y - target.y;
    return std::sqrt(dx * dx + dy * dy) < 10.f;
}
