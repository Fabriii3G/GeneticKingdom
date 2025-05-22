#include "Enemy.h"
#include <iostream>
#include <cmath>
//#include "GameScreen.h"

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

void Enemy::move(float deltaTime) {
    if (pathBlocked || pathIndex >= path.size())
        return;

    timeAccumulator += deltaTime;

    
    float moveInterval = 1.0f / speed; // tiempo entre celdas

    if (timeAccumulator >= moveInterval) {
        position = path[pathIndex];
        pathIndex++;
        timeAccumulator = 0.0f;
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

    std::cout << getType() << " recibió " << actualDamage
        << " de daño. Vida restante: " << health << "\n";

    if (health <= 0.0f) {
        std::cout << getType() << " ha muerto.\n";
		//credits += 50; // Recompensa por eliminar enemigo
    }
}



bool Enemy::isAlive() const {
    return health > 0;
}

sf::Vector2i Enemy::getPosition() const {
    return position;
}

void Enemy::setPath(const std::vector<sf::Vector2i>& newPath) {
    if (newPath.empty()) {
        pathBlocked = true;
        std::cout << getType() << " quedó bloqueado sin ruta al castillo.\n";
    }
    else {
        path = newPath;
        pathIndex = 0;
        pathBlocked = false;
    }
}

float Enemy::getFitness() const {
    // Por defecto, fitness podría ser cuántas celdas recorrió
    return static_cast<float>(pathIndex);
}