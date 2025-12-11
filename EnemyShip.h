#ifndef ENEMYSHIP_H
#define ENEMYSHIP_H

#include "Ship.h"

// ABSTRACTION: Abstract enemy class
class EnemyShip : public Ship {
protected:
    int scoreValue;
    float shootTimer;
    float shootInterval;
    
public:
    EnemyShip(float x, float y, float hp, float sz, sf::Color col, 
              float rate, float spd, int score)
        : Ship(x, y, hp, sz, col, rate, spd),
          scoreValue(score),
          shootTimer(0),
          shootInterval(2.0f) {}
    
    virtual ~EnemyShip() {}
    
    // PURE VIRTUAL:  Movement pattern varies by enemy type
    virtual void updateMovementPattern(float deltaTime) = 0;
    
    int getScoreValue() const { return scoreValue; }
    
    void update(float deltaTime) override {
        if (!active) return;
        
        updateMovementPattern(deltaTime);
        position += velocity * deltaTime;
        updateCooldown(deltaTime);
        
        shootTimer += deltaTime;
        
        // Remove if off-screen (bottom)
        if (position.y > 650) {
            destroy();
        }
    }
    
    void onCollision(GameObject* other) override {
        // Basic collision
    }
    
    bool shouldShoot() {
        if (shootTimer >= shootInterval) {
            shootTimer = 0;
            return true;
        }
        return false;
    }
};

#endif