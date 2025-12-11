#include "GameEngine.h"
#include "BasicEnemy.h"
#include "PatternEnemy.h"
#include "BossEnemy.h"
#include "ColorPalette.h"
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <algorithm>

GameEngine::GameEngine()
    : window(sf::VideoMode(800, 600), "GALAXY WARS - Enhanced Edition", 
             sf::Style:: Titlebar | sf::Style::Close),
      deltaTime(0),
      currentLevel(1),
      gameTime(0),
      levelTransitionTimer(0),
      player(nullptr),
      waveManager(),
      scoreManager(),
      particleSystem(500),
      fontLoaded(false),
      playerNameInput(""),
      showAchievements(false),
      achievementDisplayTimer(0),
      powerUpTimer(0),
      powerUpInterval(10.0f),
      screenShake(0) {
    
    window.setFramerateLimit(60);
    srand(static_cast<unsigned>(time(nullptr)));
    
    fontLoaded = font.loadFromFile("arial.ttf");
    
    difficultyManager = DifficultyManager::getInstance();
    achievementSystem. load("achievements.dat");
    
    initStars();
    
    ColorPalette::getInstance()->setTheme(ColorPalette:: NEON_CYBER);
    
    gameState. setState(GameState::MAIN_MENU);
}

GameEngine::~GameEngine() {
    delete player;
    
    for (auto* enemy : enemies) delete enemy;
    for (auto* projectile : projectiles) delete projectile;
    for (auto* asteroid : asteroids) delete asteroid;
    for (auto* collectible : collectibles) delete collectible;
    
    achievementSystem.save("achievements.dat");
}

void GameEngine:: run() {
    while (window.isOpen()) {
        deltaTime = clock.restart().asSeconds();
        if (deltaTime > 0.1f) deltaTime = 0.1f;
        
        handleEvents();
        update();
        render();
    }
}

void GameEngine::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        // Mouse input
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse:: Left) {
                float mouseX = event.mouseButton.x;
                float mouseY = event.mouseButton.y;
                
                if (gameState.getState() == GameState::MAIN_MENU) {
                    int action = menuSystem.handleClick(mouseX, mouseY);
                    
                    if (menuSystem.isInThemeSelection()) {
                        if (action == -1) {
                            menuSystem.setThemeSelection(false);
                        }
                    } else {
                        switch(action) {
                            case 1: 
                                startGame();
                                break;
                            case 2:
                                menuSystem.setThemeSelection(true);
                                break;
                            case 3:
                                gameState.setState(GameState::HIGH_SCORES);
                                break;
                            case 4:
                                window.close();
                                break;
                        }
                    }
                }
            }
        }
        
        if (event.type == sf::Event::MouseMoved) {
            if (gameState.getState() == GameState::MAIN_MENU) {
                menuSystem.handleHover(event. mouseMove.x, event.mouseMove.y);
            }
        }
        
        // Keyboard input
        if (event.type == sf::Event::KeyPressed) {
            switch (gameState.getState()) {
                case GameState::MAIN_MENU:
                    if (menuSystem.isInThemeSelection()) {
                        if (event.key.code == sf::Keyboard::Escape) {
                            menuSystem.setThemeSelection(false);
                        }
                    } else {
                        if (event.key.code == sf::Keyboard:: Num1) {
                            startGame();
                        } else if (event.key.code == sf::Keyboard::Num2) {
                            menuSystem.setThemeSelection(true);
                        } else if (event.key.code == sf:: Keyboard::Num3) {
                            gameState.setState(GameState::HIGH_SCORES);
                        } else if (event. key.code == sf::Keyboard::Num4) {
                            window.close();
                        }
                    }
                    break;
                    
                case GameState:: PLAYING:
                    if (event.key.code == sf:: Keyboard::Escape) {
                        pauseGame();
                    } else if (event.key. code == sf::Keyboard::Space) {
                        if (player && player->isActive()) {
                            player->shoot(projectiles);
                            achievementSystem.onShotFired();
                        }
                    } else if (event.key.code == sf::Keyboard::Tab) {
                        showAchievements = !showAchievements;
                    }
                    break;
                    
                case GameState::PAUSED:
                    if (event.key.code == sf:: Keyboard::Escape) {
                        resumeGame();
                    } else if (event.key.code == sf::Keyboard::R) {
                        resetGame();
                        startGame();
                    } else if (event.key.code == sf::Keyboard::Q) {
                        resetGame();
                        gameState.setState(GameState:: MAIN_MENU);
                    }
                    break;
                    
                case GameState::GAME_OVER:
                    if (event.key.code == sf::Keyboard::Return && ! playerNameInput.empty()) {
                        ScoreEntry entry(
                            playerNameInput,
                            player ?  player->getScore() : 0,
                            static_cast<int>(gameTime),
                            ""
                        );
                        scoreManager.addScore(entry);
                        gameState.setState(GameState::HIGH_SCORES);
                    } else if (event.key.code == sf::Keyboard::Escape) {
                        resetGame();
                        gameState. setState(GameState::MAIN_MENU);
                    }
                    break;
                    
                case GameState::HIGH_SCORES:
                    if (event.key.code == sf:: Keyboard:: Escape) {
                        gameState.setState(GameState::MAIN_MENU);
                    }
                    break;
                    
                default:
                    break;
            }
        }
        
        // Text input for name
        if (gameState.getState() == GameState::GAME_OVER) {
            if (event.type == sf::Event::TextEntered) {
                if (event. text.unicode == 8) {
                    if (! playerNameInput.empty()) {
                        playerNameInput.pop_back();
                    }
                } else if (event.text. unicode < 128 && playerNameInput.length() < 15) {
                    char c = static_cast<char>(event.text.unicode);
                    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || 
                        (c >= '0' && c <= '9') || c == ' ') {
                        playerNameInput += c;
                    }
                }
            }
        }
    }
}

