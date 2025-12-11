#ifndef ASTEROID_H
#define ASTEROID_H

#include "Obstacle.h"
#include "DynamicArray.h"

class Asteroid : public Obstacle {
private:
    DynamicArray<sf::Vector2f> vertices;
    int asteroidType;
    sf::Color edgeColor;
    float pulseTimer;
    
public:
    Asteroid(float x, float y, int type = 1);
    ~Asteroid() override {}
    
    void render(sf::RenderWindow& window) override;
    void onCollision(GameObject* other) override;
    
    int getType() const { return asteroidType; }
    
private:
    void generateVertices();
};

#endif