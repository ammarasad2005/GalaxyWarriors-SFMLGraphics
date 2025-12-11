#ifndef BOSSENEMY_H
#define BOSSENEMY_H

#include "EnemyShip.h"
#include "AttackPattern.h"
#include <memory>

class BossEnemy :  public EnemyShip {
private:
    int phase;
    int maxPhases;
    float phaseTimer;
    float movementDirection;
    float invulnerabilityTimer;
    bool isTransitioning;
    float transitionTimer;
    float transitionDuration;
    
    std::unique_ptr<AttackPattern> currentAttackPattern;
    std::vector<std::unique_ptr<AttackPattern>> phaseAttackPatterns;
    
    float shieldRotation;
    float energyPulse;
    sf::Color currentColor;
    sf::Color targetColor;
    float colorTransition;
    
    struct PhaseData {
        float healthThreshold;
        std::string name;
        sf::Color color;
        float speedMultiplier;
        float fireRateMultiplier;
        int attackPatternIndex;
    };
    std::vector<PhaseData> phases;
    
public:
    BossEnemy(float x, float y);
    ~BossEnemy() override {}
    
    void updateMovementPattern(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void shoot(std::vector<Projectile*>& projectiles) override;
    void takeDamage(float damage) override;
    void update(float deltaTime) override;
    
    void initializePhases();
    void checkPhaseTransition();
    void startPhaseTransition(int newPhase);
    void updatePhaseTransition(float deltaTime);
    void activatePhase(int phaseNum);
    
    int getPhase() const { return phase; }
    int getMaxPhases() const { return maxPhases; }
    bool isInTransition() const { return isTransitioning; }
    float getPhaseProgress() const;
    const std::string& getCurrentPhaseName() const;
    
    void executePhase1Movement(float deltaTime);
    void executePhase2Movement(float deltaTime);
    void executePhase3Movement(float deltaTime);
    void executePhase4Movement(float deltaTime);
    
    void renderHealthBar(sf::RenderWindow& window);
    void renderPhaseIndicator(sf::RenderWindow& window);
    void renderShield(sf::RenderWindow& window);
    void renderEnergyCore(sf::RenderWindow& window);
    void renderTransitionEffect(sf::RenderWindow& window);
};

#endif