void GameEngine::update() {
    switch (gameState.getState()) {
        case GameState::PLAYING:
            updatePlaying();
            break;
        case GameState::PAUSED: 
            updatePaused();
            break;
        case GameState::GAME_OVER: 
            updateGameOver();
            break;
        case GameState:: LEVEL_TRANSITION:
            updateLevelTransition();
            break;
        case GameState:: MAIN_MENU:
            updateMenu();
            break;
        default:
            break;
    }
    
    updateStars();
    updateScreenShake();
}

void GameEngine::updatePlaying() {
    gameTime += deltaTime;
    
    achievementSystem.update(deltaTime);
    menuSystem.update(deltaTime);
    
    if (player && player->isActive()) {
        player->handleInput(deltaTime);
        player->update(deltaTime);
        
        float healthPercent = player->getHealth() / player->getMaxHealth();
        difficultyManager->updateAdaptive(healthPercent, player->getScore());
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            player->shoot(projectiles);
            achievementSystem.onShotFired();
        }
        
        particleSystem.createTrail(player->getPosition(), 
            ColorPalette::getInstance()->getPlayerPrimary());
    } else if (player && ! player->isActive()) {
        difficultyManager->onPlayerDeath();
        gameOver();
        return;
    }
    
    // Update enemies
    for (auto* enemy : enemies) {
        if (enemy && enemy->isActive()) {
            enemy->update(deltaTime);
            
            BossEnemy* boss = dynamic_cast<BossEnemy*>(enemy);
            if (boss) {
                if (boss->shouldShoot()) {
                    boss->shoot(projectiles);
                }
                
                if (boss->isInTransition()) {
                    addScreenShake(0.3f);
                    if (rand() % 100 < 20) {
                        particleSystem.createNebula(
                            boss->getPosition(),
                            boss->getColor()
                        );
                    }
                }
            } else {
                if (enemy->shouldShoot()) {
                    enemy->shoot(projectiles);
                }
            }
        }
    }
    
    // Update projectiles
    for (auto* projectile : projectiles) {
        if (projectile && projectile->isActive()) {
            projectile->update(deltaTime);
        }
    }
    
    // Update asteroids
    for (auto* asteroid : asteroids) {
        if (asteroid && asteroid->isActive()) {
            asteroid->update(deltaTime);
        }
    }
    
    // Update collectibles
    for (auto* collectible : collectibles) {
        if (collectible && collectible->isActive()) {
            collectible->update(deltaTime);
        }
    }
    
    // Wave manager
    waveManager.update(deltaTime, enemies, asteroids);
    
    // Track kills
    size_t enemiesBeforeCollision = 0;
    for (auto* enemy : enemies) {
        if (enemy && enemy->isActive()) enemiesBeforeCollision++;
    }
    
    // Collision detection
    CollisionManager::checkAllCollisions(
        player, enemies, projectiles, asteroids, collectibles, particleSystem
    );
    
    size_t enemiesAfterCollision = 0;
    for (auto* enemy : enemies) {
        if (enemy && enemy->isActive()) enemiesAfterCollision++;
    }
    
    int killsThisFrame = enemiesBeforeCollision - enemiesAfterCollision;
    for (int i = 0; i < killsThisFrame; i++) {
        achievementSystem.onEnemyKilled();
        difficultyManager->onEnemyKilled();
    }
    
    // Check boss defeat
    for (auto* enemy : enemies) {
        BossEnemy* boss = dynamic_cast<BossEnemy*>(enemy);
        if (boss && boss->isDead()) {
            achievementSystem.onBossDefeated();
            
            for (int i = 0; i < 10; i++) {
                particleSystem.createSpiralExplosion(
                    boss->getPosition(),
                    boss->getColor()
                );
            }
            particleSystem.createShockwave(
                boss->getPosition(),
                boss->getColor(),
                150.0f
            );
            addScreenShake(1.0f);
        }
    }
    
    // Check wave complete
    if (waveManager. isWaveComplete() && ! waveManager.isActive()) {
        if (currentLevel == 1 && player && player->getScore() >= 1000) {
            nextLevel();
        } else if (currentLevel == 2 && player && player->getScore() >= 3000) {
            nextLevel();
        } else if (currentLevel == 3 && waveManager.getCurrentWave() >= 3) {
            achievementSystem.onLevelCompleted(3);
            gameOver();
        } else {
            waveManager.nextWave();
        }
    }
    
    // Spawn power-ups
    powerUpTimer += deltaTime;
    if (powerUpTimer >= powerUpInterval) {
        powerUpTimer = 0;
        if (rand() % 2 == 0) {
            spawnPowerUp();
        } else {
            spawnResource();
        }
    }
    
    particleSystem.update(deltaTime);
    cleanupEntities();
}

