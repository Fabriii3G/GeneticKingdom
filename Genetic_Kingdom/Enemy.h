#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Enemy.h"
#include "DamageType.h"

class Enemy {
protected:
    sf::Vector2i position;  // posición actual en el grid
    float health;
    float speed;  // celdas por segundo (o por tick)
    float resistanceArrows;
    float resistanceMagic;
    float resistanceArtillery;
    float timeAccumulator = 0.0f;  // tiempo acumulado desde el último movimiento
    int hitCount = 0; // nuevo atributo
    bool pathBlocked = false;


    std::vector<sf::Vector2i> path; // ruta a seguir
    int pathIndex; // siguiente paso en la ruta

public:
    Enemy(sf::Vector2i startPos, float hp, float spd,
        float resArrow, float resMagic, float resArtillery);

    virtual ~Enemy() = default;

    virtual void move(float deltaTime); // avanza hacia el objetivo
    virtual void receiveDamage(DamageType type, float amount);
    bool isAlive() const;

    sf::Vector2i getPosition() const;
    void setPath(const std::vector<sf::Vector2i>& newPath);

    virtual float getFitness() const; // para el algoritmo genético
    virtual std::string getType() const = 0; // por subclase
	virtual float getHealth() const;
    virtual float getSpeed() const;
    virtual float getResistanceArrows() const;
    virtual float getResistanceMagic() const;
    virtual float getResistanceArtillery() const;

    // Método virtual puro para dibujar el enemigo en la ventana
    virtual void draw(sf::RenderWindow& window, int tileSize, sf::Vector2f offset) const = 0;


};

#endif