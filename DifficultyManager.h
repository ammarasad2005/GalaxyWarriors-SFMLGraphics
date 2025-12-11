#ifndef DIFFICULTYMANAGER_H
#define DIFFICULTYMANAGER_H

// SINGLETON:   Manages dynamic difficulty scaling
class DifficultyManager {
private:
    static DifficultyManager* instance;
    
    float difficultyMultiplier;
    int playerDeaths;
    int consecutiveKills;
    float adaptiveScale;
    
    DifficultyManager();
    
public:
    static DifficultyManager* getInstance();
    
    float getEnemyHealthMultiplier() const;
    float getEnemySpeedMultiplier() const;
    float getSpawnRateMultiplier() const;
    float getScoreMultiplier() const;
    
    void onPlayerDeath();
    void onEnemyKilled();
    void onLevelComplete();
    void reset();
    
    void updateAdaptive(float playerHealthPercent, int currentScore);
    
    float getDifficultyMultiplier() const { return difficultyMultiplier; }
};

#endif