void GameEngine::updateMenu() {
    menuSystem.update(deltaTime);
}

void GameEngine::updatePaused() {
    // Nothing
}

void GameEngine::updateGameOver() {
    particleSystem.update(deltaTime);
}

void GameEngine::updateLevelTransition() {
    levelTransitionTimer += deltaTime;
    
    if (levelTransitionTimer >= 3.0f) {
        gameState.setState(GameState:: PLAYING);
        waveManager.startWave();
    }
}

void GameEngine:: render() {
    window.clear(sf::Color(10, 10, 20));
    
    sf::View view = window.getDefaultView();
    view.setCenter(400 + shakeOffset. x, 300 + shakeOffset.y);
    window.setView(view);
    
    renderStars();
    
    switch (gameState.getState()) {
        case GameState::PLAYING:
            renderPlaying();
            break;
        case GameState::MAIN_MENU:
            renderMenu();
            break;
        case GameState::PAUSED: 
            renderPlaying();
            renderPaused();
            break;
        case GameState::GAME_OVER:
            renderPlaying();
            renderGameOver();
            break;
        case GameState::HIGH_SCORES: 
            renderHighScores();
            break;
        case GameState::LEVEL_TRANSITION:
            renderPlaying();
            renderLevelTransition();
            break;
        default:
            break;
    }
    
    window.display();
}

