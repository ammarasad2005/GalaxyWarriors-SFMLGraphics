#include "MenuSystem.h"
#include <cmath>
#include <sstream>

MenuSystem:: MenuSystem() 
    : selectedThemeIndex(0), 
      inThemeSelection(false),
      animationTimer(0) {
    
    setupMainMenu();
    setupThemeSelection();
}

void MenuSystem::setupMainMenu() {
    mainMenuButtons.clear();
    
    float centerX = 300;
    float startY = 300;
    float buttonWidth = 200;
    float buttonHeight = 50;
    float spacing = 70;
    
    mainMenuButtons.pushBack(MenuButton(centerX, startY, buttonWidth, buttonHeight, "[1] START GAME", 1));
    mainMenuButtons.pushBack(MenuButton(centerX, startY + spacing, buttonWidth, buttonHeight, "[2] THEME SELECTION", 2));
    mainMenuButtons.pushBack(MenuButton(centerX, startY + spacing * 2, buttonWidth, buttonHeight, "[3] HIGH SCORES", 3));
    mainMenuButtons.pushBack(MenuButton(centerX, startY + spacing * 3, buttonWidth, buttonHeight, "[4] EXIT", 4));
}

void MenuSystem::setupThemeSelection() {
    themeCards.clear();
    
    float cardWidth = 140;
    float cardHeight = 180;
    float spacing = 20;
    float startX = 50;
    float startY = 150;
    
    for (int i = 0; i < 5; i++) {
        float x = startX + (cardWidth + spacing) * i;
        themeCards.pushBack(ThemeCard(
            static_cast<ColorPalette::   Theme>(i),
            x, startY, cardWidth, cardHeight
        ));
    }
    
    themeCards[0].selected = true;
}

void MenuSystem::update(float deltaTime) {
    animationTimer += deltaTime;
    
    if (rand() % 100 < 5) {
        spawnMenuParticle();
    }
    
    updateMenuParticles(deltaTime);
}

void MenuSystem::updateMenuParticles(float deltaTime) {
    for (int i = 0; i < menuParticles.getSize();) {
        menuParticles[i].position += menuParticles[i].velocity * deltaTime;
        menuParticles[i].life -= deltaTime;
        
        menuParticles[i].color.a = static_cast<sf::Uint8>(menuParticles[i].life * 255);
        menuParticles[i].velocity *= 0.98f;
        
        if (menuParticles[i].life <= 0) {
            menuParticles.removeAt(i);
        } else {
            i++;
        }
    }
}

void MenuSystem::spawnMenuParticle() {
    ColorPalette* palette = ColorPalette::getInstance();
    
    MenuParticle particle;
    particle.position = sf::Vector2f(rand() % 800, 600);
    particle.velocity = sf::Vector2f((rand() % 40 - 20) * 0.5f, -(50 + rand() % 50));
    particle.color = palette->getUIAccent();
    particle.color.a = 150;
    particle.life = 2.0f + (rand() % 20) * 0.1f;
    particle.size = 2.0f + (rand() % 3);
    
    menuParticles.pushBack(particle);
}

