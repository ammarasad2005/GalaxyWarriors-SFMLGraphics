#ifndef WAVEMANAGER_H
#define WAVEMANAGER_H

#include "EnemyShip.h"
#include "BasicEnemy.h"
#include "PatternEnemy.h"
#include "BossEnemy.h"
#include "Asteroid.h"
#include "DynamicArray.h"
#include <cstdlib>

class WaveManager {
private:  
    int currentWave;
    int currentLevel;
    float spawnTimer;
    float spawnInterval;
    int enemiesPerWave;
    int enemiesSpawned;
    int enemiesAlive;
    bool waveActive;
    bool bossSpawned;
    
    struct WaveConfig {
        int basicEnemyCount;
        int patternEnemyCount;
        float spawnRate;
        float enemySpeed;
        bool hasBoss;
    };
    
    WaveConfig getWaveConfig() const;
    
public: 
    WaveManager();
    ~WaveManager() {}
    
    void update(float deltaTime, 
                DynamicArray<EnemyShip*>& enemies,
                DynamicArray<Asteroid*>& asteroids);
    
    void startWave();
    void nextWave();
    void setLevel(int level);
    
    int getCurrentWave() const { return currentWave; }
    int getCurrentLevel() const { return currentLevel; }
    bool isWaveComplete() const;
    bool isBossActive() const { return bossSpawned; }
    
    inline bool isActive() const { return waveActive; }
    
private:
    void spawnEnemy(DynamicArray<EnemyShip*>& enemies);
    void spawnAsteroid(DynamicArray<Asteroid*>& asteroids);
};

#endif