#ifndef BASICENEMY_H
#define BASICENEMY_H

#include "EnemyShip.h"
#include "Projectile.h"

class BasicEnemy : public EnemyShip {
public:
    BasicEnemy(float x, float y);
    ~BasicEnemy() override {}
    
    void updateMovementPattern(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void shoot(std::vector<Projectile*>& projectiles) override;
};

#endif