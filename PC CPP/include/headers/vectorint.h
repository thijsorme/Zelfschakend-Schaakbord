#pragma once
#include <iostream>

class Vec2 {
    public:
        Vec2(int xValue = 0, int yValue = 0);
        ~Vec2();

        int x;
        int y;

		Vec2 operator-();
		Vec2 operator+(const Vec2& a);
		Vec2 operator-(const Vec2& a);
		Vec2 operator+=(const Vec2& a);
		Vec2 operator-=(const Vec2& a);

		bool operator==(const Vec2& a);

    private:

};