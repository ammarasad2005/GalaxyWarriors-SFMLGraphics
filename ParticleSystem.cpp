#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "Particle.h"
#include <vector>
#include <cstdlib>

// TEMPLATE (BONUS): Object pool pattern
template<typename T>
class ObjectPool {
private:
    std::vector<T> pool;
    size_t nextAvailable;
    
public:
    ObjectPool(size_t size) : nextAvailable(0) {
        pool.resize(size);
    }
    
    T* getNext() {
        for (size_t i = 0; i < pool.size(); i++) {
            size_t index = (nextAvailable + i) % pool.size();
            if (! pool[index].active) {
                nextAvailable = (index + 1) % pool.size();
                return &pool[index];
            }
        }
        return nullptr;
    }
    
    std::vector<T>& getAll() { return pool; }
};

class ParticleSystem {
private: 
    ObjectPool<Particle> particlePool;
    
    // STATIC MEMBER
    static int totalParticlesCreated;
    
public: 
    ParticleSystem(size_t poolSize = 500);
    ~ParticleSystem() {}
    
    // Particle effects
    void createExplosion(sf::Vector2f position, sf::Color color, int count = 20);
    void createTrail(sf::Vector2f position, sf::Color color);
    void createPowerUpCollect(sf::Vector2f position, sf::Color color);
    void createHitSpark(sf::Vector2f position);
    void createShockwave(sf::Vector2f position, sf::Color color, float radius = 50.0f);
    void createSpiralExplosion(sf::Vector2f position, sf::Color color);
    void createNebula(sf::Vector2f position, sf::Color color);
    void createWarpEffect(sf::Vector2f position, sf::Color color);
    
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    // STATIC METHOD
    static int getTotalParticlesCreated() { return totalParticlesCreated; }
};

#endif
#include "ParticleSystem.h"
#include <cmath>

// STATIC MEMBER initialization
int ParticleSystem::totalParticlesCreated = 0;

ParticleSystem::ParticleSystem(size_t poolSize) 
    : particlePool(poolSize) {
}

void ParticleSystem::createExplosion(sf::Vector2f position, sf::Color color, int count) {
    for (int i = 0; i < count; i++) {
        Particle* p = particlePool.getNext();
   if (!p) break;
        
 float angle = (rand() % 360) * 3.14159f / 180.0f;
        float speed = 100.0f + (rand() % 200);
        
     sf::Vector2f velocity(
     std::cos(angle) * speed,
        std::sin(angle) * speed
  );
        
        sf::Color particleColor = color;
particleColor.r = std::min(255, color.r + (rand() % 50) - 25);
        particleColor.g = std::min(255, color.g + (rand() % 50) - 25);
   particleColor.b = std::min(255, color.b + (rand() % 50) - 25);
        
 float size = 2.0f + (rand() % 4);
        float life = 0.5f + (rand() % 10) * 0.1f;
        
  p->reset(position, velocity, particleColor, life, size);
        totalParticlesCreated++;
    }
}

void ParticleSystem::createTrail(sf::Vector2f position, sf::Color color) {
    Particle* p = particlePool.getNext();
    if (!p) return;
    
    sf::Vector2f velocity(
        (rand() % 20) - 10,
        (rand() % 20) - 10
    );
  
    color.a = 200;
    p->reset(position, velocity, color, 0.3f, 3.0f);
    totalParticlesCreated++;
}

void ParticleSystem::createPowerUpCollect(sf::Vector2f position, sf::Color color) {
    for (int i = 0; i < 15; i++) {
 Particle* p = particlePool.getNext();
 if (!p) break;
        
  float angle = (360.0f / 15.0f * i) * 3.14159f / 180.0f;
   float speed = 150.0f;
        
        sf::Vector2f velocity(
            std::cos(angle) * speed,
            std::sin(angle) * speed
  );
        
        p->reset(position, velocity, color, 0.8f, 4.0f);
 totalParticlesCreated++;
    }
}

