#include "BossEnemy.h"
#include "EnemyBullet.h"
#include "ColorPalette.h"
#include <cmath>

BossEnemy::BossEnemy(float x, float y)
    : EnemyShip(x, y, 500.0f, 70.0f, sf::Color::Magenta, 5.0f, 150.0f, 5000),
      phase(1),
   maxPhases(4),
      phaseTimer(0),
      movementDirection(1),
      invulnerabilityTimer(0),
      isTransitioning(false),
      transitionTimer(0),
      transitionDuration(2.5f),
      shieldRotation(0),
      energyPulse(0),
      colorTransition(0) {
    
    shootInterval = 1.0f;
    
    initializePhases();
    activatePhase(1);
}

void BossEnemy::initializePhases() {
    ColorPalette* palette = ColorPalette::getInstance();
    
    // Phase 1
    PhaseData phase1;
    phase1.healthThreshold = 0.75f;
    phase1.name = "AWAKENING";
    phase1.color = palette->getBossPrimary();
    phase1.speedMultiplier = 1.0f;
    phase1.fireRateMultiplier = 1.0f;
    phase1.attackPatternIndex = 0;
    phases.push_back(phase1);
    
    // Phase 2
    PhaseData phase2;
    phase2.healthThreshold = 0.50f;
    phase2.name = "RAMPAGE";
    phase2.color = sf::Color(255, 100, 0);
    phase2.speedMultiplier = 1.3f;
    phase2.fireRateMultiplier = 1.5f;
    phase2.attackPatternIndex = 1;
    phases.push_back(phase2);
    
    // Phase 3
    PhaseData phase3;
    phase3.healthThreshold = 0.25f;
    phase3.name = "FURY";
    phase3.color = sf::Color::  Red;
    phase3.speedMultiplier = 1.6f;
    phase3.fireRateMultiplier = 2.0f;
    phase3.attackPatternIndex = 2;
    phases.push_back(phase3);
    
    // Phase 4
    PhaseData phase4;
    phase4.healthThreshold = 0.0f;
    phase4.name = "ANNIHILATION";
    phase4.color = sf::Color(150, 0, 255);
    phase4.speedMultiplier = 2.0f;
    phase4.fireRateMultiplier = 3.0f;
    phase4.attackPatternIndex = 3;
    phases.push_back(phase4);
    
    // Initialize attack patterns
    phaseAttackPatterns.push_back(std::make_unique<SpreadShotPattern>(25.0f));
    phaseAttackPatterns.push_back(std::make_unique<CircularBurstPattern>(10));
    phaseAttackPatterns.push_back(std::make_unique<SpiralPattern>());
    phaseAttackPatterns.push_back(std::make_unique<ShotgunPattern>(20));
    
    currentColor = phase1.color;
    targetColor = phase1.color;
}

void BossEnemy::update(float deltaTime) {
    if (!active) return;
    
    phaseTimer += deltaTime;
    energyPulse += deltaTime;
    shieldRotation += deltaTime * 50.0f;
    
    if (invulnerabilityTimer > 0) {
        invulnerabilityTimer -= deltaTime;
    }
    
    if (colorTransition < 1.0f) {
        colorTransition += deltaTime * 0.5f;
        if (colorTransition > 1.0f) colorTransition = 1.0f;
        
        color = ColorPalette::blend(currentColor, targetColor, colorTransition);
    }
    
    if (isTransitioning) {
        updatePhaseTransition(deltaTime);
        return;
    }
    
    updateMovementPattern(deltaTime);
    position += velocity * deltaTime;
    updateCooldown(deltaTime);
    
    if (currentAttackPattern) {
        currentAttackPattern->update(deltaTime);
    }
    
    // Keep within bounds
    if (position.x < 100) position.x = 100;
    if (position.x > 700) position.x = 700;
    if (position.y < 80) position.y = 80;
    if (position.y > 250) position.y = 250;
    
    checkPhaseTransition();
}

void BossEnemy::updateMovementPattern(float deltaTime) {
    switch(phase) {
        case 1:
            executePhase1Movement(deltaTime);
            break;
        case 2:
            executePhase2Movement(deltaTime);
            break;
        case 3:
            executePhase3Movement(deltaTime);
            break;
        case 4:
            executePhase4Movement(deltaTime);
            break;
    }
}

void BossEnemy::executePhase1Movement(float deltaTime) {
    velocity.x = movementDirection * speed;
    velocity.  y = std::sin(phaseTimer * 0.5f) * 20.0f;
    
    if (position.x > 650 || position.x < 150) {
        movementDirection *= -1;
    }
}

