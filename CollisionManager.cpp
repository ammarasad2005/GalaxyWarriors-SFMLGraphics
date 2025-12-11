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
    std::vector<EnemyShip*>& enemies,
    std::vector<Projectile*>& projectiles,
    std::vector<Asteroid*>& asteroids,
    std::vector<Collectible*>& collectibles,
    ParticleSystem& particles
) {
    if (! player || !player->isActive()) return;
    
    // 1. Player bullets vs Enemies
    for (auto* projectile : projectiles) {
        if (! projectile->isActive() || projectile->getOwnerId() != 0) continue;
        
        for (auto* enemy : enemies) {
            if (!enemy->isActive()) continue;
            
            if (checkCollision(projectile, enemy)) {
                enemy->takeDamage(projectile->getDamage());
                projectile->destroy();
                
                particles.createHitSpark(projectile->getPosition());
                
                if (enemy->isDead()) {
                    player->addScore(enemy->getScoreValue());
                    particles.createExplosion(
                        enemy->getPosition(), 
                        enemy->getColor(), 
                        30
                    );
                }
                
                totalCollisions++;
                break;
            }
        }
    }
    
    // 2. Player bullets vs Asteroids
    for (auto* projectile : projectiles) {
        if (!projectile->isActive() || projectile->getOwnerId() != 0) continue;
        
        for (auto* asteroid : asteroids) {
            if (!asteroid->isActive()) continue;
            
            if (checkCollision(projectile, asteroid)) {
                asteroid->takeDamage(projectile->getDamage());
                projectile->destroy();
                
                particles.createHitSpark(projectile->getPosition());
                
                if (asteroid->isDead()) {
                    player->addScore(50 * (asteroid->getType() + 1));
                    particles. createExplosion(
                        asteroid->getPosition(),
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
    for (auto* projectile : projectiles) {
        if (!projectile->isActive() || projectile->getOwnerId() != 1) continue;
        
        if (checkCollision(projectile, player)) {
            player->takeDamage(projectile->getDamage());
            projectile->destroy();
            
            particles. createExplosion(
                projectile->getPosition(),
                sf::Color(255, 100, 0),
                15
            );
            
            totalCollisions++;
        }
    }
    
    // 4. Player vs Enemies
    for (auto* enemy :  enemies) {
        if (!enemy->isActive()) continue;
        
        if (checkCollision(player, enemy)) {
            if (!player->isInvincible()) {
                player->takeDamage(20.0f);
                enemy->takeDamage(50.0f);
                
                particles.createExplosion(
                    enemy->getPosition(),
                    sf::Color(255, 150, 0),
                    25
                );
                
                if (enemy->isDead()) {
                    player->addScore(enemy->getScoreValue() / 2);
                }
                
                totalCollisions++;
            }
        }
    }
    
    // 5. Player vs Asteroids
    for (auto* asteroid : asteroids) {
        if (!asteroid->isActive()) continue;
        
        if (checkCollision(player, asteroid)) {
            if (!player->isInvincible()) {
                player->takeDamage(15.0f);
                asteroid->takeDamage(30.0f);
                
                particles. createExplosion(
                    asteroid->getPosition(),
                    sf::Color(120, 120, 120),
                    20
                );
                
                totalCollisions++;
            }
        }
    }
    
    // 6. Player vs Collectibles
    for (auto* collectible : collectibles) {
        if (!collectible->isActive() || collectible->isCollected()) continue;
        
        if (checkCollision(player, collectible)) {
            collectible->apply(player);
            
            PowerUp* powerUp = dynamic_cast<PowerUp*>(collectible);
            Resource* resource = dynamic_cast<Resource*>(collectible);
            
            if (powerUp) {
                sf::Color effectColor;
                switch(powerUp->getType()) {
                    case PowerUp:: SHIELD:  effectColor = sf::Color(0, 150, 255); break;
                    case PowerUp::DOUBLE_FIRE: effectColor = sf::Color(255, 50, 50); break;
                    case PowerUp::REPAIR:  effectColor = sf::Color(50, 255, 50); break;
                    case PowerUp::SPEED_BOOST: effectColor = sf::Color(255, 255, 50); break;
                }
                particles.createPowerUpCollect(collectible->getPosition(), effectColor);
            } else if (resource) {
                particles.createPowerUpCollect(
                    collectible->getPosition(),
                    sf::Color(255, 255, 100)
                );
            }
            
            totalCollisions++;
        }
    }
}