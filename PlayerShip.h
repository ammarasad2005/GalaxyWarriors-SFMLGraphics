#ifndef PLAYERSHIP_H
#define PLAYERSHIP_H

#include "Ship.h"
#include "Projectile.h"

// Forward declaration for friend class
class GameEngine;

// MULTILEVEL INHERITANCE: GameObject -> Entity -> Ship -> PlayerShip
class PlayerShip : public Ship {
private: 
    int score;
    int lives;
    bool shieldActive;
    float shieldDuration;
    bool doubleFireActive;
    float doubleFireDuration;
    float invincibilityTimer;
    
    // FRIEND CLASS: GameEngine can access private members
    friend class GameEngine;
    
public:
    PlayerShip(float x, float y);
    
    // VIRTUAL DESTRUCTOR
    ~PlayerShip() override {}
    
    // POLYMORPHISM: Override pure virtual functions
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void shoot(std::vector<Projectile*>& projectiles) override;
    void onCollision(GameObject* other) override;
    
    // Player-specific methods
    void handleInput(float deltaTime);
    void addScore(int points);
    void activateShield(float duration);
    void activateDoubleFire(float duration);
    void loseLife();
    
    // CONST CORRECTNESS
    int getScore() const { return score; }
    int getLives() const { return lives; }
    bool hasShield() const { return shieldActive; }
    bool hasDoubleFire() const { return doubleFireActive; }
    bool isInvincible() const { return invincibilityTimer > 0; }
    
    // FUNCTION OVERLOADING: Multiple takeDamage signatures
    void takeDamage(float damage) override;
    void takeDamage(float damage, bool ignoreShield);
};

#endif