void BossEnemy::executePhase2Movement(float deltaTime) {
    float zigzagSpeed = speed * 1.5f;
    velocity.x = std::sin(phaseTimer * 2.0f) * zigzagSpeed;
    velocity.y = std::cos(phaseTimer * 1.5f) * 50.0f;
}

void BossEnemy::executePhase3Movement(float deltaTime) {
    float centerX = 400;
    float centerY = 150;
    float radius = 120;
    float angularSpeed = 2.5f;
    
    float angle = phaseTimer * angularSpeed;
    float targetX = centerX + radius * std::  cos(angle);
    float targetY = centerY + radius * std:: sin(angle);
    
    sf::Vector2f direction(targetX - position.x, targetY - position.y);
    float dist = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if (dist > 5.0f) {
        velocity.x = (direction.x / dist) * speed * 2.0f;
        velocity. y = (direction.y / dist) * speed * 2.0f;
    }
}

void BossEnemy::executePhase4Movement(float deltaTime) {
    float changeChance = deltaTime * 5.0f;
    if (rand() % 100 < changeChance * 100) {
        movementDirection = (rand() % 2) * 2 - 1;
    }
    
    velocity.x = movementDirection * speed * 2.0f + std::sin(phaseTimer * 5.0f) * 100.0f;
    velocity. y = std::sin(phaseTimer * 3.0f) * 80.0f;
    
    if (static_cast<int>(phaseTimer * 2) % 5 == 0) {
        velocity.y += 50.0f;
    }
}

void BossEnemy::  checkPhaseTransition() {
    if (isTransitioning || phase >= maxPhases) return;
    
    float healthPercent = health / maxHealth;
    
    if (phase < maxPhases && healthPercent <= phases[phase - 1].healthThreshold) {
        startPhaseTransition(phase + 1);
    }
}

void BossEnemy::startPhaseTransition(int newPhase) {
    if (newPhase > maxPhases) return;
    
    isTransitioning = true;
    transitionTimer = 0;
    invulnerabilityTimer = transitionDuration;
    
    velocity = sf::Vector2f(0, 0);
    
    currentColor = color;
    targetColor = phases[newPhase - 1].  color;
    colorTransition = 0;
}

void BossEnemy::updatePhaseTransition(float deltaTime) {
    transitionTimer += deltaTime;
    
    if (transitionTimer >= transitionDuration) {
        isTransitioning = false;
        activatePhase(phase + 1);
    }
}

void BossEnemy::activatePhase(int phaseNum) {
    if (phaseNum > maxPhases) return;
    
    phase = phaseNum;
    phaseTimer = 0;
    
    PhaseData& phaseData = phases[phase - 1];
    
    speed = 150.0f * phaseData.speedMultiplier;
    shootInterval = 1.0f / phaseData.fireRateMultiplier;
    color = phaseData.color;
    
    if (phaseData.attackPatternIndex < phaseAttackPatterns.size()) {
        // Note: Can't move from vector, so we'll just use reference
        currentAttackPattern. reset();
    }
}

void BossEnemy::shoot(std::vector<Projectile*>& projectiles) {
    if (! canShoot() || ! active || isTransitioning) return;
    
    sf::Vector2f pos = position;
    
    // Phase-specific attacks
    switch(phase) {
        case 1:
            // Three bullets
            projectiles.push_back(new EnemyBullet(pos.x - 20, pos.y + size, 0, 1));
            projectiles.push_back(new EnemyBullet(pos.x, pos.y + size, 0, 1));
            projectiles.push_back(new EnemyBullet(pos.x + 20, pos.y + size, 0, 1));
            break;
            
        case 2:
            // Five spread
            for (int i = -2; i <= 2; i++) {
                projectiles.push_back(new EnemyBullet(
                    pos.x, pos.y + size, 
                    i * 0.3f, 1
                ));
            }
            break;
            
        case 3:
            // Circular burst
            for (int i = 0; i < 8; i++) {
                float angle = (45.0f * i) * 3.14159f / 180.0f;
                projectiles.push_back(new EnemyBullet(
                    pos.x, pos.y,
                    std::cos(angle) * 0.5f, std::sin(angle) * 0.5f
                ));
            }
            break;
            
        case 4:
            // Rapid fire
            projectiles.push_back(new EnemyBullet(pos.x, pos. y + size, 0, 1));
            projectiles.push_back(new EnemyBullet(pos.x - 30, pos.y + size, 0, 1));
            projectiles.push_back(new EnemyBullet(pos.  x + 30, pos.y + size, 0, 1));
            break;
    }
    
    resetCooldown();
}

void BossEnemy::takeDamage(float damage) {
    if (invulnerabilityTimer > 0) return;
    
    EnemyShip::takeDamage(damage);
}

