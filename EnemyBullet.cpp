#include "EnemyBullet.h"
#include "ColorPalette.h"

EnemyBullet::EnemyBullet(float x, float y, float dirX, float dirY)
    : Projectile(x, y, 15.0f, 300.0f, 1, dirX, dirY, sf::Color::Red) {
    
    ColorPalette* palette = ColorPalette::getInstance();
    color = palette->getEnemyBasicPrimary();
}

void EnemyBullet:: render(sf::RenderWindow& window) {
    if (!active) return;
    
    sf::RectangleShape bullet(sf::Vector2f(size, size * 2));
    bullet.setFillColor(color);
    bullet.setPosition(position.x - size/2, position.y - size);
    window.draw(bullet);
}