#ifndef PARTICLE_H
#define PARTICLE_H

#include <SFML/Graphics.hpp>

struct Particle {
    sf::Vector2f position;
    sf:: Vector2f velocity;
    sf::Color color;
    float lifeTime;
    float maxLifeTime;
    float size;
    bool active;
    
    Particle() : lifeTime(0), maxLifeTime(1.0f), size(3), active(false) {}
    
    void reset(sf::Vector2f pos, sf:: Vector2f vel, sf::Color col, float life, float sz) {
        position = pos;
        velocity = vel;
        color = col;
        lifeTime = life;
        maxLifeTime = life;
        size = sz;
        active = true;
    }
    
    void update(float deltaTime) {
        if (!active) return;
        
        position += velocity * deltaTime;
        lifeTime -= deltaTime;
        
        // Fade out
        float alpha = (lifeTime / maxLifeTime) * 255;
        color.a = static_cast<sf::Uint8>(alpha);
        
        // Slow down
        velocity *= 0.98f;
        
        if (lifeTime <= 0) {
            active = false;
        }
    }
    
    void render(sf::RenderWindow& window) {
        if (!active) return;
        
        sf::CircleShape circle(size);
        circle.setPosition(position. x - size, position.y - size);
        circle.setFillColor(color);
        window.draw(circle);
    }
};

#endif