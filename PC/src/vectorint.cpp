#include <headers/vectorint.h>
#include <math.h>

Vec2::Vec2(int xValue, int yValue)
{
    x = xValue;
    y = yValue;
}

Vec2::~Vec2() {}


Vec2 Vec2::operator+(const Vec2 &a) { 
    return Vec2(x + a.x, y + a.y);
}

Vec2 Vec2::operator-(const Vec2 &a) { 
    return Vec2(x - a.x, y - a.y);
}

Vec2 Vec2::operator-()
{
    return Vec2(-x,-y);
}

Vec2 Vec2::operator+=(const Vec2 &a)
{
    x += a.x;
    y += a.y;
    return *this;
}

Vec2 Vec2::operator-=(const Vec2 &a)
{
    x -= a.x;
    y -= a.y;
    return *this;
}


bool Vec2::operator==(const Vec2 &a) {
    return x == a.x && y == a.y; 
}