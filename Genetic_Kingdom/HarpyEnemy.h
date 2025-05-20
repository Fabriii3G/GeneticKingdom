#ifndef HARPYENEMY_H
#define HARPYENEMY_H

#include "Enemy.h"

class HarpyEnemy : public Enemy {
private:
    sf::Sprite sprite;
    sf::Texture texture;

public:
    HarpyEnemy(sf::Vector2i startPos = { 0, 24 }, float hp = 60.0f);
    void move() override;
    std::string getType() const override;
    void draw(sf::RenderWindow& window, int tileSize, sf::Vector2f offset) const override;
};

#endif
