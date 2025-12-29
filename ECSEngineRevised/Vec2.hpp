#pragma once
#include <cmath>
#include <ostream>

class Vec2
{
public:
    float x{ 0.0f };
    float y{ 0.0f };

    // Constructors
    Vec2() = default;
    Vec2(float xin, float yin) : x(xin), y(yin) {}

    // Comparison operators
    bool operator==(const Vec2& rhs) const { return x == rhs.x && y == rhs.y; }
    bool operator!=(const Vec2& rhs) const { return !(*this == rhs); }

    // Arithmetic operators
    Vec2 operator+(const Vec2& rhs) const { return { x + rhs.x, y + rhs.y }; }
    Vec2 operator-(const Vec2& rhs) const { return { x - rhs.x, y - rhs.y }; }
    Vec2 operator*(float val) const { return { x * val, y * val }; }
    Vec2 operator/(float val) const { return { x / val, y / val }; }

    Vec2& operator+=(const Vec2& rhs) { x += rhs.x; y += rhs.y; return *this; }
    Vec2& operator-=(const Vec2& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
    Vec2& operator*=(float val) { x *= val; y *= val; return *this; }
    Vec2& operator/=(float val) { x /= val; y /= val; return *this; }

    // Magnitude and distance
    float Magnitude() const { return std::sqrt(x * x + y * y); }
    float MagnitudeSqr() const { return x * x + y * y; }
    float Dist(const Vec2& rhs) const { return (*this - rhs).Magnitude(); }

    // Normalize
    Vec2 Normalize() const {
        float mag = Magnitude();
        return mag == 0 ? Vec2(0, 0) : (*this / mag);
    }

    // Rotate vector by angle in radians
    Vec2 Rotate(float angle) const {
        float cosA = std::cos(angle);
        float sinA = std::sin(angle);
        return { x * cosA - y * sinA, x * sinA + y * cosA };
    }

    // Dot product
    static float Dot(const Vec2& a, const Vec2& b) { return a.x * b.x + a.y * b.y; }

    // Clamp
    static float Clamp(float value, float min, float max) {
        return (value < min) ? min : (value > max ? max : value);
    }

    // Lerp
    static Vec2 Lerp(const Vec2& start, const Vec2& end, float t) {
        return start + (end - start) * Clamp(t, 0.0f, 1.0f);
    }

    // Zero vector
    static Vec2 Zero() { return Vec2(0.0f, 0.0f); }

    // Stream output
    friend std::ostream& operator<<(std::ostream& os, const Vec2& vec) {
        os << "(" << vec.x << ", " << vec.y << ")";
        return os;
    }
};