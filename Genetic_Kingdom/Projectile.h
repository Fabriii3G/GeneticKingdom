#pragma once
#include <SFML/Graphics.hpp>

class Projectile {
public:
    Projectile(sf::Vector2f start, sf::Vector2f target);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    bool hasReachedTarget() const;

private:
    sf::CircleShape shape;
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f target;
    float speed = 200.0f; // píxeles por segundo
};
