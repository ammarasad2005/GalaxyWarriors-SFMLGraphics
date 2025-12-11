#ifndef RESOURCE_H
#define RESOURCE_H

#include "Collectible.h"
#include "PlayerShip.h"
#include <iostream>

class Resource : public Collectible {
public:
    enum Type {
        ENERGY,
        MINERALS,
        SCRAP
    };
    
private: 
    Type type;
    int value;
    sf::Color primaryColor;
    sf::Color secondaryColor;
    
public: 
    Resource(float x, float y, Type t);
    ~Resource() override {}
    
    void render(sf::RenderWindow& window) override;
    void apply(PlayerShip* player) override;
    
    Type getType() const { return type; }
    int getValue() const { return value; }
    
    // OPERATOR OVERLOADING:  Combine resources
    Resource operator+(const Resource& other) const;
    
    // FRIEND FUNCTION: For debugging
    friend std::ostream& operator<<(std::ostream& os, const Resource& res);
};

// FRIEND FUNCTION implementation
inline std::ostream& operator<<(std::ostream& os, const Resource& res) {
    os << "Resource(Type:" << res.type << ", Value:" << res.value << ")";
    return os;
}

#endif