#ifndef PATTERNENEMY_H
#define PATTERNENEMY_H

#include "EnemyShip.h"

class PatternEnemy : public EnemyShip {
private:
    float time;
    int patternType;
    
public:
    PatternEnemy(float x, float y);
    ~PatternEnemy() override {}
    
    void updateMovementPattern(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void shoot(std:: vector<Projectile*>& projectiles) override;
};

#endif