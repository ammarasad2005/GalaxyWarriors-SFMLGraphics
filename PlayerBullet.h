#ifndef PLAYERBULLET_H
#define PLAYERBULLET_H

#include "Projectile.h"

class PlayerBullet : public Projectile {
public:
    PlayerBullet(float x, float y, float dirX, float dirY);
    ~PlayerBullet() override {}
    
    void render(sf::RenderWindow& window) override;
};

#endif