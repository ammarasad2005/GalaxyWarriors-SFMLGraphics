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
    
    if (theme == NEON_CYBER) {
        // Cyberpunk neon aesthetic
        player_primary = sf::Color(0, 255, 255);      // Cyan
        player_secondary = sf::Color(255, 0, 255);    // Magenta
        player_trail = sf::Color(0, 200, 255);
        enemy_basic = sf::Color(255, 0, 100);         // Hot pink
        enemy_pattern = sf::Color(255, 100, 255);     // Pink-purple
        boss = sf::Color(255, 0, 255);                // Magenta
        powerup_shield = sf::Color(0, 255, 255);      // Cyan
        powerup_fire = sf::Color(255, 50, 150);       // Pink
        powerup_repair = sf::Color(0, 255, 150);      // Mint
        powerup_speed = sf::Color(255, 255, 0);       // Yellow
        explosion_core = sf::Color(255, 255, 255);
        explosion_outer = sf::Color(255, 0, 200);
        starfield = sf::Color(100, 200, 255);
        ui_accent = sf::Color(0, 255, 255);
        background = sf::Color(10, 5, 20);
    } else if (theme == DEEP_SPACE) {
        // Deep space - blues and purples
        player_primary = sf::Color(100, 150, 255);
        player_secondary = sf::Color(150, 100, 255);
        player_trail = sf::Color(120, 140, 255);
        enemy_basic = sf::Color(180, 50, 100);
        enemy_pattern = sf::Color(150, 80, 200);
        boss = sf::Color(120, 50, 180);
        powerup_shield = sf::Color(80, 120, 255);
        powerup_fire = sf::Color(200, 80, 120);
        powerup_repair = sf::Color(80, 200, 180);
        powerup_speed = sf::Color(200, 200, 100);
        explosion_core = sf::Color(255, 200, 150);
        explosion_outer = sf::Color(150, 100, 200);
        starfield = sf::Color(180, 180, 255);
        ui_accent = sf::Color(120, 150, 255);
        background = sf::Color(5, 5, 15);
    } else if (theme == SOLAR_FLARE) {
        // Solar - oranges, yellows, reds
        player_primary = sf::Color(255, 200, 50);
        player_secondary = sf::Color(255, 150, 0);
        player_trail = sf::Color(255, 180, 0);
        enemy_basic = sf::Color(255, 80, 50);
        enemy_pattern = sf::Color(255, 120, 0);
        boss = sf::Color(255, 50, 0);
        powerup_shield = sf::Color(255, 220, 100);
        powerup_fire = sf::Color(255, 100, 50);
        powerup_repair = sf:: Color(100, 255, 100);
        powerup_speed = sf::Color(255, 255, 150);
        explosion_core = sf::Color(255, 255, 200);
        explosion_outer = sf::Color(255, 150, 50);
        starfield = sf::Color(255, 220, 180);
        ui_accent = sf::Color(255, 200, 100);
        background = sf::Color(20, 10, 5);
    } else if (theme == TOXIC_WASTE) {
        // Toxic - greens and acid colors
        player_primary = sf::Color(150, 255, 50);
        player_secondary = sf::Color(100, 255, 150);
        player_trail = sf::Color(150, 255, 100);
        enemy_basic = sf::Color(200, 50, 50);
        enemy_pattern = sf::Color(255, 200, 50);
        boss = sf::Color(150, 50, 150);
        powerup_shield = sf::Color(100, 255, 200);
        powerup_fire = sf::Color(255, 255, 50);
        powerup_repair = sf:: Color(150, 255, 150);
        powerup_speed = sf::Color(200, 255, 100);
        explosion_core = sf::Color(200, 255, 100);
        explosion_outer = sf::Color(100, 200, 50);
        starfield = sf::Color(150, 255, 150);
        ui_accent = sf::Color(150, 255, 100);
        background = sf::Color(10, 15, 5);
    } else if (theme == ROYAL_NEBULA) {
        // Royal - purples, golds, majestic
        player_primary = sf::Color(200, 150, 255);
        player_secondary = sf::Color(255, 215, 0);   // Gold
        player_trail = sf::Color(220, 180, 255);
        enemy_basic = sf::Color(180, 50, 150);
        enemy_pattern = sf::Color(150, 100, 200);
        boss = sf::Color(120, 50, 200);
        powerup_shield = sf::Color(180, 150, 255);
        powerup_fire = sf:: Color(255, 100, 150);
        powerup_repair = sf:: Color(100, 255, 200);
        powerup_speed = sf::Color(255, 215, 100);
        explosion_core = sf::Color(255, 230, 180);
        explosion_outer = sf::Color(180, 100, 255);
        starfield = sf::Color(200, 180, 255);
        ui_accent = sf::Color(200, 150, 255);
        background = sf::Color(15, 10, 25);
    }
}

sf::Color ColorPalette::get(const std::string& name) const {
    if (name == "player_primary") {
        return player_primary;
    } else if (name == "player_secondary") {
        return player_secondary;
    } else if (name == "player_trail") {
        return player_trail;
    } else if (name == "enemy_basic") {
        return enemy_basic;
    } else if (name == "enemy_pattern") {
        return enemy_pattern;
    } else if (name == "boss") {
        return boss;
    } else if (name == "powerup_shield") {
        return powerup_shield;
    } else if (name == "powerup_fire") {
        return powerup_fire;
    } else if (name == "powerup_repair") {
        return powerup_repair;
    } else if (name == "powerup_speed") {
        return powerup_speed;
    } else if (name == "explosion_core") {
        return explosion_core;
    } else if (name == "explosion_outer") {
        return explosion_outer;
    } else if (name == "starfield") {
        return starfield;
    } else if (name == "ui_accent") {
        return ui_accent;
    } else if (name == "background") {
        return background;
    } else {
        return sf::Color::White; // Fallback
    }
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