void BossEnemy::render(sf::RenderWindow& window) {
    if (!active) return;
    
    if (isTransitioning) {
        renderTransitionEffect(window);
    }
    
    if (invulnerabilityTimer > 0) {
        renderShield(window);
    }
    
    float bodyPulse = 1.0f + std::sin(energyPulse * 3.0f) * 0.05f;
    
    // Outer glow
    for (int i = 3; i >= 0; i--) {
        sf::CircleShape glow(size + i * 15, 6);
        glow.setOrigin(size + i * 15, size + i * 15);
        glow.setPosition(position);
        glow.setRotation(shieldRotation + i * 30);
        
        sf::Color glowColor = color;
        glowColor.a = 40 - i * 10;
        glow.setFillColor(glowColor);
        glow.setScale(bodyPulse, bodyPulse);
        
        window.draw(glow);
    }
    
    // Main body
    sf::CircleShape body(size, 6);
    body.setOrigin(size, size);
    body.setPosition(position);
    body.setRotation(phaseTimer * 20);
    body.setFillColor(color);
    body.setOutlineColor(ColorPalette::lighten(color, 0.4f));
    body.setOutlineThickness(4);
    
    window.draw(body);
    
    renderEnergyCore(window);
    
    // Armor plates
    for (int i = 0; i < 6; i++) {
        float angle = (60.0f * i + phaseTimer * 30) * 3.14159f / 180.0f;
        float plateDistance = size * 0.7f;
        
        sf::ConvexShape plate(3);
        plate.setPoint(0, sf::Vector2f(0, -8));
        plate.setPoint(1, sf::Vector2f(-6, 6));
        plate.setPoint(2, sf::Vector2f(6, 6));
        
        plate.setPosition(
            position.x + std::cos(angle) * plateDistance,
            position.y + std::sin(angle) * plateDistance
        );
        plate.setRotation(angle * 180.0f / 3.14159f);
        plate.setFillColor(ColorPalette::darken(color, 0.3f));
        plate.setOutlineColor(ColorPalette::lighten(color, 0.2f));
        plate.setOutlineThickness(2);
        
        window. draw(plate);
    }
    
    // Weapon hardpoints
    for (int i = 0; i < 4; i++) {
        float angle = (90.0f * i + phaseTimer * 15) * 3.14159f / 180.0f;
        float weaponDistance = size * 0.85f;
        
        sf::  CircleShape weapon(6);
        weapon.setPosition(
            position.x + std::cos(angle) * weaponDistance - 6,
            position.y + std::sin(angle) * weaponDistance - 6
        );
        weapon.setFillColor(sf::Color::Red);
        weapon.setOutlineColor(sf::Color(255, 150, 0));
        weapon.setOutlineThickness(2);
        
        if (canShoot()) {
            sf::CircleShape weaponGlow(9);
            weaponGlow.  setPosition(
                position.x + std::cos(angle) * weaponDistance - 9,
                position.y + std::sin(angle) * weaponDistance - 9
            );
            weaponGlow.setFillColor(sf::Color(255, 100, 0, 100));
            window.draw(weaponGlow);
        }
        
        window.draw(weapon);
    }
    
    renderHealthBar(window);
    renderPhaseIndicator(window);
}

void BossEnemy::renderHealthBar(sf::RenderWindow& window) {
    float barWidth = 300;
    float barHeight = 20;
    float barX = 250;
    float barY = 20;
    
    sf::RectangleShape bgBar(sf::Vector2f(barWidth, barHeight));
    bgBar.setPosition(barX, barY);
    bgBar.setFillColor(sf:: Color(40, 40, 40));
    bgBar.setOutlineColor(sf::Color::  White);
    bgBar.setOutlineThickness(2);
    window.draw(bgBar);
    
    float healthPercent = health / maxHealth;
    sf::RectangleShape healthBar(sf::Vector2f(barWidth * healthPercent, barHeight));
    healthBar.setPosition(barX, barY);
    
    sf::Color barColor;
    if (healthPercent > 0.6f) {
        barColor = sf::Color::  Green;
    } else if (healthPercent > 0.3f) {
        barColor = sf::Color::Yellow;
    } else {
        barColor = sf::Color::Red;
    }
    
    healthBar.  setFillColor(barColor);
    window.draw(healthBar);
    
    // Phase segments
    for (int i = 1; i < maxPhases; i++) {
        float segmentX = barX + barWidth * phases[i - 1].healthThreshold;
        sf::RectangleShape segment(sf::Vector2f(2, barHeight));
        segment.setPosition(segmentX, barY);
        segment.setFillColor(sf::Color(100, 100, 100));
        window.draw(segment);
    }
}

