#pragma once
#include <SFML/Graphics.hpp>


class Projectile {
public:
    Projectile(sf::Vector2f startPos, sf::Vector2f targetPos, int targetRow, int targetCol);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;
    bool hasReachedTarget() const;

    int getTargetRow() const { return targetRow; }
    int getTargetCol() const { return targetCol; }

private:
    sf::Vector2f position;
    sf::Vector2f direction;
    sf::CircleShape shape;
    float speed;
    sf::Vector2f target;

    int targetRow;
    int targetCol;
};

