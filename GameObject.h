#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SFML/Graphics.hpp>

// ABSTRACTION: Pure virtual base class - cannot be instantiated
// All game entities derive from this
class GameObject {
protected:
    // ENCAPSULATION: Protected members accessible to derived classes
    sf::Vector2f position;
    sf:: Vector2f velocity;
    float rotation;
    bool active;
    
public:
    // Constructor
    GameObject(float x = 0, float y = 0) 
        : position(x, y), velocity(0, 0), rotation(0), active(true) {}
    
    // VIRTUAL DESTRUCTOR: Ensures proper cleanup in derived classes
    virtual ~GameObject() {}
    
    // PURE VIRTUAL FUNCTIONS: Must be implemented by derived classes
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual sf::FloatRect getBounds() const = 0;
    
    // CONST CORRECTNESS: Getters don't modify state
    const sf::Vector2f& getPosition() const { return position; }
    const sf::Vector2f& getVelocity() const { return velocity; }
    float getRotation() const { return rotation; }
    bool isActive() const { return active; }
    
    // Setters
    void setPosition(const sf::Vector2f& pos) { position = pos; }
    void setPosition(float x, float y) { position = sf::Vector2f(x, y); }
    void setVelocity(const sf::Vector2f& vel) { velocity = vel; }
    void setRotation(float rot) { rotation = rot; }
    void setActive(bool state) { active = state; }
    
    // Virtual methods with default implementation
    virtual void destroy() { active = false; }
};

#endif