void GameEngine:: renderPlaying() {
    particleSystem.render(window);
    
    for (auto* asteroid : asteroids) {
        if (asteroid && asteroid->isActive()) {
            asteroid->render(window);
        }
    }
    
    for (auto* collectible : collectibles) {
        if (collectible && collectible->isActive()) {
            collectible->render(window);
        }
    }
    
    for (auto* projectile : projectiles) {
        if (projectile && projectile->isActive()) {
            projectile->render(window);
        }
    }
    
    for (auto* enemy : enemies) {
        if (enemy && enemy->isActive()) {
            enemy->render(window);
        }
    }
    
    if (player && player->isActive()) {
        player->render(window);
    }
    
    if (achievementSystem.getComboSystem().getCurrentCombo() > 0) {
        achievementSystem.renderComboCounter(window, sf::Vector2f(400, 500));
    }
    
    achievementSystem.renderNotifications(window);
    
    if (showAchievements) {
        renderAchievementPanel();
    }
    
    renderHUD();
}

void GameEngine::renderMenu() {
    if (menuSystem.isInThemeSelection()) {
        menuSystem.renderThemeSelection(window, fontLoaded, font);
    } else {
        menuSystem.render(window, fontLoaded, font);
    }
}

void GameEngine:: renderPaused() {
    sf::RectangleShape overlay(sf::Vector2f(800, 600));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);
    
    drawText("PAUSED", 400, 200, 60, sf::Color::Yellow, true);
    drawText("[ESC] RESUME", 400, 300, 20, sf::Color::White, true);
    drawText("[R] RESTART", 400, 340, 20, sf::Color:: White, true);
    drawText("[Q] QUIT TO MENU", 400, 380, 20, sf::Color:: White, true);
}

void GameEngine::renderGameOver() {
    sf::RectangleShape overlay(sf::Vector2f(800, 600));
    overlay.setFillColor(sf::Color(0, 0, 30, 200));
    window.draw(overlay);
    
    bool victory = (currentLevel == 3 && waveManager.getCurrentWave() >= 3);
    
    if (victory) {
        drawText("VICTORY!", 400, 150, 70, sf::Color::Green, true);
        drawText("You saved the galaxy!", 400, 220, 25, sf::Color(100, 255, 100), true);
    } else {
        drawText("GAME OVER", 400, 150, 70, sf:: Color::Red, true);
    }
    
    if (player) {
        std::stringstream ss;
        ss << "Final Score: " << player->getScore();
        drawText(ss. str(), 400, 280, 30, sf::Color::White, true);
        
        ss.str("");
        ss << "Level Reached: " << currentLevel;
        drawText(ss.str(), 400, 320, 25, sf::Color:: Cyan, true);
        
        ss.str("");
        int minutes = static_cast<int>(gameTime) / 60;
        int seconds = static_cast<int>(gameTime) % 60;
        ss << "Time: " << minutes << ":" << std::setfill('0') << std::setw(2) << seconds;
        drawText(ss.str(), 400, 350, 25, sf::Color::Yellow, true);
    }
    
    drawText("Enter Your Name:", 400, 400, 20, sf::Color::White, true);
    
    sf::RectangleShape inputBox(sf::Vector2f(300, 40));
    inputBox.setPosition(250, 430);
    inputBox.setFillColor(sf::Color(50, 50, 80));
    inputBox.setOutlineColor(sf::Color:: Cyan);
    inputBox.setOutlineThickness(2);
    window.draw(inputBox);
    
    drawText(playerNameInput + "_", 400, 440, 25, sf::Color::White, true);
    
    drawText("[ENTER] to Save Score", 400, 500, 18, sf::Color(150, 150, 150), true);
    drawText("[ESC] to Skip", 400, 530, 18, sf::Color(150, 150, 150), true);
}

