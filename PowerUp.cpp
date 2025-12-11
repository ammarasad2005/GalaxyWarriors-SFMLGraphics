#include "PowerUp.h"
#include "ColorPalette.h"
#include <cmath>

PowerUp::PowerUp(float x, float y, Type t)
    : Collectible(x, y, 8.0f),
      type(t),
      duration(0),
      effectStrength(1.0f) {
    
    ColorPalette* palette = ColorPalette::getInstance();
    
    // Balanced durations & effects
    switch(type) {
        case SHIELD:
            duration = 7.0f;
            effectStrength = 1.0f;
            glowColor = palette->getPowerUpShield();
            name = "SHIELD";
            break;
            
        case DOUBLE_FIRE:
            duration = 10.0f;
            effectStrength = 2.0f;
            glowColor = palette->getPowerUpFire();
            name = "DOUBLE FIRE";
            break;
            
        case REPAIR: 
            duration = 0.1f;
            effectStrength = 40.0f;
            glowColor = palette->getPowerUpRepair();
            name = "REPAIR";
            break;
            
        case SPEED_BOOST:
            duration = 8.0f;
            effectStrength = 1.5f;
            glowColor = palette->getPowerUpSpeed();
            name = "SPEED";
            break;
    }
}

void PowerUp::render(sf::RenderWindow& window) {
    if (!active) return;
    
    float currentY = position.y + floatOffset;
    float pulse = 1.0f + std::sin(pulseTimer * 5.0f) * 0.2f;
    
    // Outer glow
    sf::RectangleShape outerGlow(sf::Vector2f(35, 35));
    outerGlow.setOrigin(17.5f, 17.5f);
    outerGlow.setPosition(position.x, currentY);
    outerGlow.setRotation(rotation + pulseTimer * 50);
    sf::Color glowTransparent = glowColor;
    glowTransparent.a = 100;
    outerGlow.setFillColor(sf::Color::Transparent);
    outerGlow.setOutlineColor(glowTransparent);
    outerGlow.setOutlineThickness(3);
    outerGlow.setScale(pulse, pulse);
    window.draw(outerGlow);
    
    // Main icon (hexagon)
    sf::CircleShape icon(15, 6);
    icon.setOrigin(15, 15);
    icon.setPosition(position.x, currentY);
    icon.setRotation(-rotation - pulseTimer * 30);
    icon.setFillColor(glowColor);
    icon.setOutlineColor(sf::Color::White);
    icon.setOutlineThickness(2);
    window.draw(icon);
    
    // Inner detail
    sf::CircleShape innerDetail(8);
    innerDetail.setOrigin(8, 8);
    innerDetail.setPosition(position.x, currentY);
    
    if (type == SHIELD) {
        innerDetail.setFillColor(sf::Color(100, 200, 255));
    } else if (type == DOUBLE_FIRE) {
        innerDetail.setFillColor(sf::Color(255, 100, 100));
    } else if (type == REPAIR) {
        innerDetail.setFillColor(sf::Color(100, 255, 100));
        
        // Draw cross
        sf::RectangleShape cross1(sf::Vector2f(10, 2));
        cross1.setOrigin(5, 1);
        cross1.setPosition(position.x, currentY);
        cross1.setFillColor(sf::Color::White);
        window.draw(cross1);
        
        sf::RectangleShape cross2(sf::Vector2f(2, 10));
        cross2.setOrigin(1, 5);
        cross2.setPosition(position.x, currentY);
        cross2.setFillColor(sf::Color::White);
        window.draw(cross2);
    } else if (type == SPEED_BOOST) {
        innerDetail.setFillColor(sf::Color(255, 255, 100));
        
        // Draw arrow
        sf::ConvexShape arrow(3);
        arrow.setPoint(0, sf::Vector2f(0, -6));
        arrow.setPoint(1, sf::Vector2f(-4, 4));
        arrow.setPoint(2, sf::Vector2f(4, 4));
        arrow.setPosition(position.x, currentY);
        arrow.setFillColor(sf::Color::White);
        window.draw(arrow);
    }
    
    window.draw(innerDetail);
    
    // Blink effect near end of life
    if (lifeTime < 2.0f) {
        int blinkFrame = static_cast<int>(lifeTime * 8) % 2;
        if (blinkFrame == 0) return;
    }
    
    // Particle trail
    for (int i = 0; i < 3; i++) {
        sf::CircleShape particle(3 - i);
        particle.setPosition(
            position.x - 3 + i, 
            currentY + 20 + i * 5
        );
        sf::Color particleColor = glowColor;
        particleColor.a = 150 - i * 50;
        particle.setFillColor(particleColor);
        window.draw(particle);
    }
}

void PowerUp:: apply(PlayerShip* player) {
    if (!player) return;
    
    switch(type) {
        case SHIELD:
            player->activateShield(duration);
            break;
        case DOUBLE_FIRE: 
            player->activateDoubleFire(duration);
            break;
        case REPAIR: 
            player->heal(effectStrength);
            break;
        case SPEED_BOOST:
            player->setSpeed(player->getSpeed() * effectStrength);
            break;
    }
    
    collect();
}