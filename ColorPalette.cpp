#include "ColorPalette.h"

// STATIC MEMBER initialization
ColorPalette* ColorPalette::instance = nullptr;

ColorPalette:: ColorPalette() : currentTheme(NEON_CYBER) {
    setTheme(NEON_CYBER);
}

ColorPalette* ColorPalette::getInstance() {
    if (instance == nullptr) {
        instance = new ColorPalette();
    }
    return instance;
}

void ColorPalette:: setTheme(Theme theme) {
    currentTheme = theme;
    colors.clear();
    
    switch(theme) {
        case NEON_CYBER:
            // Cyberpunk neon aesthetic
            colors["player_primary"] = sf::Color(0, 255, 255);      // Cyan
            colors["player_secondary"] = sf::Color(255, 0, 255);    // Magenta
            colors["player_trail"] = sf::Color(0, 200, 255);
            colors["enemy_basic"] = sf::Color(255, 0, 100);         // Hot pink
            colors["enemy_pattern"] = sf::Color(255, 100, 255);     // Pink-purple
            colors["boss"] = sf::Color(255, 0, 255);                // Magenta
            colors["powerup_shield"] = sf::Color(0, 255, 255);      // Cyan
            colors["powerup_fire"] = sf::Color(255, 50, 150);       // Pink
            colors["powerup_repair"] = sf::Color(0, 255, 150);      // Mint
            colors["powerup_speed"] = sf::Color(255, 255, 0);       // Yellow
            colors["explosion_core"] = sf::Color(255, 255, 255);
            colors["explosion_outer"] = sf::Color(255, 0, 200);
            colors["starfield"] = sf::Color(100, 200, 255);
            colors["ui_accent"] = sf::Color(0, 255, 255);
            colors["background"] = sf::Color(10, 5, 20);
            break;
            
        case DEEP_SPACE:
            // Deep space - blues and purples
            colors["player_primary"] = sf::Color(100, 150, 255);
            colors["player_secondary"] = sf::Color(150, 100, 255);
            colors["player_trail"] = sf::Color(120, 140, 255);
            colors["enemy_basic"] = sf::Color(180, 50, 100);
            colors["enemy_pattern"] = sf::Color(150, 80, 200);
            colors["boss"] = sf::Color(120, 50, 180);
            colors["powerup_shield"] = sf::Color(80, 120, 255);
            colors["powerup_fire"] = sf::Color(200, 80, 120);
            colors["powerup_repair"] = sf::Color(80, 200, 180);
            colors["powerup_speed"] = sf::Color(200, 200, 100);
            colors["explosion_core"] = sf::Color(255, 200, 150);
            colors["explosion_outer"] = sf::Color(150, 100, 200);
            colors["starfield"] = sf::Color(180, 180, 255);
            colors["ui_accent"] = sf::Color(120, 150, 255);
            colors["background"] = sf::Color(5, 5, 15);
            break;
            
        case SOLAR_FLARE:
            // Solar - oranges, yellows, reds
            colors["player_primary"] = sf::Color(255, 200, 50);
            colors["player_secondary"] = sf::Color(255, 150, 0);
            colors["player_trail"] = sf::Color(255, 180, 0);
            colors["enemy_basic"] = sf::Color(255, 80, 50);
            colors["enemy_pattern"] = sf::Color(255, 120, 0);
            colors["boss"] = sf::Color(255, 50, 0);
            colors["powerup_shield"] = sf::Color(255, 220, 100);
            colors["powerup_fire"] = sf::Color(255, 100, 50);
            colors["powerup_repair"] = sf:: Color(100, 255, 100);
            colors["powerup_speed"] = sf::Color(255, 255, 150);
            colors["explosion_core"] = sf::Color(255, 255, 200);
            colors["explosion_outer"] = sf::Color(255, 150, 50);
            colors["starfield"] = sf::Color(255, 220, 180);
            colors["ui_accent"] = sf::Color(255, 200, 100);
            colors["background"] = sf::Color(20, 10, 5);
            break;
            
        case TOXIC_WASTE: 
            // Toxic - greens and acid colors
            colors["player_primary"] = sf::Color(150, 255, 50);
            colors["player_secondary"] = sf::Color(100, 255, 150);
            colors["player_trail"] = sf::Color(150, 255, 100);
            colors["enemy_basic"] = sf::Color(200, 50, 50);
            colors["enemy_pattern"] = sf::Color(255, 200, 50);
            colors["boss"] = sf::Color(150, 50, 150);
            colors["powerup_shield"] = sf::Color(100, 255, 200);
            colors["powerup_fire"] = sf::Color(255, 255, 50);
            colors["powerup_repair"] = sf:: Color(150, 255, 150);
            colors["powerup_speed"] = sf::Color(200, 255, 100);
            colors["explosion_core"] = sf::Color(200, 255, 100);
            colors["explosion_outer"] = sf::Color(100, 200, 50);
            colors["starfield"] = sf::Color(150, 255, 150);
            colors["ui_accent"] = sf::Color(150, 255, 100);
            colors["background"] = sf::Color(10, 15, 5);
            break;
            
        case ROYAL_NEBULA:
            // Royal - purples, golds, majestic
            colors["player_primary"] = sf::Color(200, 150, 255);
            colors["player_secondary"] = sf::Color(255, 215, 0);   // Gold
            colors["player_trail"] = sf::Color(220, 180, 255);
            colors["enemy_basic"] = sf::Color(180, 50, 150);
            colors["enemy_pattern"] = sf::Color(150, 100, 200);
            colors["boss"] = sf::Color(120, 50, 200);
            colors["powerup_shield"] = sf::Color(180, 150, 255);
            colors["powerup_fire"] = sf:: Color(255, 100, 150);
            colors["powerup_repair"] = sf:: Color(100, 255, 200);
            colors["powerup_speed"] = sf::Color(255, 215, 100);
            colors["explosion_core"] = sf::Color(255, 230, 180);
            colors["explosion_outer"] = sf::Color(180, 100, 255);
            colors["starfield"] = sf::Color(200, 180, 255);
            colors["ui_accent"] = sf::Color(200, 150, 255);
            colors["background"] = sf::Color(15, 10, 25);
            break;
    }
}

