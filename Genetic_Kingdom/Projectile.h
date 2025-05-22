#pragma once
#include <SFML/Graphics.hpp>
#include "DamageType.h"

class Projectile {
public:
    Projectile(sf::Vector2f startPos, sf::Vector2f targetPos, int targetRow, int targetCol,
        DamageType type, float damage);

    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;
    bool hasReachedTarget() const;

    int getTargetRow() const { return targetRow; }
    int getTargetCol() const { return targetCol; }

    DamageType getDamageType() const { return damageType; }
    float getDamageAmount() const { return damageAmount; }

private:
    sf::Vector2f position;
    sf::Vector2f direction;
    sf::CircleShape shape;
    float speed;
    sf::Vector2f target;

    int targetRow;
    int targetCol;

    DamageType damageType;
    float damageAmount;
};
