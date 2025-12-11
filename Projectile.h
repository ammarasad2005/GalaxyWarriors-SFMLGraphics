#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Entity.h"

class Projectile : public Entity {
protected:
    float damage;
    float speed;
    int ownerId; // 0 = player, 1 = enemy
    float directionX;
    float directionY;
    
public:
    Projectile(float x, float y, float dmg, float spd, int owner, 
        float dirX, float dirY, sf::Color col)
        : Entity(x, y, 1.0f, 8.0f, col),
          damage(dmg),
  speed(spd),
    ownerId(owner),
          directionX(dirX),
       directionY(dirY) {
        
velocity = sf::Vector2f(directionX * speed, directionY * speed);
    }
    
    virtual ~Projectile() {}
    
    float getDamage() const { return damage; }
    int getOwnerId() const { return ownerId; }
    
    void update(float deltaTime) override {
        position += velocity * deltaTime;
        
      // Remove if off-screen
   if (position.y < -20 || position.y > 620 ||
 position.x < -20 || position.x > 820) {
     destroy();
    }
    }
  
    void onCollision(GameObject* other) override {
        destroy();
    }
};

#endif