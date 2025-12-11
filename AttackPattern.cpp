#include "AttackPattern.h"
#include "EnemyShip.h"
#include "EnemyBullet.h"
#include <cmath>

void SingleShotPattern::execute(EnemyShip* shooter, DynamicArray<Projectile*>& projectiles) {
    if (!shooter || !canShoot()) return;
    
    sf::Vector2f pos = shooter->getPosition();
    projectiles.pushBack(new EnemyBullet(pos.x, pos.y + shooter->getSize()/2, 0, 1));
    
    resetCooldown();
}

void SpreadShotPattern::execute(EnemyShip* shooter, DynamicArray<Projectile*>& projectiles) {
    if (!shooter || !canShoot()) return;
    
    sf::Vector2f pos = shooter->getPosition();
    
    // Center
    projectiles.pushBack(new EnemyBullet(pos.x, pos.y + shooter->getSize()/2, 0, 1));
    
    // Left
    float leftAngle = -spreadAngle * 3.14159f / 180.0f;
    projectiles.pushBack(new EnemyBullet(
        pos.x, pos.y + shooter->getSize()/2, 
        std::sin(leftAngle), std::cos(leftAngle)
    ));
    
    // Right
    float rightAngle = spreadAngle * 3.14159f / 180.0f;
    projectiles.pushBack(new EnemyBullet(
        pos.x, pos.y + shooter->getSize()/2,
        std::sin(rightAngle), std::cos(rightAngle)
    ));
    
    resetCooldown();
}

void CircularBurstPattern::execute(EnemyShip* shooter, DynamicArray<Projectile*>& projectiles) {
    if (!shooter || !canShoot()) return;
    
    sf::Vector2f pos = shooter->getPosition();
    float angleStep = 360.0f / bulletCount;
    
    for (int i = 0; i < bulletCount; i++) {
        float angle = (angleStep * i) * 3.14159f / 180.0f;
        projectiles.pushBack(new EnemyBullet(
            pos.x, pos.y,
            std::cos(angle), std::sin(angle)
        ));
    }
    
    resetCooldown();
}

void AimedShotPattern::execute(EnemyShip* shooter, DynamicArray<Projectile*>& projectiles) {
    if (!shooter || !canShoot()) return;
    
    sf::Vector2f pos = shooter->getPosition();
    sf::Vector2f direction = playerPos - pos;
    
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0) {
        direction.x /= length;
        direction.y /= length;
    }
    
    projectiles.pushBack(new EnemyBullet(
        pos.x, pos.y + shooter->getSize()/2,
        direction.x, direction.y
    ));
    
    resetCooldown();
}

void SpiralPattern::execute(EnemyShip* shooter, DynamicArray<Projectile*>& projectiles) {
    if (!shooter || !canShoot()) return;
    
    sf::Vector2f pos = shooter->getPosition();
    
    float rad = angle * 3.14159f / 180.0f;
    projectiles.pushBack(new EnemyBullet(
        pos.x, pos.y + shooter->getSize()/2,
        std::sin(rad) * 0.5f, std::cos(rad)
    ));
    
    angle += angleIncrement;
    if (angle >= 360.0f) angle -= 360.0f;
    
    resetCooldown();
}

void WavePattern::execute(EnemyShip* shooter, DynamicArray<Projectile*>& projectiles) {
    if (!shooter || !canShoot()) return;
    
    sf::Vector2f pos = shooter->getPosition();
    float spacing = 15.0f;
    float startX = pos.x - (waveSize - 1) * spacing / 2;
    
    for (int i = 0; i < waveSize; i++) {
        projectiles.pushBack(new EnemyBullet(
            startX + i * spacing,
            pos.y + shooter->getSize()/2,
            0, 1
        ));
    }
    
    resetCooldown();
}

void ShotgunPattern::execute(EnemyShip* shooter, DynamicArray<Projectile*>& projectiles) {
    if (!shooter || !canShoot()) return;
    
    sf::Vector2f pos = shooter->getPosition();
    
    for (int i = 0; i < pelletCount; i++) {
        float randomAngle = ((rand() % 60) - 30) * 3.14159f / 180.0f;
        float randomSpeed = 0.8f + (rand() % 40) * 0.01f;
        
        projectiles.pushBack(new EnemyBullet(
            pos.x + (rand() % 20) - 10,
            pos.y + shooter->getSize()/2,
            std::sin(randomAngle) * randomSpeed,
            std::cos(randomAngle) * randomSpeed
        ));
    }
    
    resetCooldown();
}

void LaserBeamPattern::execute(EnemyShip* shooter, DynamicArray<Projectile*>& projectiles) {
    if (!shooter) return;
    
    if (!firing && canShoot()) {
        firing = true;
        beamTimer = 0;
        resetCooldown();
    }
    
    if (firing) {
        sf::Vector2f pos = shooter->getPosition();
        projectiles.pushBack(new EnemyBullet(pos.x, pos.y + shooter->getSize()/2, 0, 1));
    }
}

void LaserBeamPattern::update(float deltaTime) {
    AttackPattern::update(deltaTime);
    
    if (firing) {
        beamTimer += deltaTime;
        if (beamTimer >= beamDuration) {
            firing = false;
            beamTimer = 0;
        }
    }
}