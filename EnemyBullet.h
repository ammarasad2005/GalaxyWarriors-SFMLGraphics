#ifndef ENEMYBULLET_H
#define ENEMYBULLET_H

#include "Projectile.h"

class EnemyBullet : public Projectile {
public:
    EnemyBullet(float x, float y, float dirX, float dirY);
    ~EnemyBullet() override {}
    
    void render(sf:: RenderWindow& window) override;
};

#endif