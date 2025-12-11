#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "GameObject.h"
#include "PlayerShip.h"
#include "EnemyShip.h"
#include "Projectile.h"
#include "Asteroid.h"
#include "Collectible.h"
#include "ParticleSystem.h"
#include <vector>

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
        std::vector<EnemyShip*>& enemies,
        std::vector<Projectile*>& projectiles,
        std::vector<Asteroid*>& asteroids,
        std::vector<Collectible*>& collectibles,
        ParticleSystem& particles
    );
    
    static int getTotalCollisions() { return totalCollisions; }
    static void resetCollisions() { totalCollisions = 0; }
};

inline bool checkAABB(const sf::FloatRect& a, const sf::FloatRect& b) {
    return a.intersects(b);
}

#endif