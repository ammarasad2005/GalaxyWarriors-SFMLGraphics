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
            if (!pool[index].active) {
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