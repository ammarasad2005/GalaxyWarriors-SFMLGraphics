#ifndef MENUSYSTEM_H
#define MENUSYSTEM_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "ColorPalette.h"

struct MenuButton {
    sf::FloatRect bounds;
    std::string text;
    bool highlighted;
    sf::Color normalColor;
    sf::Color highlightColor;
    int actionId;
    
    MenuButton(float x, float y, float w, float h, const std::string& txt, int id)
     : bounds(x, y, w, h), 
          text(txt), 
  highlighted(false),
      normalColor(sf::Color(40, 40, 80)),
     highlightColor(sf::Color(80, 80, 150)),
          actionId(id) {}
    
 bool contains(float x, float y) const {
        return bounds.contains(x, y);
    }
};

struct ThemeCard {
    ColorPalette::Theme theme;
    sf::FloatRect bounds;
    std::string name;
    std::string description;
    bool selected;
    bool hovered;
    
    ThemeCard(ColorPalette::Theme t, float x, float y, float w, float h)
        : theme(t), bounds(x, y, w, h), selected(false), hovered(false) {
        
        switch(t) {
   case ColorPalette::NEON_CYBER:
        name = "NEON CYBER";
          description = "Cyberpunk vibes";
    break;
    case ColorPalette::DEEP_SPACE:
     name = "DEEP SPACE";
       description = "Classic space";
      break;
            case ColorPalette::SOLAR_FLARE:
        name = "SOLAR FLARE";
  description = "Fire and fury";
                break;
case ColorPalette::TOXIC_WASTE:
     name = "TOXIC WASTE";
       description = "Radioactive";
   break;
            case ColorPalette::ROYAL_NEBULA:
       name = "ROYAL NEBULA";
           description = "Majestic cosmos";
           break;
    }
    }
    
    bool contains(float x, float y) const {
        return bounds.contains(x, y);
    }
};

class MenuSystem {
private:
    std::vector<MenuButton> mainMenuButtons;
    std::vector<ThemeCard> themeCards;
    int selectedThemeIndex;
    bool inThemeSelection;
    float animationTimer;
    
    struct MenuParticle {
        sf::Vector2f position;
        sf::Vector2f velocity;
 sf::Color color;
        float life;
        float size;
    };
    std::vector<MenuParticle> menuParticles;
    
public:
    MenuSystem();
    
    void setupMainMenu();
    void setupThemeSelection();
    
    void update(float deltaTime);
    void render(sf::RenderWindow& window, bool fontLoaded, sf::Font& font);
    void renderThemeSelection(sf::RenderWindow& window, bool fontLoaded, sf::Font& font);
    
    int handleClick(float x, float y);
    void handleHover(float x, float y);
    
    bool isInThemeSelection() const { return inThemeSelection; }
  void setThemeSelection(bool state) { inThemeSelection = state; }
    
    ColorPalette::Theme getSelectedTheme() const {
        return themeCards[selectedThemeIndex].theme;
    }
    
private:
    void updateMenuParticles(float deltaTime);
    void spawnMenuParticle();
    void drawText(sf::RenderWindow& window, const std::string& text, 
     float x, float y, int size, sf::Color color, 
       bool centered, bool fontLoaded, sf::Font& font);
};

#endif