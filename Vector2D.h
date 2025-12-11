#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <cmath>

// OPERATOR OVERLOADING:  Custom vector class with mathematical operators
class Vector2D {
private:
    float x, y;

public:
    // Constructors
    Vector2D() : x(0), y(0) {}
    Vector2D(float x, float y) : x(x), y(y) {}

    // CONST CORRECTNESS: Getters don't modify object
    float getX() const { return x; }
    float getY() const { return y; }
    
    void setX(float newX) { x = newX; }
    void setY(float newY) { y = newY; }

    // OPERATOR OVERLOADING: Vector addition
    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }

    // OPERATOR OVERLOADING: Vector subtraction
    Vector2D operator-(const Vector2D& other) const {
        return Vector2D(x - other.x, y - other.y);
    }

    // OPERATOR OVERLOADING:  Scalar multiplication
    Vector2D operator*(float scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    // OPERATOR OVERLOADING:  Compound assignment
    Vector2D& operator+=(const Vector2D& other) {
        x += other. x;
        y += other. y;
        return *this;
    }

    // OPERATOR OVERLOADING:  Equality comparison
    bool operator==(const Vector2D& other) const {
        return (x == other.x && y == other.y);
    }

    // Utility methods
    float length() const {
        return std::sqrt(x * x + y * y);
    }

    Vector2D normalize() const {
        float len = length();
        if (len > 0) {
            return Vector2D(x / len, y / len);
        }
        return Vector2D(0, 0);
    }

    float distance(const Vector2D& other) const {
        return (*this - other).length();
    }
};

#endif