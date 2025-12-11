#ifndef ENTITY_H
#define ENTITY_H

#include "GameObject.h"

// INHERITANCE: Entity inherits from GameObject
// ABSTRACTION: Still abstract - adds health and collision concepts
class Entity : public GameObject {
protected:
    float health;
    float maxHealth;
    float size;
    sf::Color color;
    
public:
    Entity(float x, float y, float hp, float sz, sf::Color col)
        : GameObject(x, y), health(hp), maxHealth(hp), size(sz), color(col) {}
    
    // VIRTUAL DESTRUCTOR
    virtual ~Entity() {}
    
    // PURE VIRTUAL:  Collision response must be defined by derived classes
    virtual void onCollision(GameObject* other) = 0;
    
    // CONST CORRECTNESS
    float getHealth() const { return health; }
    float getMaxHealth() const { return maxHealth; }
    float getSize() const { return size; }
    sf::Color getColor() const { return color; }
    
    void setColor(sf::Color col) { color = col; }
    
    // VIRTUAL FUNCTION: Can be overridden
    virtual void takeDamage(float damage) {
        health -= damage;
        if (health <= 0) {
            health = 0;
            destroy();
        }
    }
    
    void heal(float amount) {
        health += amount;
        if (health > maxHealth) {
            health = maxHealth;
        }
    }
    
    bool isDead() const { return health <= 0; }
    
    // Default getBounds implementation
    sf::FloatRect getBounds() const override {
        return sf::FloatRect(
            position.x - size/2, 
            position.y - size/2, 
            size, 
            size
        );
    }
};

#endif