#include "Enemy.h"
#include <iostream>
#include <cmath>

Enemy::Enemy(sf::Vector2i startPos, float hp, float spd,
    float resArrow, float resMagic, float resArtillery)
    : position(startPos),
    health(hp),
    speed(spd),
    resistanceArrows(resArrow),
    resistanceMagic(resMagic),
    resistanceArtillery(resArtillery),
    pathIndex(0) {
}

void Enemy::move() {
    if (pathIndex < path.size()) {
        position = path[pathIndex];
        pathIndex++;
    }
}

void Enemy::receiveDamage(DamageType type, float amount) {
    float actualDamage = amount;
    switch (type) {
    case DamageType::Arrows:
        actualDamage *= (1.0f - resistanceArrows);
        break;
    case DamageType::Magic:
        actualDamage *= (1.0f - resistanceMagic);
        break;
    case DamageType::Artillery:
        actualDamage *= (1.0f - resistanceArtillery);
        break;
    }

    health -= actualDamage;
    std::cout << getType() << " recibió " << actualDamage << " de daño. Vida restante: " << health << "\n";
}

bool Enemy::isAlive() const {
    return health > 0;
}

sf::Vector2i Enemy::getPosition() const {
    return position;
}

void Enemy::setPath(const std::vector<sf::Vector2i>& newPath) {
    path = newPath;
    pathIndex = 0;
}

float Enemy::getFitness() const {
    // Por defecto, fitness podría ser cuántas celdas recorrió
    return static_cast<float>(pathIndex);
}
