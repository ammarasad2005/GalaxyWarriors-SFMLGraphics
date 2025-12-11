#include "DifficultyManager.h"

DifficultyManager* DifficultyManager::instance = nullptr;

DifficultyManager::DifficultyManager()
    : difficultyMultiplier(1.0f),
      playerDeaths(0),
      consecutiveKills(0),
      adaptiveScale(1.0f) {
}

DifficultyManager* DifficultyManager::getInstance() {
    if (instance == nullptr) {
        instance = new DifficultyManager();
    }
    return instance;
}

float DifficultyManager::getEnemyHealthMultiplier() const {
    float killBonus = 1.0f + (consecutiveKills * 0.05f);
    float deathPenalty = 1.0f - (playerDeaths * 0.1f);
    deathPenalty = deathPenalty < 0.5f ? 0.5f : deathPenalty;
    
    return difficultyMultiplier * killBonus * deathPenalty * adaptiveScale;
}

float DifficultyManager::getEnemySpeedMultiplier() const {
    return difficultyMultiplier * (1.0f + consecutiveKills * 0.03f) * adaptiveScale;
}

float DifficultyManager::getSpawnRateMultiplier() const {
    float rate = 1.0f + (consecutiveKills * 0.05f);
    if (playerDeaths > 0) {
        rate *= (1.0f - playerDeaths * 0.15f);
        rate = rate < 0.6f ? 0.6f : rate;
    }
    return rate;
}

float DifficultyManager::getScoreMultiplier() const {
    return 1.0f + (difficultyMultiplier - 1.0f) * 0.5f + (consecutiveKills * 0.02f);
}

void DifficultyManager::onPlayerDeath() {
    playerDeaths++;
    consecutiveKills = 0;
    
    adaptiveScale *= 0.9f;
    adaptiveScale = adaptiveScale < 0.5f ? 0.5f : adaptiveScale;
}

void DifficultyManager::onEnemyKilled() {
    consecutiveKills++;
    
    if (consecutiveKills % 10 == 0) {
        adaptiveScale += 0.05f;
        adaptiveScale = adaptiveScale > 2.0f ? 2.0f : adaptiveScale;
    }
}

void DifficultyManager::onLevelComplete() {
    difficultyMultiplier += 0.3f;
    consecutiveKills = 0;
}

void DifficultyManager:: reset() {
    difficultyMultiplier = 1.0f;
    playerDeaths = 0;
    consecutiveKills = 0;
    adaptiveScale = 1.0f;
}

void DifficultyManager::updateAdaptive(float playerHealthPercent, int currentScore) {
    if (playerHealthPercent < 0.3f) {
        adaptiveScale -= 0.01f;
        adaptiveScale = adaptiveScale < 0.5f ? 0.5f : adaptiveScale;
    }
    
    if (playerHealthPercent > 0.8f && consecutiveKills > 15) {
        adaptiveScale += 0.01f;
        adaptiveScale = adaptiveScale > 2.0f ? 2.0f : adaptiveScale;
    }
}