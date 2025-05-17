#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

enum class DamageType {
    Arrows,
    Magic,
    Artillery
};

class Enemy {
protected:
    sf::Vector2i position;  // posici�n actual en el grid
    float health;
    float speed;  // celdas por segundo (o por tick)
    float resistanceArrows;
    float resistanceMagic;
    float resistanceArtillery;

    std::vector<sf::Vector2i> path; // ruta a seguir
    int pathIndex; // siguiente paso en la ruta

public:
    Enemy(sf::Vector2i startPos, float hp, float spd,
        float resArrow, float resMagic, float resArtillery);

    virtual ~Enemy() = default;

    virtual void move(); // avanza hacia el objetivo
    virtual void receiveDamage(DamageType type, float amount);
    bool isAlive() const;

    sf::Vector2i getPosition() const;
    void setPath(const std::vector<sf::Vector2i>& newPath);

    virtual float getFitness() const; // para el algoritmo gen�tico
    virtual std::string getType() const = 0; // por subclase

    // M�todo virtual puro para dibujar el enemigo en la ventana
    virtual void draw(sf::RenderWindow& window, int tileSize, sf::Vector2f offset) const = 0;


    // Getter/setters si necesitas m�s acceso
};

#endif