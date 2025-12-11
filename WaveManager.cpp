#include "WaveManager.h"
#include <ctime>

WaveManager::WaveManager() 
    : currentWave(0),
      currentLevel(1),
      spawnTimer(0),
      spawnInterval(2.0f),
      enemiesPerWave(5),
      enemiesSpawned(0),
      enemiesAlive(0),
      waveActive(false),
      bossSpawned(false) {
    
    srand(static_cast<unsigned>(time(nullptr)));
}

WaveManager::WaveConfig WaveManager::getWaveConfig() const {
    WaveConfig config;
    
    if (currentLevel == 1) {
        config.basicEnemyCount = 5 + currentWave * 2;
        config.patternEnemyCount = 0;
        config.spawnRate = 2.5f;
        config.enemySpeed = 1.0f;
        config.hasBoss = false;
    }
    else if (currentLevel == 2) {
        config.basicEnemyCount = 4 + currentWave;
        config.patternEnemyCount = 2 + currentWave;
        config.spawnRate = 2.0f;
        config. enemySpeed = 1.3f;
        config.hasBoss = false;
    }
    else {
        config.basicEnemyCount = 2 + currentWave;
        config.patternEnemyCount = 4 + currentWave * 2;
        config.spawnRate = 1.5f;
        config.enemySpeed = 1.6f;
        config.hasBoss = (currentWave == 3);
    }
    
    return config;
}

void WaveManager::update(float deltaTime, 
                         std::vector<EnemyShip*>& enemies,
                         std::vector<Asteroid*>& asteroids) {
    if (!waveActive) return;
    
    enemiesAlive = 0;
    for (auto* enemy : enemies) {
        if (enemy && enemy->isActive()) {
            enemiesAlive++;
        }
    }
    
    WaveConfig config = getWaveConfig();
    int totalEnemies = config.basicEnemyCount + config.patternEnemyCount;
    if (config.hasBoss) totalEnemies++;
    
    if (enemiesSpawned < totalEnemies) {
        spawnTimer += deltaTime;
        
        if (spawnTimer >= config.spawnRate) {
            spawnTimer = 0;
            spawnEnemy(enemies);
            enemiesSpawned++;
        }
    }
    
    if (rand() % 100 < 2) {
        spawnAsteroid(asteroids);
    }
    
    if (enemiesSpawned >= totalEnemies && enemiesAlive == 0) {
        waveActive = false;
        bossSpawned = false;
    }
}

void WaveManager::startWave() {
    waveActive = true;
    enemiesSpawned = 0;
    spawnTimer = 0;
    currentWave++;
}

void WaveManager::nextWave() {
    startWave();
}

void WaveManager::setLevel(int level) {
    currentLevel = level;
    currentWave = 0;
}

bool WaveManager::isWaveComplete() const {
    WaveConfig config = getWaveConfig();
    int totalEnemies = config.basicEnemyCount + config.patternEnemyCount;
    if (config.hasBoss) totalEnemies++;
    
    return enemiesSpawned >= totalEnemies && enemiesAlive == 0;
}

void WaveManager::spawnEnemy(std::vector<EnemyShip*>& enemies) {
    WaveConfig config = getWaveConfig();
    
    float x = 100 + (rand() % 600);
    float y = -50;
    
    if (config.hasBoss && ! bossSpawned && enemiesSpawned == 0) {
        enemies.push_back(new BossEnemy(400, 50));
        bossSpawned = true;
        return;
    }
    
    int remainingBasic = config.basicEnemyCount - 
        std::min(enemiesSpawned, config.basicEnemyCount);
    int remainingPattern = config.patternEnemyCount;
    
    if (remainingPattern > 0 && (remainingBasic == 0 || rand() % 2 == 0)) {
        enemies.push_back(new PatternEnemy(x, y));
    } else {
        enemies.push_back(new BasicEnemy(x, y));
    }
}

void WaveManager::spawnAsteroid(std::vector<Asteroid*>& asteroids) {
    float x = 50 + (rand() % 700);
    float y = -50;
    
    int type = 0;
    if (currentLevel >= 2) {
        type = rand() % 2;
    }
    if (currentLevel >= 3) {
        type = rand() % 3;
    }
    
    asteroids.push_back(new Asteroid(x, y, type));
}