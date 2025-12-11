#include "ScoreManager.h"
#include <sstream>
#include <ctime>
#include <iomanip>

float ScoreManager::scoreMultiplier = 1.0f;

ScoreManager::ScoreManager(const std::string& file, int max)
    : filename(file), maxScores(max) {
    
    try {
        loadScores();
    } catch (const FileException& e) {
        highScores.clear();
    }
}

void ScoreManager::loadScores() {
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        throw FileException(filename);
    }
    
    highScores.clear();
    std::string line;
    
    if (std::getline(file, line)) {
        if (line.find("PlayerName") != std::string::npos) {
            // Skip header
        } else {
            file. seekg(0);
        }
    }
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        try {
            std::stringstream ss(line);
            std::string name, scoreStr, timeStr, date;
            
            std::getline(ss, name, ',');
            std::getline(ss, scoreStr, ',');
            std::getline(ss, timeStr, ',');
            std::getline(ss, date, ',');
            
            if (! name.empty() && !scoreStr.empty()) {
                ScoreEntry entry(
                    name,
                    std::stoi(scoreStr),
                    std:: stoi(timeStr),
                    date
                );
                highScores.pushBack(entry);
            }
        } catch (const std::exception& e) {
            continue;
        }
    }
    
    file.close();
    sortScores();
}

void ScoreManager::saveScores() {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        throw FileException(filename);
    }
    
    file << "PlayerName,Score,Time,Date\n";
    
    int count = 0;
    for (int i = 0; i < highScores.getSize(); i++) {
        if (count >= maxScores) break;
        
        file << highScores[i].playerName << ","
             << highScores[i].score << ","
             << highScores[i].timeSeconds << ","
             << highScores[i].date << "\n";
        
        count++;
    }
    
    file.close();
}

void ScoreManager::addScore(const ScoreEntry& entry) {
    highScores.pushBack(entry);
    sortScores();
    
    // Remove excess scores
    while (highScores.getSize() > maxScores) {
        highScores.removeAt(highScores.getSize() - 1);
    }
    
    try {
        saveScores();
    } catch (const FileException& e) {
        // Silent fail
    }
}

bool ScoreManager::isHighScore(int score) const {
    if (highScores.getSize() < maxScores) {
        return true;
    }
    return score > highScores[highScores.getSize() - 1].score;
}

int ScoreManager::getRank(int score) const {
    int rank = 1;
    for (int i = 0; i < highScores.getSize(); i++) {
        if (score > highScores[i].score) {
            return rank;
        }
        rank++;
    }
    return rank;
}

void ScoreManager::sortScores() {
    // Simple bubble sort for high scores (small dataset)
    for (int i = 0; i < highScores.getSize() - 1; i++) {
        for (int j = 0; j < highScores.getSize() - i - 1; j++) {
            if (highScores[j].score < highScores[j + 1].score) {
                // Swap
                ScoreEntry temp = highScores[j];
                highScores[j] = highScores[j + 1];
                highScores[j + 1] = temp;
            }
        }
    }
}

std::string ScoreManager::getCurrentDate() const {
    time_t now = time(nullptr);
    tm timeInfo;
#ifdef _WIN32
    localtime_s(&timeInfo, &now);
    tm* ltm = &timeInfo;
#else
    tm* ltm = localtime_r(&now, &timeInfo);
#endif
    
    std::stringstream ss;
    ss << std::setfill('0')
       << std::setw(4) << (1900 + ltm->tm_year) << "-"
       << std::setw(2) << (1 + ltm->tm_mon) << "-"
       << std::setw(2) << ltm->tm_mday;
    
    return ss.str();
}