#pragma once
#include <SFML/Graphics.hpp>
#include "Projectile.h"
class Tower {
protected:
    int damage;
    DamageType damageType;
    float damageAmount;
    float speed;
    float range;
    float specialCooldown;
    float attackCooldown;

    sf::Sprite sprite;
    sf::CircleShape attackEffect;  // Visual para el ataque
    bool isAttacking = false;
    float attackTimer = 0.f;

public:
    Tower(int dmg, float spd, float rng, float spCd, float atkCd);
    virtual ~Tower();

    void draw(sf::RenderWindow& window, float x, float y);
    void setTexture(const sf::Texture& texture, float tileSize);
    virtual void update(float x, float y, int grid[25][25], int row, int col, std::vector<Projectile>& projectiles);

    float getRange() const { return range; }

    void setDamage(DamageType type, float amount) {
        damageType = type;
        damageAmount = amount;
    }

    DamageType getDamageType() const { return damageType; }
    float getDamageAmount() const { return damageAmount; }

};