void MenuSystem::render(sf::RenderWindow& window, bool fontLoaded, sf::Font& font) {
    ColorPalette* palette = ColorPalette::getInstance();
    
    // Render particles
    for (int i = 0; i < menuParticles.getSize(); i++) {
        sf::CircleShape circle(menuParticles[i].size);
        circle.setPosition(menuParticles[i].position);
        circle.setFillColor(menuParticles[i].color);
        window.draw(circle);
    }
    
    // Title
    float titleY = 80;
    float pulse = 1.0f + std::sin(animationTimer * 2.0f) * 0.1f;
    
    for (int i = 5; i > 0; i--) {
        sf::Color glowColor = palette->getUIAccent();
        glowColor.   a = 30 * i;
        drawText(window, "GALAXY WARS", 400, titleY, 
                80 + i * 3, glowColor, true, fontLoaded, font);
    }
    
    sf::Color titleColor = ColorPalette::lighten(palette->getUIAccent(), 0.3f);
    drawText(window, "GALAXY WARS", 400, titleY, 80, titleColor, true, fontLoaded, font);
    
    // Subtitle
    float hue = std::sin(animationTimer * 1.5f) * 0.5f + 0.5f;
    sf::Color subtitleColor = ColorPalette::blend(
        palette->getPlayerPrimary(),
        palette->getPlayerSecondary(),
        hue
    );
    drawText(window, "Object-Oriented Space Battle", 400, 160, 
             20, subtitleColor, true, fontLoaded, font);
    
    // Menu buttons
    for (int i = 0; i < mainMenuButtons.getSize(); i++) {
        MenuButton& button = mainMenuButtons[i];
        sf::RectangleShape bg(sf::Vector2f(button.bounds.width, button.bounds.height));
        bg.setPosition(button.bounds.left, button.bounds.top);
        
        if (button.highlighted) {
            sf::RectangleShape glow(sf::Vector2f(button.bounds.width + 10, button.bounds.height + 10));
            glow.setPosition(button.bounds.left - 5, button.bounds.top - 5);
            sf::Color glowColor = palette->getUIAccent();
            glowColor.a = 100;
            glow.setFillColor(glowColor);
            window.draw(glow);
            
            bg.setFillColor(button.highlightColor);
            bg.setOutlineColor(palette->getUIAccent());
            bg.setOutlineThickness(3);
            
            for (int j = 0; j < 4; j++) {
                sf::CircleShape particle(3);
                float progress = std::fmod(animationTimer * 2.0f + j * 0.25f, 1.0f);
                float x = button.bounds.left + button.bounds.width * progress;
                float y = button.bounds.top + (j % 2) * button.bounds.height;
                particle.setPosition(x, y);
                particle.setFillColor(palette->getUIAccent());
                window.draw(particle);
            }
        } else {
            bg.setFillColor(button.normalColor);
            bg.setOutlineColor(sf::Color(100, 100, 150));
            bg.setOutlineThickness(2);
        }
        
        window.draw(bg);
        
        sf::Color textColor = button.highlighted ? sf::Color::White : sf::Color(200, 200, 200);
        drawText(window, button.text, 
                button.bounds.left + button.bounds.width / 2,
                button.bounds.top + button.bounds.height / 2 - 10,
                22, textColor, true, fontLoaded, font);
    }
    
    // Decorative orbs
    float orbPulse = 1.0f + std::sin(animationTimer * 3.0f) * 0.15f;
    
    sf::CircleShape leftOrb(25);
    leftOrb.setPosition(180, 320);
    leftOrb.setFillColor(ColorPalette::withAlpha(palette->getPlayerPrimary(), 180));
    leftOrb.setScale(orbPulse, orbPulse);
    window.draw(leftOrb);
    
    sf::CircleShape leftCore(12);
    leftCore.setPosition(193, 333);
    leftCore.setFillColor(ColorPalette::   lighten(palette->getPlayerPrimary(), 0.5f));
    window.draw(leftCore);
    
    sf::CircleShape rightOrb(25);
    rightOrb.setPosition(575, 320);
    rightOrb.setFillColor(ColorPalette::withAlpha(palette->getPlayerSecondary(), 180));
    rightOrb.setScale(orbPulse, orbPulse);
    window.draw(rightOrb);
    
    sf::CircleShape rightCore(12);
    rightCore.setPosition(588, 333);
    rightCore.setFillColor(ColorPalette::lighten(palette->getPlayerSecondary(), 0.5f));
    window.draw(rightCore);
    
    drawText(window, "v2.0 - Enhanced Edition", 400, 565, 
             14, sf::Color(120, 120, 120), true, fontLoaded, font);
}

