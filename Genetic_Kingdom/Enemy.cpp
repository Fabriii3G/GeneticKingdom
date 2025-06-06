﻿#include "Enemy.h"
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

    std::cout << getType() << " recibi� " << actualDamage
        << " de dano. Vida restante: " << health << "\n";

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
        std::cout << getType() << " qued� bloqueado sin ruta al castillo.\n";
    }
    else {
        path = newPath;
        pathIndex = 0;
        pathBlocked = false;
    }
}

float Enemy::getFitness() const {
    // Por defecto, fitness podr�a ser cu�ntas celdas recorri�
    return static_cast<float>(pathIndex);
}

float Enemy::getHealth() const {
    return health;
}

float Enemy::getSpeed() const {
    return speed;
}

float Enemy::getResistanceArrows() const {
    return resistanceArrows;
}

float Enemy::getResistanceMagic() const {
    return resistanceMagic;
}

float Enemy::getResistanceArtillery() const {
    return resistanceArtillery;
}

void Enemy::addEvolution(const EnemyEvolution& evo) {
    // Aplica la evoluci�n a los stats
    evo.applyTo(health, speed, resistanceArrows, resistanceMagic, resistanceArtillery);
    evolutions.push_back(evo);
}

void Enemy::drawEvolutions(sf::RenderWindow& window, int tileSize, sf::Vector2f offset) const {
    for (const auto& evo : evolutions) {
        sf::Sprite overlay = evo.getOverlaySprite();
        overlay.setScale(static_cast<float>(tileSize) / overlay.getTexture()->getSize().x,
            static_cast<float>(tileSize) / overlay.getTexture()->getSize().y);
        overlay.setPosition(offset.x + position.x * tileSize, offset.y + position.y * tileSize);
        window.draw(overlay);
    }
}

void Enemy::kill() {
    health = 0;
    killedByPlayer = false;
}