void GameEngine::renderHighScores() {
    drawText("HIGH SCORES", 400, 50, 50, sf::Color::Yellow, true);
    
    const auto& scores = scoreManager.getHighScores();
    int displayCount = std::min(10, static_cast<int>(scores. size()));
    
    float startY = 120;
    for (int i = 0; i < displayCount; i++) {
        float y = startY + i * 40;
        
        std::stringstream ss;
        ss << (i + 1) << ".";
        drawText(ss.str(), 100, y, 20, sf::Color::White, false);
        
        drawText(scores[i].playerName, 150, y, 20, sf:: Color:: Cyan, false);
        
        ss.str("");
        ss << scores[i].score;
        drawText(ss.str(), 450, y, 20, sf:: Color::Yellow, false);
        
        int minutes = scores[i].timeSeconds / 60;
        int seconds = scores[i].timeSeconds % 60;
        ss.str("");
        ss << minutes << ":" << std::setfill('0') << std::setw(2) << seconds;
        drawText(ss.str(), 600, y, 18, sf::Color::Green, false);
    }
    
    if (scores.empty()) {
        drawText("No scores yet!", 400, 300, 30, sf::Color(150, 150, 150), true);
    }
    
    drawText("[ESC] Back to Menu", 400, 560, 18, sf::Color:: White, true);
}

void GameEngine::renderLevelTransition() {
    sf::RectangleShape overlay(sf::Vector2f(800, 600));
    overlay.setFillColor(sf::Color(0, 0, 50, 220));
    window.draw(overlay);
    
    drawText("LEVEL COMPLETE!", 400, 200, 50, sf::Color::Green, true);
    
    std::stringstream ss;
    ss << "Entering Level " << currentLevel;
    drawText(ss.str(), 400, 280, 35, sf::Color:: Cyan, true);
    
    std::string description;
    if (currentLevel == 2) {
        description = "Enemy Territory - Increased Difficulty";
    } else if (currentLevel == 3) {
        description = "Final Assault - Prepare for Boss Battle! ";
    }
    drawText(description, 400, 340, 20, sf::Color::Yellow, true);
    
    float progress = levelTransitionTimer / 3.0f;
    sf::RectangleShape progressBg(sf::Vector2f(400, 20));
    progressBg.setPosition(200, 400);
    progressBg.setFillColor(sf::Color(50, 50, 50));
    window.draw(progressBg);
    
    sf::RectangleShape progressBar(sf::Vector2f(400 * progress, 20));
    progressBar.setPosition(200, 400);
    progressBar.setFillColor(sf::Color:: Cyan);
    window.draw(progressBar);
}

void GameEngine::renderHUD() {
    window.setView(window.getDefaultView());
    
    if (player) {
        std::stringstream ss;
        ss << "SCORE: " << player->getScore();
        drawText(ss.str(), 10, 10, 20, sf::Color::Yellow, false);
        
        ss.str("");
        ss << "LEVEL: " << currentLevel << "  WAVE: " << waveManager.getCurrentWave();
        drawText(ss.str(), 10, 35, 18, sf::Color:: Cyan, false);
        
        drawText("HEALTH", 650, 10, 18, sf::Color::White, false);
        
        float healthPercent = player->getHealth() / player->getMaxHealth();
        sf::RectangleShape healthBg(sf::Vector2f(130, 20));
        healthBg.setPosition(650, 35);
        healthBg.setFillColor(sf::Color(50, 50, 50));
        healthBg.setOutlineColor(sf::Color::White);
        healthBg.setOutlineThickness(2);
        window.draw(healthBg);
        
        sf::RectangleShape healthBar(sf::Vector2f(130 * healthPercent, 20));
        healthBar.setPosition(650, 35);
        
        if (healthPercent > 0.6f) {
            healthBar. setFillColor(sf::Color::Green);
        } else if (healthPercent > 0.3f) {
            healthBar.setFillColor(sf:: Color::Yellow);
        } else {
            healthBar.setFillColor(sf::Color::Red);
        }
        
        window.draw(healthBar);
        
        ss.str("");
        ss << "LIVES: " << player->getLives();
        drawText(ss.str(), 650, 60, 16, sf::Color::White, false);
        
        float powerUpY = 100;
        if (player->hasShield()) {
            drawText("SHIELD ACTIVE", 650, powerUpY, 14, sf::Color(0, 150, 255), false);
            powerUpY += 20;
        }
        if (player->hasDoubleFire()) {
            drawText("DOUBLE FIRE", 650, powerUpY, 14, sf::Color(255, 50, 50), false);
        }
    }
    
    if (deltaTime > 0) {
        int fps = static_cast<int>(1.0f / deltaTime);
        std::stringstream ss;
        ss << "FPS: " << fps;
        drawText(ss. str(), 10, 570, 14, sf::Color(100, 100, 100), false);
    }
}

