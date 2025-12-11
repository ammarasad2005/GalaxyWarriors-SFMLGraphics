#ifndef ASTEROID_H
#define ASTEROID_H

#include "Obstacle.h"
#include <vector>

class Asteroid : public Obstacle {
private:
    std::vector<sf::Vector2f> vertices;
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