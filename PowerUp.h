#ifndef POWERUP_H
#define POWERUP_H

#include "Collectible.h"
#include "PlayerShip.h"

class PowerUp : public Collectible {
public:
    enum Type {
        SHIELD,
        DOUBLE_FIRE,
        REPAIR,
        SPEED_BOOST
    };
    
private:
    Type type;
    float duration;
    float effectStrength;
    sf::Color glowColor;
    std::string name;
    
public:
    PowerUp(float x, float y, Type t);
    ~PowerUp() override {}
    
    void render(sf::RenderWindow& window) override;
    void apply(PlayerShip* player) override;
    
    Type getType() const { return type; }
    const std::string& getName() const { return name; }
    float getDuration() const { return duration; }
    float getEffectStrength() const { return effectStrength; }
};

#endif