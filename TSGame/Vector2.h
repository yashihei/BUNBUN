#pragma once
#include <cmath>

struct Vector2
{
	float x, y;

    Vector2(float x = 0, float y = 0) : x(x) , y(y) {}

    float dot(const Vector2& other) const {
        return x * other.x + y * other.y;
    }

    float cross(const Vector2& other) const {
        return x * other.y - y * other.x;
    }

    float length() const {
        return std::sqrt(lengthSquare());
    }

    float lengthSquare() const {
        return dot(*this);
    }

    float distanceFrom(const Vector2& other) const {
        return (other - *this).length();
    }

    Vector2 normalized() const {
        return *this / length();
    }

    bool isZero() const {
        return x == 0.0 && y == 0.0;
    }

    static Vector2 fromAngle(float rad) {
        return Vector2(std::cos(rad), std::sin(rad));
    }

    float toAngle() const {
        if (isZero())
            return 0.0f;
        return std::atan2(y, x);
    }

    Vector2 rotate(float rad) const {
        return Vector2(x * std::cos(rad) - y * std::sin(rad), x * std::sin(rad) + y * std::cos(rad));
    }

    Vector2 operator +() const {
        return *this;
    }

    Vector2 operator -() const {
        return { -x, -y };
    }

    Vector2 operator +(const Vector2& other) const {
        return { x + other.x, y + other.y };
    }

    Vector2 operator -(const Vector2& other) const {
        return { x - other.x, y - other.y };
    }

    Vector2 operator *(float s) const {
        return { x * s, y * s };
    }

    Vector2 operator /(float s) const {
        return { x / s, y / s };
    }

    Vector2& operator +=(const Vector2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2& operator -=(const Vector2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vector2& operator *=(float s) {
        x *= s;
        y *= s;
        return *this;
    }

    Vector2& operator /=(float s) {
        x /= s;
        y /= s;
        return *this;
    }
};

using Vec2 = Vector2;
