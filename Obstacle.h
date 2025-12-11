#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "Entity.h"

// ABSTRACTION: Base obstacle class
class Obstacle : public Entity {
protected:
    float rotationSpeed;
    
public:
    Obstacle(float x, float y, float hp, float sz, sf::Color col, float rotSpeed)
        : Entity(x, y, hp, sz, col), rotationSpeed(rotSpeed) {}
    
    virtual ~Obstacle() {}
    
    void update(float deltaTime) override {
        position += velocity * deltaTime;
        rotation += rotationSpeed * deltaTime;
        
        // Remove if off-screen
        if (position. y > 650) {
            destroy();
        }
    }
};

#endif