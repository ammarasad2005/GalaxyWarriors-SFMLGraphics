#ifndef SHIP_H
#define SHIP_H

#include "Entity.h"
#include <vector>

class Projectile; // Forward declaration

// INHERITANCE:  Ship inherits from Entity
// ABSTRACTION: Abstract class for all ship types
class Ship : public Entity {
protected:
    float fireRate;        // Shots per second
    float fireCooldown;    // Current cooldown
    int weaponPower;
    float speed;
    
public:
    Ship(float x, float y, float hp, float sz, sf::Color col, float rate, float spd)
        : Entity(x, y, hp, sz, col), 
          fireRate(rate), 
          fireCooldown(0), 
          weaponPower(1),
          speed(spd) {}
    
    virtual ~Ship() {}
    
    // PURE VIRTUAL: Each ship type shoots differently
    virtual void shoot(std::vector<Projectile*>& projectiles) = 0;
    
    // CONST CORRECTNESS
    float getFireRate() const { return fireRate; }
    int getWeaponPower() const { return weaponPower; }
    float getSpeed() const { return speed; }
    
    void setFireRate(float rate) { fireRate = rate; }
    void setWeaponPower(int power) { weaponPower = power; }
    void setSpeed(float spd) { speed = spd; }
    
    // Update cooldown
    void updateCooldown(float deltaTime) {
        if (fireCooldown > 0) {
            fireCooldown -= deltaTime;
        }
    }
    
    bool canShoot() const { return fireCooldown <= 0; }
    
    void resetCooldown() { fireCooldown = 1.0f / fireRate; }
};

#endif