void GameEngine::renderAchievementPanel() {
    sf::RectangleShape panel(sf::Vector2f(350, 550));
    panel.setPosition(425, 25);
    panel.setFillColor(sf::Color(20, 20, 40, 230));
    panel.setOutlineColor(ColorPalette::getInstance()->getUIAccent());
    panel.setOutlineThickness(3);
    window.draw(panel);
    
    drawText("ACHIEVEMENTS", 600, 40, 24, sf::Color::Yellow, true);
    
    int unlocked = achievementSystem.getUnlockedCount();
    int total = achievementSystem.getTotalCount();
    std::stringstream ss;
    ss << unlocked << " / " << total << " Unlocked";
    drawText(ss.str(), 600, 70, 16, sf::Color::White, true);
    
    drawText("[TAB] to close", 600, 565, 14, sf::Color(150, 150, 150), true);
}

void GameEngine::startGame() {
    resetGame();
    
    player = new PlayerShip(400, 500);
    
    waveManager.setLevel(currentLevel);
    waveManager.startWave();
    
    gameState.setState(GameState:: PLAYING);
    gameTime = 0;
}

void GameEngine::resetGame() {
    delete player;
    player = nullptr;
    
    for (auto* enemy : enemies) delete enemy;
    enemies.clear();
    
    for (auto* projectile : projectiles) delete projectile;
    projectiles.clear();
    
    for (auto* asteroid : asteroids) delete asteroid;
    asteroids.clear();
    
    for (auto* collectible : collectibles) delete collectible;
    collectibles.clear();
    
    currentLevel = 1;
    gameTime = 0;
    powerUpTimer = 0;
    playerNameInput = "";
    showAchievements = false;
    
    waveManager = WaveManager();
    difficultyManager->reset();
    CollisionManager::resetCollisions();
}

void GameEngine::nextLevel() {
    currentLevel++;
    
    if (currentLevel > 3) {
        achievementSystem.onLevelCompleted(3);
        gameOver();
        return;
    }
    
    for (auto* enemy : enemies) delete enemy;
    enemies.clear();
    
    for (auto* projectile : projectiles) delete projectile;
    projectiles.clear();
    
    waveManager.setLevel(currentLevel);
    difficultyManager->onLevelComplete();
    achievementSystem.onLevelCompleted(currentLevel - 1);
    
    gameState.setState(GameState::LEVEL_TRANSITION);
    levelTransitionTimer = 0;

    if (player) {
        player->heal(50.0f);
    }
}

void GameEngine::gameOver() {
    gameState.setState(GameState::GAME_OVER);
    playerNameInput = "";
}

void GameEngine::pauseGame() {
    if (gameState.getState() == GameState::PLAYING) {
        gameState.setState(GameState::PAUSED);
    }
}

void GameEngine::resumeGame() {
    if (gameState.getState() == GameState::PAUSED) {
        gameState.setState(GameState::PLAYING);
    }
}

void GameEngine::spawnPowerUp() {
    float x = 100 + (rand() % 600);
    float y = -50;
    
    PowerUp:: Type type = static_cast<PowerUp::Type>(rand() % 4);
    
    collectibles.push_back(new PowerUp(x, y, type));
}

