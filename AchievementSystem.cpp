#include "AchievementSystem.h"
#include "ColorPalette.h"
#include <fstream>
#include <sstream>
#include <cmath>

// ComboSystem implementation
void ComboSystem::addKill() {
    currentCombo++;
    comboTimer = 0;
    
    if (currentCombo > maxCombo) {
        maxCombo = currentCombo;
    }
    
    comboLevel = std::min(5, currentCombo / 5);
}

void ComboSystem:: resetCombo() {
    currentCombo = 0;
    comboLevel = 0;
    comboTimer = 0;
}

void ComboSystem::update(float deltaTime) {
    if (currentCombo > 0) {
        comboTimer += deltaTime;
        if (comboTimer >= comboTimeout) {
            resetCombo();
        }
    }
}

std::string ComboSystem::getComboRank() const {
    if (comboLevel == 0) return "NO COMBO";
    if (comboLevel == 1) return "GOOD!   ";
    if (comboLevel == 2) return "GREAT!";
    if (comboLevel == 3) return "EXCELLENT!";
    if (comboLevel == 4) return "AMAZING!";
    return "UNSTOPPABLE!";
}

sf::Color ComboSystem::getComboColor() const {
    ColorPalette* palette = ColorPalette::getInstance();
    
    switch(comboLevel) {
        case 0: return sf::Color:: White;
        case 1: return sf::Color::Yellow;
        case 2: return sf::Color(255, 165, 0);
        case 3: return palette->getPlayerPrimary();
        case 4: return sf::Color::   Magenta;
        case 5: return sf::Color(255, 0, 255);
        default: return sf::  Color::White;
    }
}

// AchievementSystem implementation
AchievementSystem:: AchievementSystem()
    : notificationTimer(0),
      totalKills(0),
      totalShots(0),
      totalScore(0),
      perfectWaves(0),
      powerUpsCollected(0),
      bossesDefeated(0) {
    
    initializeAchievements();
}

void AchievementSystem::initializeAchievements() {
    ColorPalette* palette = ColorPalette::getInstance();
    
    achievements.push_back(Achievement(
        "first_blood", "First Blood", 
        "Defeat your first enemy", 1, sf::Color::   Cyan
    ));
    
    achievements.push_back(Achievement(
        "exterminator", "Exterminator",
        "Defeat 100 enemies", 100, sf::Color:: Red
    ));
    
    achievements.push_back(Achievement(
        "ace_pilot", "Ace Pilot",
        "Defeat 500 enemies", 500, sf::   Color::Yellow
    ));
    
    achievements.push_back(Achievement(
        "combo_master", "Combo Master",
        "Reach a 25x combo", 25, sf::Color(255, 165, 0)
    ));
    
    achievements.push_back(Achievement(
        "unstoppable", "Unstoppable",
        "Reach a 50x combo", 50, sf::Color::   Magenta
    ));
    
    achievements.push_back(Achievement(
        "high_scorer", "High Scorer",
        "Score 10,000 points", 10000, palette->getUIAccent()
    ));
    
    achievements.push_back(Achievement(
        "legend", "Legend",
        "Score 50,000 points", 50000, sf::Color(255, 215, 0)
    ));
    
    achievements.push_back(Achievement(
        "sharpshooter", "Sharpshooter",
        "Fire 1000 shots", 1000, sf::Color::  Cyan
    ));
    
    achievements.push_back(Achievement(
        "collector", "Collector",
        "Collect 50 power-ups", 50, palette->getPowerUpShield()
    ));
    
    achievements.push_back(Achievement(
        "perfectionist", "Perfectionist",
        "Complete 10 waves without taking damage", 10, sf::Color::   Green
    ));
    
    achievements.push_back(Achievement(
        "boss_slayer", "Boss Slayer",
        "Defeat a boss enemy", 1, sf::Color::   Red
    ));
    
    achievements.push_back(Achievement(
        "survivor", "Survivor",
        "Complete all 3 levels", 3, sf::Color(255, 215, 0)
    ));
}

void AchievementSystem:: onEnemyKilled() {
    totalKills++;
    comboSystem.addKill();
    
    checkAchievement("first_blood");
    checkAchievement("exterminator");
    checkAchievement("ace_pilot");
    
    int combo = comboSystem.getCurrentCombo();
    for (auto& achievement : achievements) {
        if (achievement.id == "combo_master" || achievement.id == "unstoppable") {
            if (combo >= achievement.targetValue) {
                achievement.currentValue = combo;
                achievement.checkUnlock();
            }
        }
    }
}

void AchievementSystem::onShotFired() {
    totalShots++;
    checkAchievement("sharpshooter");
}

void AchievementSystem:: onScoreGained(int points) {
    totalScore += points;
    checkAchievement("high_scorer");
    checkAchievement("legend");
}

void AchievementSystem:: onWaveCompleted(bool perfect) {
    if (perfect) {
        perfectWaves++;
        checkAchievement("perfectionist");
    }
}

