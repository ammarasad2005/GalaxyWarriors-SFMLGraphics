#include "CollisionManager.h"
#include "PlayerBullet.h"
#include "EnemyBullet.h"
#include "PowerUp.h"
#include "Resource.h"

// STATIC MEMBER initialization
int CollisionManager::totalCollisions = 0;

bool CollisionManager::checkCollision(GameObject* a, GameObject* b) {
    if (!a || !b || ! a->isActive() || !b->isActive()) {
        return false;
    }
    
    return checkAABB(a->getBounds(), b->getBounds());
}

bool CollisionManager::checkAABB(const sf::FloatRect& a, const sf:: FloatRect& b) {
    return a.intersects(b);
}

void CollisionManager::checkAllCollisions(
    PlayerShip* player,
    DynamicArray<EnemyShip*>& enemies,
    DynamicArray<Projectile*>& projectiles,
    DynamicArray<Asteroid*>& asteroids,
    DynamicArray<Collectible*>& collectibles,
    ParticleSystem& particles
) {
    if (!player || !player->isActive()) return;
    
    // 1. Player bullets vs Enemies
    for (int i = 0; i < projectiles.getSize(); i++) {
        if (!projectiles[i]->isActive() || projectiles[i]->getOwnerId() != 0) continue;
        
        for (int j = 0; j < enemies.getSize(); j++) {
            if (!enemies[j]->isActive()) continue;
            
            if (checkCollision(projectiles[i], enemies[j])) {
                enemies[j]->takeDamage(projectiles[i]->getDamage());
                projectiles[i]->destroy();
                
                particles.createHitSpark(projectiles[i]->getPosition());
                
                if (enemies[j]->isDead()) {
                    player->addScore(enemies[j]->getScoreValue());
                    particles.createExplosion(
                        enemies[j]->getPosition(), 
                        enemies[j]->getColor(), 
                        30
                    );
                }
                
                totalCollisions++;
                break;
            }
        }
    }
    
    // 2. Player bullets vs Asteroids
    for (int i = 0; i < projectiles.getSize(); i++) {
        if (!projectiles[i]->isActive() || projectiles[i]->getOwnerId() != 0) continue;
        
        for (int j = 0; j < asteroids.getSize(); j++) {
            if (!asteroids[j]->isActive()) continue;
            
            if (checkCollision(projectiles[i], asteroids[j])) {
                asteroids[j]->takeDamage(projectiles[i]->getDamage());
                projectiles[i]->destroy();
                
                particles.createHitSpark(projectiles[i]->getPosition());
                
                if (asteroids[j]->isDead()) {
                    player->addScore(50 * (asteroids[j]->getType() + 1));
                    particles.createExplosion(
                        asteroids[j]->getPosition(),
                        sf::Color(100, 100, 100),
                        20
                    );
                }
                
                totalCollisions++;
                break;
            }
        }
    }
    
    // 3. Enemy bullets vs Player
    for (int i = 0; i < projectiles.getSize(); i++) {
        if (!projectiles[i]->isActive() || projectiles[i]->getOwnerId() != 1) continue;
        
        if (checkCollision(projectiles[i], player)) {
            player->takeDamage(projectiles[i]->getDamage());
            projectiles[i]->destroy();
            
            particles.createExplosion(
                projectiles[i]->getPosition(),
                sf::Color(255, 100, 0),
                15
            );
            
            totalCollisions++;
        }
    }
    
    // 4. Player vs Enemies
    for (int i = 0; i < enemies.getSize(); i++) {
        if (!enemies[i]->isActive()) continue;
        
        if (checkCollision(player, enemies[i])) {
            if (!player->isInvincible()) {
                player->takeDamage(20.0f);
                enemies[i]->takeDamage(50.0f);
                
                particles.createExplosion(
                    enemies[i]->getPosition(),
                    sf::Color(255, 150, 0),
                    25
                );
                
                if (enemies[i]->isDead()) {
                    player->addScore(enemies[i]->getScoreValue() / 2);
                }
                
                totalCollisions++;
            }
        }
    }
    
    // 5. Player vs Asteroids
    for (int i = 0; i < asteroids.getSize(); i++) {
        if (!asteroids[i]->isActive()) continue;
        
        if (checkCollision(player, asteroids[i])) {
            if (!player->isInvincible()) {
                player->takeDamage(15.0f);
                asteroids[i]->takeDamage(30.0f);
                
                particles.createExplosion(
                    asteroids[i]->getPosition(),
                    sf::Color(120, 120, 120),
                    20
                );
                
                totalCollisions++;
            }
        }
    }
    
    // 6. Player vs Collectibles
    for (int i = 0; i < collectibles.getSize(); i++) {
        if (!collectibles[i]->isActive() || collectibles[i]->isCollected()) continue;
        
        if (checkCollision(player, collectibles[i])) {
            collectibles[i]->apply(player);
            
            PowerUp* powerUp = dynamic_cast<PowerUp*>(collectibles[i]);
            Resource* resource = dynamic_cast<Resource*>(collectibles[i]);
            
            if (powerUp) {
                sf::Color effectColor;
                switch(powerUp->getType()) {
                    case PowerUp::SHIELD:  effectColor = sf::Color(0, 150, 255); break;
                    case PowerUp::DOUBLE_FIRE: effectColor = sf::Color(255, 50, 50); break;
                    case PowerUp::REPAIR:  effectColor = sf::Color(50, 255, 50); break;
                    case PowerUp::SPEED_BOOST: effectColor = sf::Color(255, 255, 50); break;
                }
                particles.createPowerUpCollect(collectibles[i]->getPosition(), effectColor);
            } else if (resource) {
                particles.createPowerUpCollect(
                    collectibles[i]->getPosition(),
                    sf::Color(255, 255, 100)
                );
            }
            
            totalCollisions++;
        }
    }
}