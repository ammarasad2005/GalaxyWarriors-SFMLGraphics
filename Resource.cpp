#include "Resource.h"
#include "ColorPalette.h"
#include <cmath>

Resource::Resource(float x, float y, Type t)
    : Collectible(x, y, 10.0f),
      type(t),
      value(0) {
    
    velocity = sf::Vector2f(0, 50.0f);
    
    switch(type) {
        case ENERGY:
            value = 50;
            primaryColor = sf::Color(255, 255, 0);
            secondaryColor = sf:: Color(255, 200, 0);
            break;
        case MINERALS:
            value = 75;
            primaryColor = sf:: Color(0, 150, 255);
            secondaryColor = sf::Color(0, 100, 200);
            break;
        case SCRAP:
            value = 25;
            primaryColor = sf:: Color(150, 150, 150);
            secondaryColor = sf::Color(100, 100, 100);
            break;
    }
}

void Resource::render(sf::RenderWindow& window) {
    if (!active) return;
    
    float currentY = position. y + floatOffset;
    float pulse = 1.0f + std::sin(pulseTimer * 4.0f) * 0.15f;
    
    // Glow effect
    sf::CircleShape glow(18);
    glow.setOrigin(18, 18);
    glow.setPosition(position.x, currentY);
    sf::Color glowColor = primaryColor;
    glowColor. a = 80;
    glow.setFillColor(glowColor);
    window.draw(glow);
    
    // Crystal/gem (hexagon)
    sf::CircleShape gem(12, 6);
    gem.setOrigin(12, 12);
    gem.setPosition(position.x, currentY);
    gem.setRotation(rotation + pulseTimer * 40);
    gem.setFillColor(primaryColor);
    gem.setOutlineColor(secondaryColor);
    gem.setOutlineThickness(2);
    gem.setScale(pulse, pulse);
    window.draw(gem);
    
    // Inner shine
    sf::CircleShape shine(6, 4);
    shine.setOrigin(6, 6);
    shine.setPosition(position.x - 3, currentY - 3);
    shine.setRotation(-rotation);
    sf::Color shineColor = sf::Color:: White;
    shineColor. a = 200;
    shine.setFillColor(shineColor);
    window.draw(shine);
    
    // Sparkles
    for (int i = 0; i < 4; i++) {
        float sparkleAngle = (90.0f * i + pulseTimer * 100) * 3.14159f / 180.0f;
        float sparkleDist = 15.0f + std::sin(pulseTimer * 5.0f + i) * 3.0f;
        
        sf::CircleShape sparkle(2);
        sparkle.setPosition(
            position.x + std::cos(sparkleAngle) * sparkleDist - 2,
            currentY + std:: sin(sparkleAngle) * sparkleDist - 2
        );
        sparkle.setFillColor(sf::Color::White);
        window.draw(sparkle);
    }
}

void Resource::apply(PlayerShip* player) {
    if (!player) return;
    
    player->addScore(value);
    collect();
}

// OPERATOR OVERLOADING
Resource Resource::operator+(const Resource& other) const {
    Resource combined(position.x, position.y, type);
    combined.value = this->value + other.value;
    
    // Bonus for combining different types
    if (type != other.type) {
        combined.value = (this->value + other.value) * 1.5f;
        
        if (type == MINERALS || other.type == MINERALS) {
            combined.type = MINERALS;
        } else if (type == ENERGY || other.type == ENERGY) {
            combined.type = ENERGY;
        }
    }
    
    return combined;
}