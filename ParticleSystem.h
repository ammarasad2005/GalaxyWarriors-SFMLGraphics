#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "Particle.h"
#include "DynamicArray.h"
#include <cstdlib>

// TEMPLATE (BONUS): Object pool pattern
template<typename T>
class ObjectPool {
private:
    DynamicArray<T> pool;
    size_t nextAvailable;
    
public:
    ObjectPool(size_t size) : nextAvailable(0) {
        for (size_t i = 0; i < size; i++) {
            pool.pushBack(T());
        }
    }
    
    T* getNext() {
        for (size_t i = 0; i < pool.getSize(); i++) {
            size_t index = (nextAvailable + i) % pool.getSize();
            if (!pool[index].active) {
                nextAvailable = (index + 1) % pool.getSize();
                return &pool[index];
            }
        }
        return nullptr;
    }
    
    DynamicArray<T>& getAll() { return pool; }
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