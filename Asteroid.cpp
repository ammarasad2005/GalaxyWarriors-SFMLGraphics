#include "Asteroid.h"
#include "ColorPalette.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

Asteroid::Asteroid(float x, float y, int type)
    : Obstacle(x, y, 20.0f * (type + 1), 20.0f + type * 15, 
               sf::Color(80, 80, 80), (rand() % 100 - 50) * 0.5f),
      asteroidType(type),
      pulseTimer(0) {
    
    velocity = sf::Vector2f(
        (rand() % 100 - 50) * 0.5f,
        50.0f + type * 20.0f
    );
    
    // Creative color variations
    int colorVariant = rand() % 3;
    if (colorVariant == 0) {
        color = sf::Color(100, 80, 70);
        edgeColor = sf::Color(140, 110, 90);
    } else if (colorVariant == 1) {
        color = sf:: Color(60, 70, 90);
        edgeColor = sf::Color(90, 110, 140);
    } else {
        color = sf::Color(90, 70, 60);
        edgeColor = sf::Color(130, 100, 80);
    }
    
    generateVertices();
}

void Asteroid::generateVertices() {
    vertices.clear();
    
    switch(asteroidType) {
        case 0: // Small - Irregular rock
            {
                int vertexCount = 6 + (rand() % 3);
                float angleStep = 360.0f / vertexCount;
                
                for (int i = 0; i < vertexCount; i++) {
                    float angle = angleStep * i + (rand() % 15 - 7);
                    float distance = size * (0.7f + (rand() % 35) * 0.01f);
                    
                    float rad = angle * 3.14159f / 180.0f;
                    vertices.push_back(sf::Vector2f(
                        std::cos(rad) * distance,
                        std::sin(rad) * distance
                    ));
                }
            }
            break;
            
        case 1: // Medium - Chunky asteroid
            {
                int vertexCount = 8 + (rand() % 4);
                float angleStep = 360.0f / vertexCount;
                
                for (int i = 0; i < vertexCount; i++) {
                    float angle = angleStep * i + (rand() % 20 - 10);
                    float radiusVariation = (i % 2 == 0) ? 1.0f : 0.7f;
                    float distance = size * radiusVariation * (0.8f + (rand() % 25) * 0.01f);
                    
                    float rad = angle * 3.14159f / 180.0f;
                    vertices.push_back(sf::Vector2f(
                        std::cos(rad) * distance,
                        std::sin(rad) * distance
                    ));
                }
            }
            break;
            
        case 2: // Large - Complex shape
            {
                int vertexCount = 10 + (rand() % 5);
                float angleStep = 360.0f / vertexCount;
                
                for (int i = 0; i < vertexCount; i++) {
                    float angle = angleStep * i + (rand() % 25 - 12);
                    float wave = std::sin(i * 0.8f) * 0.3f;
                    float distance = size * (0.7f + wave + (rand() % 30) * 0.01f);
                    
                    float rad = angle * 3.14159f / 180.0f;
                    vertices.push_back(sf::Vector2f(
                        std::cos(rad) * distance,
                        std::sin(rad) * distance
                    ));
                }
            }
            break;
    }
}

void Asteroid::render(sf::RenderWindow& window) {
    if (!active || vertices.empty()) return;
    
    pulseTimer += 0.016f;
    
    // Create convex shape
    sf::ConvexShape asteroidShape(vertices.size());
    for (size_t i = 0; i < vertices.size(); i++) {
        asteroidShape.setPoint(i, vertices[i]);
    }
    
    // Subtle pulse
    float pulse = 1.0f + std::sin(pulseTimer * 1.5f) * 0.03f;
    asteroidShape. setScale(pulse, pulse);
    
    // Enhanced shading
    float shadeFactor = 0.7f + std::sin(rotation * 3.14159f / 180.0f) * 0.3f;
    sf::Color shadedColor = color;
    shadedColor.r *= shadeFactor;
    shadedColor.g *= shadeFactor;
    shadedColor.b *= shadeFactor;
    
    asteroidShape.setFillColor(shadedColor);
    asteroidShape.setOutlineColor(edgeColor);
    asteroidShape.setOutlineThickness(2);
    asteroidShape.setPosition(position);
    asteroidShape.setRotation(rotation);
    
    window.draw(asteroidShape);
    
    // Crater details
    if (asteroidType >= 1) {
        int craterCount = 3 + asteroidType * 2;
        for (int i = 0; i < craterCount; i++) {
            float craterAngle = (360.0f / craterCount) * i + rotation;
            float craterRad = craterAngle * 3.14159f / 180.0f;
            float craterDist = size * (0.3f + (i % 2) * 0.2f);
            float craterSize = size * (0.1f + (rand() % 10) * 0.01f);
            
            // Crater shadow
            sf::CircleShape craterShadow(craterSize * 1.2f);
            craterShadow.setFillColor(sf::Color(20, 20, 20, 100));
            craterShadow.setPosition(
                position.x + std::cos(craterRad) * craterDist - craterSize * 1.2f,
                position. y + std::sin(craterRad) * craterDist - craterSize * 1.2f
            );
            window.draw(craterShadow);
            
            // Crater
            sf::CircleShape crater(craterSize);
            crater. setFillColor(sf::Color(30, 30, 30, 180));
            crater.setPosition(
                position.x + std:: cos(craterRad) * craterDist - craterSize,
                position. y + std::sin(craterRad) * craterDist - craterSize
            );
            window.draw(crater);
            
            // Highlight
            sf::CircleShape highlight(craterSize * 0.4f);
            highlight.setFillColor(sf::Color(100, 100, 100, 100));
            highlight.setPosition(
                position.x + std:: cos(craterRad) * craterDist - craterSize * 0.6f,
                position.y + std::sin(craterRad) * craterDist - craterSize * 0.6f
            );
            window.draw(highlight);
        }
    }
    
    // Surface details for large asteroids
    if (asteroidType == 2) {
        for (int i = 0; i < 5; i++) {
            float detailAngle = (72.0f * i + rotation * 0.5f) * 3.14159f / 180.0f;
            float detailDist = size * 0.5f;
            
            sf::CircleShape detail(size * 0.08f);
            detail.setFillColor(ColorPalette::lighten(color, 0.2f));
            detail.setPosition(
                position.x + std::cos(detailAngle) * detailDist - size * 0.08f,
                position.y + std::sin(detailAngle) * detailDist - size * 0.08f
            );
            window.draw(detail);
        }
    }
}

void Asteroid::onCollision(GameObject* other) {
    // Handled by CollisionManager
}