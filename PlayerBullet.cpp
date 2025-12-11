#include "PlayerBullet.h"
#include "ColorPalette.h"

PlayerBullet::PlayerBullet(float x, float y, float dirX, float dirY)
    : Projectile(x, y, 10.0f, 500.0f, 0, dirX, dirY, sf::Color::Cyan) {
    
    ColorPalette* palette = ColorPalette::getInstance();
    color = palette->getPlayerPrimary();
}

void PlayerBullet::render(sf:: RenderWindow& window) {
    if (!active) return;
    
    // Glow effect
    sf::CircleShape glow(size * 1.5f);
    glow.setFillColor(ColorPalette::withAlpha(color, 100));
    glow.setPosition(position. x - size * 1.5f, position.y - size * 1.5f);
    window.draw(glow);
    
    // Main bullet
    sf::CircleShape bullet(size);
    bullet.setFillColor(color);
    bullet.setPosition(position.x - size, position.y - size);
    window.draw(bullet);
}