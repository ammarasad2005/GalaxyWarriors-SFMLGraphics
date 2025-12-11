#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "PlayerShip.h"
#include "EnemyShip.h"
#include "Projectile.h"
#include "Asteroid.h"
#include "Collectible.h"
#include "PowerUp.h"
#include "Resource.h"
#include "CollisionManager.h"
#include "WaveManager.h"
#include "ScoreManager.h"
#include "ParticleSystem.h"
#include "DifficultyManager.h"
#include "AchievementSystem.h"
#include "MenuSystem.h"
#include <vector>

class GameEngine {
private:  
    // Window
    sf::RenderWindow window;
    sf::Clock clock;
    float deltaTime;
    
    // Game state
    GameState gameState;
    int currentLevel;
    float gameTime;
    float levelTransitionTimer;
    
    // Entities
    PlayerShip* player;
    std::vector<EnemyShip*> enemies;
    std:: vector<Projectile*> projectiles;
    std::vector<Asteroid*> asteroids;
    std::vector<Collectible*> collectibles;
    
    // Managers
    WaveManager waveManager;
    ScoreManager scoreManager;
    ParticleSystem particleSystem;
    DifficultyManager* difficultyManager;
    AchievementSystem achievementSystem;
    MenuSystem menuSystem;
    
    // UI
    sf::Font font;
    bool fontLoaded;
    std::string playerNameInput;
    bool showAchievements;
    float achievementDisplayTimer;
    
    // Power-up spawning
    float powerUpTimer;
    float powerUpInterval;
    
    // Background stars
    struct Star {
        sf::Vector2f position;
        float size;
        float speed;
        sf::Color color;
    };
    std::vector<Star> stars;
    
    // Screen shake
    float screenShake;
    sf::Vector2f shakeOffset;
    
public:
    GameEngine();
    ~GameEngine();
    
    void run();
    
private:
    // Core loop
    void handleEvents();
    void update();
    void render();
    
    // State updates
    void updatePlaying();
    void updateMenu();
    void updatePaused();
    void updateGameOver();
    void updateLevelTransition();
    
    // State rendering
    void renderPlaying();
    void renderMenu();
    void renderPaused();
    void renderGameOver();
    void renderHighScores();
    void renderLevelTransition();
    void renderHUD();
    void renderAchievementPanel();
    
    // Game management
    void startGame();
    void resetGame();
    void nextLevel();
    void gameOver();
    void pauseGame();
    void resumeGame();
    
    // Entity management
    void spawnPowerUp();
    void spawnResource();
    void cleanupEntities();
    
    // Background
    void initStars();
    void updateStars();
    void renderStars();
    
    // Effects
    void addScreenShake(float intensity);
    void updateScreenShake();
    
    // UI helpers
    void drawText(const std::string& text, float x, float y, 
                  int size, sf:: Color color, bool centered = false);
};

#endif