#include "PlayerShip.h"
#include "PlayerBullet.h"
#include "ColorPalette.h"
#include <SFML/Window/Keyboard.hpp>
#include <cmath>

// CONSTRUCTOR
PlayerShip::PlayerShip(float x, float y)
    : Ship(x, y, 100.0f, 30.0f, sf::Color::Cyan, 5.0f, 300.0f),
      score(0),
      lives(3),
      shieldActive(false),
      shieldDuration(0),
      doubleFireActive(false),
      doubleFireDuration(0),
      invincibilityTimer(0) {
}

void PlayerShip::update(float deltaTime) {
    if (! active) return;
    
    // Update power-up timers
    if (shieldActive) {
        shieldDuration -= deltaTime;
        if (shieldDuration <= 0) {
            shieldActive = false;
            shieldDuration = 0;
        }
    }
    
    if (doubleFireActive) {
        doubleFireDuration -= deltaTime;
        if (doubleFireDuration <= 0) {
            doubleFireActive = false;
            doubleFireDuration = 0;
        }
    }
    
    if (invincibilityTimer > 0) {
        invincibilityTimer -= deltaTime;
    }
    
    // Update fire cooldown
    updateCooldown(deltaTime);
    
    // Update position based on velocity
    position += velocity * deltaTime;
    
    // Screen boundaries (800x600)
    if (position.x < size/2) position.x = size/2;
    if (position.x > 800 - size/2) position.x = 800 - size/2;
    if (position.y < size/2) position.y = size/2;
    if (position.y > 600 - size/2) position.y = 600 - size/2;
    
    // Gradually reduce velocity (friction)
    velocity *= 0.95f;
}

void PlayerShip::render(sf::RenderWindow& window) {
    if (!active) return;
    
    ColorPalette* palette = ColorPalette::getInstance();
    
    // Create triangle ship shape
    sf::ConvexShape triangle(3);
    triangle.setPoint(0, sf::Vector2f(0, -size/2));
    triangle.setPoint(1, sf::Vector2f(-size/2, size/2));
    triangle.setPoint(2, sf::Vector2f(size/2, size/2));
    
    // Dynamic color based on power-ups
    sf::Color shipColor = palette->getPlayerPrimary();
    sf::Color glowColor = palette->getPlayerSecondary();
    
    if (shieldActive) {
        shipColor = palette->getPowerUpShield();
        glowColor = ColorPalette::lighten(shipColor, 0.3f);
    } else if (doubleFireActive) {
        shipColor = palette->getPowerUpFire();
        glowColor = ColorPalette::lighten(shipColor, 0.3f);
    }
    
    // Blinking effect when invincible
    if (invincibilityTimer > 0) {
        int blink = static_cast<int>(invincibilityTimer * 10) % 2;
        if (blink == 0) {
            shipColor. a = 128;
            glowColor.a = 64;
        }
    }
    
    // Draw outer glow (multiple layers)
    for (int i = 3; i > 0; i--) {
        sf::ConvexShape glow(3);
        glow.setPoint(0, sf::Vector2f(0, -size/2));
        glow.setPoint(1, sf::Vector2f(-size/2, size/2));
        glow.setPoint(2, sf::Vector2f(size/2, size/2));
        
        float scale = 1.0f + i * 0.15f;
        glow.setScale(scale, scale);
        glow.setPosition(position);
        glow.setRotation(rotation);
        
        sf::Color layerColor = glowColor;
        layerColor.a = 30 * (4 - i);
        glow.setFillColor(layerColor);
        
        window.draw(glow);
    }
    
    // Main ship body
    triangle.setFillColor(shipColor);
    triangle.setOutlineColor(ColorPalette::lighten(shipColor, 0.5f));
    triangle.setOutlineThickness(2);
    triangle.setPosition(position);
    triangle.setRotation(rotation);
    
    window.draw(triangle);
    
    // Cockpit detail
    sf::CircleShape cockpit(size * 0.2f);
    cockpit.setFillColor(ColorPalette::lighten(shipColor, 0.6f));
    cockpit.setPosition(position. x - size * 0.2f, position.y - size * 0.15f);
    window.draw(cockpit);
    
    // Wing details
    sf::ConvexShape leftWing(3);
    leftWing.setPoint(0, sf::Vector2f(-size * 0.4f, -size * 0.1f));
    leftWing.setPoint(1, sf::Vector2f(-size * 0.5f, size * 0.3f));
    leftWing.setPoint(2, sf::Vector2f(-size * 0.3f, size * 0.2f));
    leftWing.setFillColor(ColorPalette::darken(shipColor, 0.2f));
    leftWing.setPosition(position);
    window.draw(leftWing);
    
    sf::ConvexShape rightWing(3);
    rightWing.setPoint(0, sf::Vector2f(size * 0.4f, -size * 0.1f));
    rightWing.setPoint(1, sf::Vector2f(size * 0.3f, size * 0.2f));
    rightWing.setPoint(2, sf::Vector2f(size * 0.5f, size * 0.3f));
    rightWing.setFillColor(ColorPalette::darken(shipColor, 0.2f));
    rightWing.setPosition(position);
    window.draw(rightWing);
    
    // Shield visual
    if (shieldActive) {
        float shieldPulse = std::sin(shieldDuration * 5.0f) * 0.1f + 0.9f;
        
        for (int i = 0; i < 3; i++) {
            sf::CircleShape shield(size + i * 8);
            shield.setFillColor(sf::Color::Transparent);
            shield.setOutlineColor(ColorPalette::withAlpha(
                palette->getPowerUpShield(), 
                100 - i * 30
            ));
            shield. setOutlineThickness(2);
            shield.setPosition(position. x - size - i * 8, position.y - size - i * 8);
            shield.setScale(shieldPulse, shieldPulse);
            window.draw(shield);
        }
    }
    
    // Enhanced engine glow
    float engineFlicker = 0.8f + (rand() % 40) * 0.01f;
    sf::Color engineColor = palette->getPlayerSecondary();
    
    for (int i = 0; i < 2; i++) {
        float xOffset = (i == 0) ? -size/4 : size/4;
        
        // Outer glow
        sf::CircleShape engineGlow(8);
        engineGlow.setFillColor(ColorPalette::withAlpha(engineColor, 80));
        engineGlow.setPosition(position.x + xOffset - 8, position.y + size/2 - 8);
        engineGlow.setScale(1.2f, 1.5f * engineFlicker);
        window.draw(engineGlow);
        
        // Core
        sf::CircleShape engine(5);
        engine.setFillColor(ColorPalette::lighten(engineColor, 0.5f));
        engine.setPosition(position.x + xOffset - 5, position.y + size/2 - 5);
        window.draw(engine);
        
        // Flame trail
        if (velocity.y < -50 || velocity.y > 50) {
            for (int j = 0; j < 3; j++) {
                sf:: CircleShape flame(3 - j);
                flame.setFillColor(ColorPalette::withAlpha(
                    engineColor,
                    150 - j * 50
                ));
                flame. setPosition(
                    position.x + xOffset - (3 - j),
                    position.y + size/2 + j * 6
                );
                window.draw(flame);
            }
        }
    }
}