void ParticleSystem::createHitSpark(sf::Vector2f position) {
    for (int i = 0; i < 8; i++) {
     Particle* p = particlePool.getNext();
   if (!p) break;
     
        float angle = (rand() % 360) * 3.14159f / 180.0f;
        float speed = 200.0f;
        
        sf::Vector2f velocity(
   std::cos(angle) * speed,
            std::sin(angle) * speed
  );
        
    sf::Color sparkColor = sf::Color::White;
     
        p->reset(position, velocity, sparkColor, 0.4f, 2.0f);
        totalParticlesCreated++;
    }
}

void ParticleSystem::createShockwave(sf::Vector2f position, sf::Color color, float radius) {
    int particleCount = 30;
    for (int i = 0; i < particleCount; i++) {
        Particle* p = particlePool.getNext();
        if (!p) break;
  
        float angle = (360.0f / particleCount * i) * 3.14159f / 180.0f;
     float speed = 200.0f + (rand() % 100);
        
        sf::Vector2f velocity(
            std::cos(angle) * speed,
        std::sin(angle) * speed
        );
        
        p->reset(position, velocity, color, 0.8f, 5.0f);
        totalParticlesCreated++;
    }
}

void ParticleSystem::createSpiralExplosion(sf::Vector2f position, sf::Color color) {
    int spirals = 3;
    int particlesPerSpiral = 15;
    
    for (int s = 0; s < spirals; s++) {
        float spiralOffset = (360.0f / spirals * s) * 3.14159f / 180.0f;
        
        for (int i = 0; i < particlesPerSpiral; i++) {
        Particle* p = particlePool.getNext();
            if (!p) break;
 
            float angle = spiralOffset + (i * 0.4f);
   float speed = 50.0f + i * 15.0f;
       
   sf::Vector2f velocity(
            std::cos(angle) * speed,
    std::sin(angle) * speed
            );
        
            sf::Color particleColor = color;
  particleColor.r = std::min(255, color.r + (rand() % 50) - 25);
     particleColor.g = std::min(255, color.g + (rand() % 50) - 25);
 
            p->reset(position, velocity, particleColor, 1.2f, 4.0f);
            totalParticlesCreated++;
 }
    }
}

void ParticleSystem::createNebula(sf::Vector2f position, sf::Color color) {
    for (int i = 0; i < 40; i++) {
        Particle* p = particlePool.getNext();
        if (!p) break;
        
        float angle = (rand() % 360) * 3.14159f / 180.0f;
        float speed = 20.0f + (rand() % 40);
        
        sf::Vector2f velocity(
      std::cos(angle) * speed,
            std::sin(angle) * speed
    );
  
        sf::Color nebulaColor = color;
        nebulaColor.a = 100 + (rand() % 100);

  p->reset(position, velocity, nebulaColor, 2.0f, 8.0f + (rand() % 10));
        totalParticlesCreated++;
    }
}

void ParticleSystem::createWarpEffect(sf::Vector2f position, sf::Color color) {
    for (int i = 0; i < 20; i++) {
        Particle* p = particlePool.getNext();
    if (!p) break;
        
        float angle = (rand() % 360) * 3.14159f / 180.0f;
    float speed = 300.0f + (rand() % 200);
        
        sf::Vector2f velocity(
      std::cos(angle) * speed,
   std::sin(angle) * speed
        );
   
        p->reset(position, velocity, color, 0.5f, 2.0f);
        totalParticlesCreated++;
    }
}

void ParticleSystem::update(float deltaTime) {
 for (Particle& p : particlePool.getAll()) {
        if (p.active) {
        p.update(deltaTime);
        }
    }
}

void ParticleSystem::render(sf::RenderWindow& window) {
    for (Particle& p : particlePool.getAll()) {
      if (p.active) {
    p.render(window);
        }
 }
}