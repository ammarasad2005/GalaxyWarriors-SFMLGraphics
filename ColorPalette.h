#ifndef COLORPALETTE_H
#define COLORPALETTE_H

#include <SFML/Graphics.hpp>
#include "HashMap.h"
#include <string>

// SINGLETON PATTERN: Global color management
class ColorPalette {
private:
    // STATIC MEMBER:  Single instance
    static ColorPalette* instance;
    
    HashMap<std::string, sf::Color> colors;
    int currentTheme;
    
    // Private constructor for singleton
    ColorPalette();
    
public:
    enum Theme {
        NEON_CYBER,      // Cyberpunk neon colors
        DEEP_SPACE,      // Dark blues and purples
        SOLAR_FLARE,     // Oranges and yellows
        TOXIC_WASTE,     // Greens and acids
        ROYAL_NEBULA     // Purples and golds
    };
    
    // STATIC METHOD: Get singleton instance
    static ColorPalette* getInstance();
    
    void setTheme(Theme theme);
    sf::Color get(const std::string& name) const;
    
    // Utility color functions
    static sf::Color blend(const sf::Color& a, const sf::Color& b, float ratio);
    static sf::Color lighten(const sf::Color& color, float amount);
    static sf::Color darken(const sf::Color& color, float amount);
    static sf::Color withAlpha(const sf::Color& color, sf::Uint8 alpha);
    
    // Get themed colors
    sf::Color getPlayerPrimary() const;
    sf::Color getPlayerSecondary() const;
    sf::Color getEnemyBasicPrimary() const;
    sf::Color getEnemyPatternPrimary() const;
    sf::Color getBossPrimary() const;
    sf::Color getPowerUpShield() const;
    sf::Color getPowerUpFire() const;
    sf::Color getPowerUpRepair() const;
    sf::Color getPowerUpSpeed() const;
    sf::Color getExplosionCore() const;
    sf::Color getExplosionOuter() const;
    sf::Color getStarfield() const;
    sf::Color getUIAccent() const;
    
    Theme getCurrentTheme() const { return static_cast<Theme>(currentTheme); }
};

#endif