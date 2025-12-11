#include "ScoreManager.h"
#include <algorithm>
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
                highScores.push_back(entry);
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
    for (const auto& entry : highScores) {
        if (count >= maxScores) break;
        
        file << entry.playerName << ","
             << entry.score << ","
             << entry.timeSeconds << ","
             << entry.date << "\n";
        
        count++;
    }
    
    file.close();
}

void ScoreManager::addScore(const ScoreEntry& entry) {
    highScores.push_back(entry);
    sortScores();
    
    if (highScores.size() > static_cast<size_t>(maxScores)) {
        highScores.resize(maxScores);
    }
    
    try {
        saveScores();
    } catch (const FileException& e) {
        // Silent fail
    }
}

bool ScoreManager::isHighScore(int score) const {
    if (highScores.size() < static_cast<size_t>(maxScores)) {
        return true;
    }
    return score > highScores. back().score;
}

int ScoreManager::getRank(int score) const {
    int rank = 1;
    for (const auto& entry : highScores) {
        if (score > entry.score) {
            return rank;
        }
        rank++;
    }
    return rank;
}

void ScoreManager::sortScores() {
    std::sort(highScores.begin(), highScores.end());
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