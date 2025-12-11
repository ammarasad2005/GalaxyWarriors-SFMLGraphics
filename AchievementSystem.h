#ifndef ACHIEVEMENTSYSTEM_H
#define ACHIEVEMENTSYSTEM_H

#include <string>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>

struct Achievement {
    std::string id;
    std::string name;
    std::string description;
    int targetValue;
    int currentValue;
    bool unlocked;
    sf::Color color;
    
    Achievement(const std::string& id, const std::string& n, 
                const std::string& desc, int target, sf::Color col = sf::Color::Yellow)
        : id(id), name(n), description(desc), 
          targetValue(target), currentValue(0), 
          unlocked(false), color(col) {}
    
    float getProgress() const {
        return static_cast<float>(currentValue) / targetValue;
    }
    
    bool checkUnlock() {
        if (!  unlocked && currentValue >= targetValue) {
            unlocked = true;
            return true;
        }
        return false;
    }
};

class ComboSystem {
private:  
    int currentCombo;
    int maxCombo;
    float comboTimer;
    float comboTimeout;
    int comboLevel;
    
public:
    ComboSystem() 
        : currentCombo(0), maxCombo(0), 
          comboTimer(0), comboTimeout(3.0f), comboLevel(0) {}
    
    void addKill();
    void resetCombo();
    void update(float deltaTime);
    
    int getCurrentCombo() const { return currentCombo; }
    int getMaxCombo() const { return maxCombo; }
    int getComboLevel() const { return comboLevel; }
    float getMultiplier() const { return 1.0f + comboLevel * 0.2f; }
    float getTimeRemaining() const { return comboTimeout - comboTimer; }
    
    std::string getComboRank() const;
    sf::Color getComboColor() const;
};

class AchievementSystem {
private:  
    std::vector<Achievement> achievements;
    std::vector<std::string> recentUnlocks;
    float notificationTimer;
    ComboSystem comboSystem;
    
    int totalKills;
    int totalShots;
    int totalScore;
    int perfectWaves;
    int powerUpsCollected;
    int bossesDefeated;
    
public:  
    AchievementSystem();
    
    void initializeAchievements();
    
    void onEnemyKilled();
    void onShotFired();
    void onScoreGained(int points);
    void onWaveCompleted(bool perfect);
    void onPowerUpCollected();
    void onBossDefeated();
    void onLevelCompleted(int level);
    
    ComboSystem& getComboSystem() { return comboSystem; }
    
    const std::vector<Achievement>& getAchievements() const { return achievements; }
    int getUnlockedCount() const;
    int getTotalCount() const { return achievements.size(); }
    
    void update(float deltaTime);
    void renderNotifications(sf::RenderWindow& window);
    void renderComboCounter(sf::RenderWindow& window, sf::Vector2f position);
    
    void save(const std::string& filename);
    void load(const std:: string& filename);
    
private:
    void checkAchievement(const std::string& id);
    void unlockAchievement(const std::string& id);
};

#endif