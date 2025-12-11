#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H

#include "GameObject.h"

class PlayerShip;

// ABSTRACTION: Pure virtual collectible base
class Collectible : public GameObject {
protected:
    float lifeTime;
    float maxLifeTime;
    bool collected;
    float pulseTimer;
    float floatOffset;
    
public:
    Collectible(float x, float y, float life)
        : GameObject(x, y), 
          lifeTime(life), 
          maxLifeTime(life),
          collected(false),
          pulseTimer(0),
          floatOffset(0) {
        velocity = sf::Vector2f(0, 30.0f);
    }
    
    virtual ~Collectible() {}
    
    // PURE VIRTUAL:  Effect varies by collectible type
    virtual void apply(PlayerShip* player) = 0;
    
    bool isCollected() const { return collected; }
    
    void update(float deltaTime) override {
        if (collected) {
            destroy();
            return;
        }
        
        lifeTime -= deltaTime;
        if (lifeTime <= 0) {
            destroy();
            return;
        }
        
        position += velocity * deltaTime;
        pulseTimer += deltaTime;
        floatOffset = std::sin(pulseTimer * 3.0f) * 5.0f;
        
        if (position.y > 650) {
            destroy();
        }
    }
    
    sf::FloatRect getBounds() const override {
        float size = 20;
        return sf::FloatRect(position.x - size/2, position.y - size/2, size, size);
    }
    
    void collect() { collected = true; }
};

#endif