void MenuSystem::renderThemeSelection(sf::RenderWindow& window, bool fontLoaded, sf::Font& font) {
    ColorPalette* palette = ColorPalette::getInstance();
    
    drawText(window, "SELECT YOUR THEME", 400, 50, 
             50, palette->getUIAccent(), true, fontLoaded, font);
    
    drawText(window, "Click a theme to preview, then press START to play", 400, 100,
             18, sf::Color(180, 180, 180), true, fontLoaded, font);
    
    // Theme cards
    for (size_t i = 0; i < themeCards.getSize(); i++) {
        auto& card = themeCards[i];
        
        ColorPalette::Theme originalTheme = palette->getCurrentTheme();
        palette->setTheme(card.theme);
        
        sf::RectangleShape cardBg(sf::Vector2f(card.bounds.width, card.bounds.height));
        cardBg.setPosition(card.bounds.   left, card.bounds.top);
        
        if (card. selected) {
            sf::RectangleShape selectedGlow(sf::Vector2f(card.bounds.width + 8, card.bounds.height + 8));
            selectedGlow.   setPosition(card.bounds.left - 4, card.bounds.  top - 4);
            selectedGlow.setFillColor(sf::Color::   Transparent);
            selectedGlow.setOutlineColor(sf::Color(255, 215, 0));
            selectedGlow.setOutlineThickness(4);
            window.draw(selectedGlow);
            
            cardBg.setFillColor(sf::Color(60, 60, 100));
            cardBg.setOutlineColor(sf::Color(255, 215, 0));
            cardBg.setOutlineThickness(3);
        } else if (card.hovered) {
            cardBg.setFillColor(sf::Color(50, 50, 80));
            cardBg.setOutlineColor(palette->getUIAccent());
            cardBg.setOutlineThickness(3);
        } else {
            cardBg.setFillColor(sf::Color(30, 30, 50));
            cardBg.setOutlineColor(sf:: Color(80, 80, 100));
            cardBg.setOutlineThickness(2);
        }
        
        window.   draw(cardBg);
        
        // Preview mini ship
        float shipY = card.bounds.top + 50;
        sf::ConvexShape miniShip(3);
        miniShip.setPoint(0, sf::Vector2f(0, -10));
        miniShip.setPoint(1, sf::Vector2f(-8, 10));
        miniShip.setPoint(2, sf::Vector2f(8, 10));
        miniShip.setFillColor(palette->getPlayerPrimary());
        miniShip.setOutlineColor(palette->getPlayerSecondary());
        miniShip.setOutlineThickness(1);
        miniShip.setPosition(card.   bounds.left + card.bounds.width / 2, shipY);
        window.draw(miniShip);
        
        // Mini enemy
        sf::CircleShape miniEnemy(8, 4);
        miniEnemy.setFillColor(palette->getEnemyBasicPrimary());
        miniEnemy.setPosition(card.bounds.left + card.bounds.width / 2 - 8, shipY + 30);
        window.draw(miniEnemy);
        
        // Color swatches
        float swatchY = card.bounds.top + 110;
        float swatchSize = 15;
        float swatchSpacing = 20;
        float startX = card.bounds.left + (card.bounds.width - swatchSpacing * 3) / 2;
        
        sf::Color swatches[] = {
            palette->getPlayerPrimary(),
            palette->getEnemyBasicPrimary(),
            palette->getPowerUpShield(),
            palette->getExplosionCore()
        };
        
        for (int j = 0; j < 4; j++) {
            sf::RectangleShape swatch(sf::Vector2f(swatchSize, swatchSize));
            swatch.setPosition(startX + j * swatchSpacing, swatchY);
            swatch.setFillColor(swatches[j]);
            swatch.setOutlineColor(sf::Color::White);
            swatch.setOutlineThickness(1);
            window.draw(swatch);
        }
        
        drawText(window, card.name, 
                card.bounds.left + card.bounds.width / 2,
                card.bounds.top + card.bounds.height - 50,
                16, sf::Color::   White, true, fontLoaded, font);
        
        drawText(window, card.description,
                card.bounds.left + card.bounds.width / 2,
                card.bounds.top + card.bounds.height - 25,
                12, sf::Color(180, 180, 180), true, fontLoaded, font);
        
        palette->setTheme(originalTheme);
    }
    
    // Back button
    sf::RectangleShape backButton(sf::Vector2f(150, 40));
    backButton.setPosition(50, 550);
    backButton.setFillColor(sf::Color(60, 60, 100));
    backButton.setOutlineColor(palette->getUIAccent());
    backButton.setOutlineThickness(2);
    window.draw(backButton);
    
    drawText(window, "[ESC] BACK", 125, 560, 18, sf::Color::   White, true, fontLoaded, font);
}

int MenuSystem::handleClick(float x, float y) {
    if (inThemeSelection) {
        for (int i = 0; i < themeCards.getSize(); i++) {
            if (themeCards[i].contains(x, y)) {
                for (int j = 0; j < themeCards.getSize(); j++) {
                    themeCards[j].selected = false;
                }
                themeCards[i].selected = true;
                selectedThemeIndex = i;
                
                ColorPalette::getInstance()->setTheme(themeCards[i].theme);
                return 100 + i;
            }
        }
        
        if (x >= 50 && x <= 200 && y >= 550 && y <= 590) {
            return -1;
        }
        
        return 0;
    } else {
        for (int i = 0; i < mainMenuButtons.getSize(); i++) {
            if (mainMenuButtons[i].contains(x, y)) {
                return mainMenuButtons[i].actionId;
            }
        }
    }
    
    return 0;
}

void MenuSystem::handleHover(float x, float y) {
    if (inThemeSelection) {
        for (int i = 0; i < themeCards.getSize(); i++) {
            themeCards[i].hovered = themeCards[i].contains(x, y);
        }
    } else {
        for (int i = 0; i < mainMenuButtons.getSize(); i++) {
            mainMenuButtons[i].highlighted = mainMenuButtons[i].contains(x, y);
        }
    }
}

void MenuSystem::drawText(sf::RenderWindow& window, const std::string& text,
                          float x, float y, int size, sf::Color color,
                          bool centered, bool fontLoaded, sf::   Font& font) {
    if (fontLoaded) {
        sf::Text sfText;
        sfText.setFont(font);
        sfText.setString(text);
        sfText.setCharacterSize(size);
        sfText.setFillColor(color);
        
        if (centered) {
            sf::FloatRect bounds = sfText.getLocalBounds();
            sfText.setOrigin(bounds.width / 2, bounds.height / 2);
        }
        
        sfText.setPosition(x, y);
        window.draw(sfText);
    } else {
        sf::RectangleShape textBox(sf::Vector2f(text.length() * size * 0.6f, size));
        textBox.setFillColor(sf::Color(50, 50, 50, 100));
        textBox.setOutlineColor(color);
        textBox.setOutlineThickness(1);
        
        if (centered) {
            textBox.setPosition(x - textBox.getSize().x / 2, y);
        } else {
            textBox.setPosition(x, y);
        }
        
        window.draw(textBox);
    }
}