void PlayerShip::handleInput(float deltaTime) {
    sf::Vector2f acceleration(0, 0);
    float accelSpeed = speed * 2.0f;
    
    // WASD or Arrow keys
    if (sf:: Keyboard::isKeyPressed(sf:: Keyboard::W) || 
        sf::Keyboard::isKeyPressed(sf::Keyboard:: Up)) {
        acceleration.y = -accelSpeed;
    }
    if (sf:: Keyboard::isKeyPressed(sf:: Keyboard::S) || 
        sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        acceleration.y = accelSpeed;
    }
    if (sf::Keyboard:: isKeyPressed(sf::Keyboard::A) || 
        sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        acceleration.x = -accelSpeed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard:: D) || 
        sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        acceleration.x = accelSpeed;
    }
    
    // Apply acceleration
    velocity += acceleration * deltaTime;
    
    // Clamp velocity
    float maxSpeed = speed;
    float currentSpeed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (currentSpeed > maxSpeed) {
        velocity = (velocity / currentSpeed) * maxSpeed;
    }
}

void PlayerShip::shoot(std::vector<Projectile*>& projectiles) {
    if (!canShoot() || !active) return;
    
    if (doubleFireActive) {
        projectiles.push_back(new PlayerBullet(
            position.x - 10, position.y - size/2, 0, -1
        ));
        projectiles. push_back(new PlayerBullet(
            position.x + 10, position.y - size/2, 0, -1
        ));
    } else {
        projectiles.push_back(new PlayerBullet(
            position.x, position.y - size/2, 0, -1
        ));
    }
    
    resetCooldown();
}

void PlayerShip::onCollision(GameObject* other) {
    // Handled in CollisionManager
}

void PlayerShip:: addScore(int points) {
    score += points;
}

void PlayerShip::activateShield(float duration) {
    shieldActive = true;
    shieldDuration = duration;
}

void PlayerShip::activateDoubleFire(float duration) {
    doubleFireActive = true;
    doubleFireDuration = duration;
}

void PlayerShip::loseLife() {
    lives--;
    if (lives <= 0) {
        destroy();
    } else {
        invincibilityTimer = 2.0f;
        health = maxHealth;
    }
}

void PlayerShip::takeDamage(float damage) {
    takeDamage(damage, false);
}

void PlayerShip:: takeDamage(float damage, bool ignoreShield) {
    if (invincibilityTimer > 0) return;
    
    if (shieldActive && ! ignoreShield) {
        return;
    }
    
    health -= damage;
    if (health <= 0) {
        health = 0;
        loseLife();
    }
}