void GameEngine::spawnResource() {
    float x = 100 + (rand() % 600);
    float y = -50;
    
    Resource:: Type type = static_cast<Resource::Type>(rand() % 3);
    
    collectibles. push_back(new Resource(x, y, type));
}

void GameEngine::cleanupEntities() {
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](EnemyShip* e) {
                if (! e->isActive()) {
                    delete e;
                    return true;
                }
                return false;
            }),
        enemies.end()
    );
    
    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
            [](Projectile* p) {
                if (!p->isActive()) {
                    delete p;
                    return true;
                }
                return false;
            }),
        projectiles.end()
    );
    
    asteroids.erase(
        std:: remove_if(asteroids.begin(), asteroids.end(),
            [](Asteroid* a) {
                if (!a->isActive()) {
                    delete a;
                    return true;
                }
                return false;
            }),
        asteroids.end()
    );
    
    collectibles.erase(
        std::remove_if(collectibles. begin(), collectibles.end(),
            [](Collectible* c) {
                if (!c->isActive()) {
                    delete c;
                    return true;
                }
                return false;
            }),
        collectibles. end()
    );
}

void GameEngine::initStars() {
    for (int layer = 0; layer < 3; layer++) {
        int starCount = 50 + layer * 30;
        float baseSpeed = 20.0f + layer * 30.0f;
        
        for (int i = 0; i < starCount; i++) {
            Star star;
            star.position = sf::Vector2f(rand() % 800, rand() % 600);
            star.size = 1.0f + (rand() % 3) * 0.5f;
            star.speed = baseSpeed + (rand() % 20);
            
            int brightness = 150 + rand() % 106;
            if (layer == 0) {
                brightness = 100 + rand() % 50;
            }
            star.color = sf::Color(brightness, brightness, brightness + 20);
    
            stars.push_back(star);
        }
    }
}

void GameEngine::updateStars() {
    for (auto& star : stars) {
        star.position. y += star.speed * deltaTime;
        
        if (star.position.y > 600) {
            star.position.y = 0;
            star.position.x = rand() % 800;
        }
    }
}

void GameEngine::renderStars() {
    for (const auto& star : stars) {
        sf::CircleShape starShape(star.size);
        starShape.setPosition(star.position);
        starShape.setFillColor(star.color);
        window.draw(starShape);
    }
}

void GameEngine:: addScreenShake(float intensity) {
    screenShake = intensity;
}

void GameEngine::updateScreenShake() {
    if (screenShake > 0) {
        shakeOffset. x = (rand() % 20 - 10) * screenShake;
        shakeOffset.y = (rand() % 20 - 10) * screenShake;
        
        screenShake -= deltaTime * 5.0f;
        if (screenShake < 0) screenShake = 0;
    } else {
        shakeOffset = sf::Vector2f(0, 0);
    }
}

void GameEngine::drawText(const std::string& text, float x, float y, 
                          int size, sf::Color color, bool centered) {
    if (fontLoaded) {
        sf::Text sfText;
        sfText.setFont(font);
        sfText.setString(text);
        sfText.setCharacterSize(size);
        sfText.setFillColor(color);
        
        if (centered) {
            sf::FloatRect bounds = sfText.getLocalBounds();
            sfText.setOrigin(bounds.width / 2, bounds.height / 2);
        }
        
        sfText.setPosition(x, y);
        window.draw(sfText);
    } else {
        sf::RectangleShape textBox(sf::Vector2f(text.length() * size * 0.6f, size));
        textBox.setFillColor(sf::Color(50, 50, 50, 100));
        textBox.setOutlineColor(color);
        textBox.setOutlineThickness(1);
        
        if (centered) {
            textBox.setPosition(x - textBox.getSize().x / 2, y);
        } else {
            textBox.setPosition(x, y);
        }
        
        window.draw(textBox);
    }
}