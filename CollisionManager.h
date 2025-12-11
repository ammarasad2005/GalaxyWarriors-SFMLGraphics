#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "GameObject.h"
#include "PlayerShip.h"
#include "EnemyShip.h"
#include "Projectile.h"
#include "Asteroid.h"
#include "Collectible.h"
#include "ParticleSystem.h"
#include "DynamicArray.h"

// STATIC CLASS:  Collision detection
class CollisionManager {
private:
    // STATIC MEMBER
    static int totalCollisions;
    
    friend bool checkAABB(const sf::FloatRect& a, const sf::FloatRect& b);
    
public:
    // STATIC METHODS
    static bool checkCollision(GameObject* a, GameObject* b);
    static bool checkAABB(const sf::FloatRect& a, const sf::FloatRect& b);
    
    static void checkAllCollisions(
        PlayerShip* player,
        DynamicArray<EnemyShip*>& enemies,
        DynamicArray<Projectile*>& projectiles,
        DynamicArray<Asteroid*>& asteroids,
        DynamicArray<Collectible*>& collectibles,
        ParticleSystem& particles
    );
    
    static int getTotalCollisions() { return totalCollisions; }
    static void resetCollisions() { totalCollisions = 0; }
};

inline bool checkAABB(const sf::FloatRect& a, const sf::FloatRect& b) {
    return a.intersects(b);
}

#endif