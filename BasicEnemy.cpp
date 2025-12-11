#include "BasicEnemy.h"
#include "EnemyBullet.h"
#include "ColorPalette.h"
#include <cmath>

BasicEnemy::BasicEnemy(float x, float y)
    : EnemyShip(x, y, 30.0f, 25.0f, sf::Color::Red, 2.0f, 100.0f, 100) {
    
    velocity = sf::Vector2f(0, speed);
    shootInterval = 2.5f;
    
    // Apply theme color
    ColorPalette* palette = ColorPalette::getInstance();
    color = palette->getEnemyBasicPrimary();
}

void BasicEnemy::updateMovementPattern(float deltaTime) {
    // Simple straight-down with slight wobble
    position. x += std::sin(position.y * 0.01f) * 30.0f * deltaTime;
}

void BasicEnemy::render(sf::RenderWindow& window) {
    if (!active) return;
    
    ColorPalette* palette = ColorPalette::getInstance();
    
    // Glow effect
    for (int i = 2; i > 0; i--) {
        sf::ConvexShape glow(3);
        glow.setPoint(0, sf::Vector2f(0, size/2));
        glow.setPoint(1, sf::Vector2f(-size/2, -size/2));
        glow.setPoint(2, sf:: Vector2f(size/2, -size/2));
        
        float scale = 1.0f + i * 0.1f;
        glow.setScale(scale, scale);
        glow.setPosition(position);
        
        sf::Color glowColor = color;
        glowColor.a = 40 - i * 15;
        glow.setFillColor(glowColor);
        
        window.draw(glow);
    }
    
    // Main triangle (pointing down)
    sf::ConvexShape triangle(3);
    triangle.setPoint(0, sf::Vector2f(0, size/2));
    triangle.setPoint(1, sf::Vector2f(-size/2, -size/2));
    triangle.setPoint(2, sf::Vector2f(size/2, -size/2));
    
    triangle.setFillColor(color);
    triangle.setOutlineColor(ColorPalette::darken(color, 0.3f));
    triangle.setOutlineThickness(2);
    triangle.setPosition(position);
    
    window.draw(triangle);
    
    // Core detail
    sf::CircleShape core(size * 0.2f);
    core.setFillColor(ColorPalette:: lighten(color, 0.5f));
    core.setPosition(position.x - size * 0.2f, position. y - size * 0.1f);
    window.draw(core);
}

void BasicEnemy::shoot(std::vector<Projectile*>& projectiles) {
    if (!canShoot() || !active) return;
    
    projectiles.push_back(new EnemyBullet(
        position.x, position.y + size/2, 0, 1
    ));
    
    resetCooldown();
}