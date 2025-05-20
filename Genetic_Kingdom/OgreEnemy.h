#ifndef OGREENEMY_H
#define OGREENEMY_H

#include "Enemy.h" // Asegúrate de incluir correctamente la clase base
#include <SFML/Graphics.hpp>

class OgreEnemy : public Enemy {
private:
    sf::Sprite sprite;
    sf::Texture texture;

public:
    OgreEnemy(sf::Vector2i startPos = { 0, 0 }, float hp = 100.0f);

    // La firma debe coincidir exactamente con la de la clase base
    void move(float deltaTime) override;
    std::string getType() const override;
    void draw(sf::RenderWindow& window, int tileSize, sf::Vector2f offset) const override;
};

#endif
