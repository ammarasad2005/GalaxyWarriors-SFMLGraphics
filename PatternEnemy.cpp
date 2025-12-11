#include "PatternEnemy.h"
#include "EnemyBullet.h"
#include "ColorPalette.h"
#include <cmath>
#include <cstdlib>

PatternEnemy::PatternEnemy(float x, float y)
    : EnemyShip(x, y, 50.0f, 28.0f, sf::Color::Yellow, 3.0f, 80.0f, 200),
      time(0),
      patternType(1 + (rand() % 5)) {
    
    shootInterval = 2.0f;
    
    ColorPalette* palette = ColorPalette::getInstance();
    color = palette->getEnemyPatternPrimary();
}

void PatternEnemy::updateMovementPattern(float deltaTime) {
    time += deltaTime;
    
    switch(patternType) {
        case 1: // Sine wave
            position.x += std::cos(time * 2.0f) * 100.0f * deltaTime;
            velocity.y = speed;
            break;
            
        case 2: // Figure-8
            {
                float angle = time * 2.0f;
                position.x = 400 + std::sin(angle) * 150;
                position.y += speed * deltaTime * 0.8f;
            }
            break;
            
        case 3: // Zigzag
            if (time < 1.0f) {
                velocity.x = 150;
            } else if (time < 2.0f) {
                velocity.x = -150;
            } else {
                time = 0;
            }
            velocity.y = speed;
            break;
            
        case 4: // Spiral
            {
                float radius = 50 + time * 10;
                float angle = time * 3;
                position.x = 400 + std::cos(angle) * radius;
                velocity.y = speed * 0.7f;
            }
            break;
            
        case 5: // Wave with vertical oscillation
            position.x += std::sin(time * 2) * 100 * deltaTime;
            position.y += (speed + std::sin(time * 4) * 30) * deltaTime;
            break;
    }
    
    // Keep within bounds
    if (position. x < 50) position.x = 50;
    if (position.x > 750) position.x = 750;
}

void PatternEnemy::render(sf::RenderWindow& window) {
    if (!active) return;
    
    ColorPalette* palette = ColorPalette::getInstance();
    float pulse = 1.0f + std::sin(time * 5.0f) * 0.15f;
    
    // Outer glow layers
    for (int i = 2; i >= 0; i--) {
        sf::ConvexShape glowDiamond(4);
        float glowSize = size + i * 6;
        glowDiamond.setPoint(0, sf:: Vector2f(0, -glowSize/2));
        glowDiamond.setPoint(1, sf::Vector2f(glowSize/2, 0));
        glowDiamond.setPoint(2, sf::Vector2f(0, glowSize/2));
        glowDiamond.setPoint(3, sf::Vector2f(-glowSize/2, 0));
        glowDiamond.setPosition(position);
        glowDiamond.setRotation(time * 30 + i * 45);
        
        sf::Color glowColor = color;
        glowColor.a = 60 - i * 20;
        glowDiamond.setFillColor(glowColor);
        
        window.draw(glowDiamond);
    }
    
    // Main diamond
    sf::ConvexShape diamond(4);
    diamond.setPoint(0, sf::Vector2f(0, -size/2));
    diamond.setPoint(1, sf::Vector2f(size/2, 0));
    diamond.setPoint(2, sf::Vector2f(0, size/2));
    diamond.setPoint(3, sf::Vector2f(-size/2, 0));
    
    diamond.setFillColor(color);
    diamond.setOutlineColor(ColorPalette:: lighten(color, 0.4f));
    diamond.setOutlineThickness(2);
    diamond.setPosition(position);
    diamond.setRotation(time * 50);
    diamond.setScale(pulse, pulse);
    
    window.draw(diamond);
    
    // Inner core
    sf::CircleShape core(size * 0.3f, 6);
    core.setOrigin(size * 0.3f, size * 0.3f);
    core.setFillColor(ColorPalette:: lighten(color, 0.6f));
    core.setPosition(position);
    core.setRotation(-time * 70);
    window.draw(core);
    
    // Energy trails
    for (int i = 0; i < 4; i++) {
        float angle = (90.0f * i + time * 100) * 3.14159f / 180.0f;
        float trailDist = size * 0.6f;
        
        sf::CircleShape trail(3 - i * 0.5f);
        trail.setPosition(
            position.x + std::cos(angle) * trailDist,
            position.y + std::sin(angle) * trailDist
        );
        sf::Color trailColor = color;
        trailColor.a = 150 - i * 35;
        trail.setFillColor(trailColor);
        window.draw(trail);
    }
}

void PatternEnemy::shoot(DynamicArray<Projectile*>& projectiles) {
    if (!Ship::canShoot() || !active) return;
    
    projectiles.pushBack(new EnemyBullet(
        position.x, position.y + size/2, 0, 1
    ));
    
    Ship::resetCooldown();
}