sf::Color ColorPalette::get(const std::string& name) const {
    sf::Color* it = colors.find(name);
    if (it != nullptr) {
 return *it;
    }
    return sf::Color::White; // Fallback
}

sf::Color ColorPalette::blend(const sf::Color& a, const sf::Color& b, float ratio) {
    // Clamp ratio between 0 and 1
    if (ratio < 0.0f) ratio = 0.0f;
    if (ratio > 1.0f) ratio = 1.0f;
    return sf::Color(
        a.r + (b.r - a.r) * ratio,
        a.g + (b.g - a.g) * ratio,
        a.b + (b.b - a.b) * ratio,
        a.a + (b.a - a.a) * ratio
    );
}

sf::Color ColorPalette::lighten(const sf::Color& color, float amount) {
    return blend(color, sf::Color:: White, amount);
}

sf::Color ColorPalette::darken(const sf::Color& color, float amount) {
    return blend(color, sf::Color:: Black, amount);
}

sf::Color ColorPalette::withAlpha(const sf::Color& color, sf::Uint8 alpha) {
    sf::Color result = color;
    result.a = alpha;
    return result;
}

// Themed color getters
sf::Color ColorPalette::getPlayerPrimary() const { return get("player_primary"); }
sf::Color ColorPalette::getPlayerSecondary() const { return get("player_secondary"); }
sf::Color ColorPalette::getEnemyBasicPrimary() const { return get("enemy_basic"); }
sf::Color ColorPalette::getEnemyPatternPrimary() const { return get("enemy_pattern"); }
sf::Color ColorPalette:: getBossPrimary() const { return get("boss"); }
sf::Color ColorPalette::getPowerUpShield() const { return get("powerup_shield"); }
sf::Color ColorPalette::getPowerUpFire() const { return get("powerup_fire"); }
sf::Color ColorPalette::getPowerUpRepair() const { return get("powerup_repair"); }
sf::Color ColorPalette::getPowerUpSpeed() const { return get("powerup_speed"); }
sf::Color ColorPalette::getExplosionCore() const { return get("explosion_core"); }
sf::Color ColorPalette::getExplosionOuter() const { return get("explosion_outer"); }
sf::Color ColorPalette::getStarfield() const { return get("starfield"); }
sf::Color ColorPalette:: getUIAccent() const { return get("ui_accent"); }