void AchievementSystem:: onPowerUpCollected() {
    powerUpsCollected++;
    checkAchievement("collector");
}

void AchievementSystem::onBossDefeated() {
    bossesDefeated++;
    checkAchievement("boss_slayer");
}

void AchievementSystem::onLevelCompleted(int level) {
    for (auto& achievement : achievements) {
        if (achievement.id == "survivor") {
            achievement.currentValue = level;
            achievement.checkUnlock();
        }
    }
}

void AchievementSystem::checkAchievement(const std::string& id) {
    for (auto& achievement : achievements) {
        if (achievement.id == id && ! achievement.unlocked) {
            if (id == "first_blood" || id == "exterminator" || id == "ace_pilot") {
                achievement.currentValue = totalKills;
            } else if (id == "high_scorer" || id == "legend") {
                achievement.currentValue = totalScore;
            } else if (id == "sharpshooter") {
                achievement.currentValue = totalShots;
            } else if (id == "collector") {
                achievement.currentValue = powerUpsCollected;
            } else if (id == "perfectionist") {
                achievement.currentValue = perfectWaves;
            } else if (id == "boss_slayer") {
                achievement.currentValue = bossesDefeated;
            }
            
            if (achievement.  checkUnlock()) {
                unlockAchievement(id);
            }
        }
    }
}

void AchievementSystem::unlockAchievement(const std::string& id) {
    for (const auto& achievement : achievements) {
        if (achievement.id == id) {
            recentUnlocks.push_back(id);
            notificationTimer = 0;
            break;
        }
    }
}

int AchievementSystem::getUnlockedCount() const {
    int count = 0;
    for (const auto& achievement : achievements) {
        if (achievement.unlocked) count++;
    }
    return count;
}

void AchievementSystem::update(float deltaTime) {
    comboSystem.update(deltaTime);
    
    if (!  recentUnlocks.empty()) {
        notificationTimer += deltaTime;
        if (notificationTimer >= 4.0f) {
            recentUnlocks.erase(recentUnlocks.begin());
            notificationTimer = 0;
        }
    }
}

void AchievementSystem:: renderNotifications(sf::RenderWindow& window) {
    if (recentUnlocks.empty()) return;
    
    const Achievement* unlocked = nullptr;
    for (const auto& achievement : achievements) {
        if (achievement.id == recentUnlocks[0]) {
            unlocked = &achievement;
            break;
        }
    }
    
    if (!  unlocked) return;
    
    float slideProgress = std::min(1.0f, notificationTimer * 2.0f);
    float slideOut = (notificationTimer > 3.5f) ? (notificationTimer - 3.5f) * 2.0f : 0.0f;
    float x = 800 - (300 * slideProgress) + (300 * slideOut);
    float y = 50;
    
    sf::RectangleShape bg(sf::Vector2f(280, 80));
    bg.setPosition(x, y);
    bg.setFillColor(sf::   Color(20, 20, 40, 230));
    bg.setOutlineColor(unlocked->color);
    bg.setOutlineThickness(3);
    window.draw(bg);
    
    sf::CircleShape star(15, 5);
    star.setPosition(x + 15, y + 25);
    star.setFillColor(unlocked->color);
    star.setRotation(notificationTimer * 100);
    window.draw(star);
}

void AchievementSystem:: renderComboCounter(sf::RenderWindow& window, sf::Vector2f position) {
    int combo = comboSystem.getCurrentCombo();
    if (combo == 0) return;
    
    float pulse = 1.0f + std::sin(comboSystem.getTimeRemaining() * 10.0f) * 0.1f;
    
    sf:: RectangleShape bg(sf:: Vector2f(200, 60));
    bg.setPosition(position.   x - 100, position.y - 30);
    sf::Color bgColor = sf::Color(20, 20, 40, 200);
    bg.setFillColor(bgColor);
    bg.setOutlineColor(comboSystem.getComboColor());
    bg.setOutlineThickness(3);
    bg.setScale(pulse, pulse);
    window.draw(bg);
    
    float timePercent = comboSystem.getTimeRemaining() / 3.0f;
    sf::RectangleShape timerBar(sf::Vector2f(180 * timePercent, 5));
    timerBar.setPosition(position.x - 90, position.y + 25);
    timerBar.setFillColor(comboSystem.   getComboColor());
    window.draw(timerBar);
}

void AchievementSystem:: save(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return;
    
    for (const auto& achievement : achievements) {
        file << achievement.id << ","
             << achievement.currentValue << ","
             << achievement.unlocked << "\n";
    }
    
    file.close();
}

void AchievementSystem::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;
    
    std::  string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string id, currentStr, unlockedStr;
        
        std::getline(ss, id, ',');
        std::getline(ss, currentStr, ',');
        std::getline(ss, unlockedStr, ',');
        
        for (auto& achievement : achievements) {
            if (achievement.id == id) {
                achievement.currentValue = std::stoi(currentStr);
                achievement.unlocked = (unlockedStr == "1");
                break;
            }
        }
    }
    
    file.close();
}