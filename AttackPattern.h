#ifndef ATTACKPATTERN_H
#define ATTACKPATTERN_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Projectile.h"

class EnemyShip;

// STRATEGY PATTERN: Different attack behaviors
class AttackPattern {
protected:
    float cooldown;
    float cooldownTimer;
    
public:
    AttackPattern(float cd = 2.0f) : cooldown(cd), cooldownTimer(0) {}
    virtual ~AttackPattern() {}
    
    virtual void execute(EnemyShip* shooter, std::vector<Projectile*>& projectiles) = 0;
    
    void update(float deltaTime) {
        cooldownTimer += deltaTime;
    }
    
    bool canShoot() const {
        return cooldownTimer >= cooldown;
    }
    
    void resetCooldown() {
        cooldownTimer = 0;
    }
};

// Single shot
class SingleShotPattern : public AttackPattern {
public:
    SingleShotPattern() : AttackPattern(2.0f) {}
    void execute(EnemyShip* shooter, std::vector<Projectile*>& projectiles) override;
};

// Three-way spread
class SpreadShotPattern : public AttackPattern {
private:
    float spreadAngle;
    
public:
    SpreadShotPattern(float angle = 30.0f) : AttackPattern(2.5f), spreadAngle(angle) {}
    void execute(EnemyShip* shooter, std:: vector<Projectile*>& projectiles) override;
};

// Circular burst
class CircularBurstPattern : public AttackPattern {
private:
    int bulletCount;
    
public:
    CircularBurstPattern(int count = 8) : AttackPattern(3.0f), bulletCount(count) {}
    void execute(EnemyShip* shooter, std::vector<Projectile*>& projectiles) override;
};

// Aimed shot
class AimedShotPattern : public AttackPattern {
private:
    sf::Vector2f playerPos;
    
public:
    AimedShotPattern() : AttackPattern(1.5f) {}
    void setPlayerPosition(const sf::Vector2f& pos) { playerPos = pos; }
    void execute(EnemyShip* shooter, std::vector<Projectile*>& projectiles) override;
};

// Spiral pattern
class SpiralPattern : public AttackPattern {
private:
    float angle;
    float angleIncrement;
    
public:  
    SpiralPattern() : AttackPattern(0.3f), angle(0), angleIncrement(30.0f) {}
    void execute(EnemyShip* shooter, std::vector<Projectile*>& projectiles) override;
};

// Wave pattern
class WavePattern : public AttackPattern {
private:  
    int waveSize;
    
public:
    WavePattern(int size = 5) : AttackPattern(2.0f), waveSize(size) {}
    void execute(EnemyShip* shooter, std::vector<Projectile*>& projectiles) override;
};

// Shotgun blast
class ShotgunPattern : public AttackPattern {
private:
    int pelletCount;
    
public:  
    ShotgunPattern(int count = 12) : AttackPattern(1.8f), pelletCount(count) {}
    void execute(EnemyShip* shooter, std:: vector<Projectile*>& projectiles) override;
};

// Laser beam
class LaserBeamPattern :  public AttackPattern {
private: 
    bool firing;
    float beamDuration;
    float beamTimer;
    
public:
    LaserBeamPattern() : AttackPattern(4.0f), firing(false), beamDuration(1.5f), beamTimer(0) {}
    void execute(EnemyShip* shooter, std:: vector<Projectile*>& projectiles) override;
    void update(float deltaTime);
};

#endif