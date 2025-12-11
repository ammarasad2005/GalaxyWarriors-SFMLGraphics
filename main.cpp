#include "GameEngine.h"
#include "CustomException.h"
#include <iostream>

int main() {
    std::cout << "=================================================\n";
    std::cout << "||        GALAXY WARS - Enhanced Edition  ||\n";
    std::cout << "||        OOP Final Project 2025     ||\n";
    std::cout << "================================================\n\n";
    
    std::cout << "Initializing game systems...\n";
    
    try {
        // EXCEPTION HANDLING:  Wrap game in try-catch
        GameEngine game;
     
        std::cout << " Game initialized successfully!\n";
        std::cout << " Color palette system loaded\n";
        std::cout << " Achievement system ready\n";
        std::cout << " Physics engine active\n\n";
        
        std::cout << "=================== CONTROLS ==================\n";
        std::cout << "  Movement:   WASD / Arrow Keys\n";
        std::cout << "  Shoot:     SPACE (hold for auto-fire)\n";
        std::cout << "  Pause:     ESC\n";
        std::cout << "  Menu:      1/2/3/4 (number keys)\n";
        std::cout << "  Achievements: TAB (during gameplay)\n";
        std::cout << "===================================\n\n";
        
        std::cout << "========== GAME OBJECTIVES ===========\n";
        std::cout << "  Level 1 (Asteroid Belt):  1,000 points\n";
        std::cout << "  Level 2 (Enemy Territory): 3,000 points\n";
        std::cout << "  Level 3 (Final Assault):   Defeat Boss!\n";
        std::cout << "======================================\n\n";
        
        std::cout << "============== FEATURES ================\n";
        std::cout << "   5 Visual Themes\n";
        std::cout << "   12 Achievements\n";
        std::cout << "   Combo System\n";
        std::cout << "   Boss Battle\n";
        std::cout << "   Adaptive Difficulty\n";
        std::cout << "   High Score Leaderboard\n";
        std::cout << "==========================================\n\n";
        
        std::cout << "Starting main loop...\n\n";
        
        // Run the game
        game.run();
        
        std::cout << "\n====================================\n";
        std::cout << "Game closed successfully!\n";
        std::cout << "Thank you for playing GALAXY WARS!\n";
        std::cout << "======================================\n";
  
    } catch (const GameException& e) {
        // EXCEPTION HANDLING: Custom exceptions
        std::cerr << "\n GAME ERROR: " << e.what() << std::endl;
        std::cerr << "Please check your installation and try again.\n";
        return 1;
        
    } catch (const std::exception& e) {
        // EXCEPTION HANDLING: Standard exceptions
        std::cerr << "\n CRITICAL ERROR: " << e.what() << std::endl;
        std::cerr << "The game encountered an unexpected error.\n";
        return 1;
        
    } catch (...) {
        // EXCEPTION HANDLING:  Unknown exceptions
        std::cerr << "\n UNKNOWN ERROR occurred!\n";
        std::cerr << "Please restart the application.\n";
        return 1;
    }
    
    return 0;
}