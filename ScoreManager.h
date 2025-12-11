#ifndef SCOREMANAGER_H
#define SCOREMANAGER_H

#include <string>
#include "DynamicArray.h"
#include <fstream>
#include "CustomException.h"

struct ScoreEntry {
    std::string playerName;
    int score;
    int timeSeconds;
    std::string date;
    
    ScoreEntry() : score(0), timeSeconds(0) {}
    ScoreEntry(const std::string& name, int s, int t, const std::string& d)
        : playerName(name), score(s), timeSeconds(t), date(d) {}
    
    bool operator<(const ScoreEntry& other) const {
        return score > other.score;
    }
    
    bool operator>(const ScoreEntry& other) const {
        return score < other. score;
    }
};

class ScoreManager {
private: 
    DynamicArray<ScoreEntry> highScores;
    const std::string filename;
    const int maxScores;
    
    static float scoreMultiplier;
    
public:
    ScoreManager(const std::string& file = "scores.txt", int max = 50);
    ~ScoreManager() {}
    
    void loadScores();
    void saveScores();
    void addScore(const ScoreEntry& entry);
    
    const DynamicArray<ScoreEntry>& getHighScores() const { return highScores; }
    bool isHighScore(int score) const;
    int getRank(int score) const;
    
    static void setMultiplier(float mult) { scoreMultiplier = mult; }
    static float getMultiplier() { return scoreMultiplier; }
    static int applyMultiplier(int baseScore) {
        return static_cast<int>(baseScore * scoreMultiplier);
    }
    
private:
    void sortScores();
    std::string getCurrentDate() const;
};

#endif