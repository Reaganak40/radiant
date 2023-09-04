#include "pch.h"
#include "MathTypes.h"

namespace Radiant {

    Vec2d operator*(float multiplier, const Vec2d& vector)
    {
        return Vec2d(multiplier * vector.x, multiplier * vector.y);
    }

    Vec2d operator*(const Vec2d& vector, float multiplier)
    {
        return Vec2d(multiplier * vector.x, multiplier * vector.y);
    }

    Vec2d operator*(double multiplier, const Vec2d& vector)
    {
        return Vec2d(multiplier * vector.x, multiplier * vector.y);

    }

    Vec2d operator*(const Vec2d& vector, double multiplier)
    {
        return Vec2d(multiplier * vector.x, multiplier * vector.y);

    }

    Vec2d operator+(const Vec2d& vector1, const Vec2d& vector2)
    {
        return Vec2d(vector1.x + vector2.x, vector1.y + vector2.y);
    }

    Vec2d operator-(const Vec2d& vector1, const Vec2d& vector2)
    {
        return Vec2d(vector1.x - vector2.x, vector1.y - vector2.y);
    }

    Vec2d operator/(const Vec2d& vector, const double divider)
    {
        return Vec2d(vector.x / divider, vector.y / divider);
    }

    Vec2d Vabs(Vec2d vector)
    {
        return Vec2d(abs(vector.x), abs(vector.y));
    }

    double Vec2d::Magnitude() const
    {
        return sqrt((x * x) + (y * y));
    }

    Vec2d Vec2d::Normal() const
    {
        return Vec2d(y, -x);
    }

    Vec2d Vec2d::Normalize() const
    {
        return (*this) / Magnitude();
    }

    double Vec2d::Dot(const Vec2d& A, const Vec2d& B)
    {
        return (A.x * B.x) + (A.y * B.y);
    }

}