void BossEnemy::renderPhaseIndicator(sf::RenderWindow& window) {
    float indicatorY = 50;
    float spacing = 20;
    float startX = 300;
    
    for (int i = 1; i <= maxPhases; i++) {
        sf::CircleShape indicator(8);
        indicator.setPosition(startX + i * spacing, indicatorY);
        
        if (i < phase) {
            indicator.setFillColor(sf::Color(100, 100, 100));
        } else if (i == phase) {
            indicator.setFillColor(phases[i - 1].color);
            indicator.setOutlineColor(ColorPalette::lighten(phases[i - 1].color, 0.5f));
            indicator.setOutlineThickness(2);
            
            float pulse = 1.0f + std::sin(energyPulse * 5.0f) * 0.2f;
            indicator.setScale(pulse, pulse);
        } else {
            indicator.setFillColor(sf::Color(50, 50, 50));
            indicator.setOutlineColor(sf::Color(100, 100, 100));
            indicator.setOutlineThickness(1);
        }
        
        window.draw(indicator);
    }
}

void BossEnemy::renderShield(sf::RenderWindow& window) {
    float shieldAlpha = (invulnerabilityTimer / transitionDuration) * 150;
    
    for (int i = 0; i < 3; i++) {
        sf::CircleShape shield(size + 20 + i * 10, 6);
        shield.setOrigin(size + 20 + i * 10, size + 20 + i * 10);
        shield.setPosition(position);
        shield.setRotation(shieldRotation * (1 + i * 0.5f));
        shield.setFillColor(sf::Color::  Transparent);
        
        sf::Color shieldColor = ColorPalette::lighten(color, 0.3f);
        shieldColor.a = shieldAlpha - i * 30;
        shield.setOutlineColor(shieldColor);
        shield.setOutlineThickness(3);
        
        window.draw(shield);
    }
}

void BossEnemy::renderEnergyCore(sf::RenderWindow& window) {
    float corePulse = 0.8f + std::sin(energyPulse * 4.0f) * 0.2f;
    
    sf::CircleShape energyRing(size * 0.5f);
    energyRing.setOrigin(size * 0.5f, size * 0.5f);
    energyRing.setPosition(position);
    energyRing.setFillColor(sf::Color::  Transparent);
    energyRing.setOutlineColor(ColorPalette::withAlpha(
        ColorPalette::lighten(color, 0.6f), 150
    ));
    energyRing.setOutlineThickness(3);
    energyRing.setScale(corePulse, corePulse);
    window.draw(energyRing);
    
    sf::CircleShape core(size * 0.25f);
    core.setOrigin(size * 0.25f, size * 0.25f);
    core.setPosition(position);
    core.setFillColor(ColorPalette::lighten(color, 0.8f));
    core.setScale(corePulse, corePulse);
    window.draw(core);
    
    for (int i = 0; i < 8; i++) {
        float angle = (45.0f * i + energyPulse * 100) * 3.14159f / 180.0f;
        float orbitRadius = size * 0.4f;
        
        sf::  CircleShape particle(3);
        particle.setPosition(
            position.x + std::  cos(angle) * orbitRadius - 3,
            position.y + std::sin(angle) * orbitRadius - 3
        );
        particle.setFillColor(ColorPalette::lighten(color, 0.7f));
        window.draw(particle);
    }
}

void BossEnemy::renderTransitionEffect(sf::RenderWindow& window) {
    float transitionProgress = transitionTimer / transitionDuration;
    
  for (int i = 0; i < 5; i++) {
     float ringProgress = std::fmod(transitionProgress * 2.0f + i * 0.2f, 1.0f);
        float ringRadius = size + ringProgress * 200;
        float ringAlpha = (1.0f - ringProgress) * 200;
        
      sf::CircleShape ring(ringRadius);
        ring.setOrigin(ringRadius, ringRadius);
        ring.setPosition(position);
      ring.setFillColor(sf::Color::Transparent);
   
        sf::Color ringColor = ColorPalette::blend(currentColor, targetColor, transitionProgress);
        ringColor.a = ringAlpha;
        ring.setOutlineColor(ringColor);
        ring.setOutlineThickness(4);
    
        window.draw(ring);
    }
}

float BossEnemy::getPhaseProgress() const {
    if (phase >= maxPhases) return 1.0f;
    
    float healthPercent = health / maxHealth;
    float phaseStart = (phase > 1) ? phases[phase - 2].healthThreshold : 1.0f;
    float phaseEnd = phases[phase - 1].healthThreshold;
    
    float phaseRange = phaseStart - phaseEnd;
    float currentInPhase = healthPercent - phaseEnd;
    
    return 1.0f - (currentInPhase / phaseRange);
}

const std::string& BossEnemy::getCurrentPhaseName() const {
    if (phase > 0 && phase <= maxPhases) {
        return phases[phase - 1].name;
    }
    static std::string unknown = "UNKNOWN";
    return unknown;
}