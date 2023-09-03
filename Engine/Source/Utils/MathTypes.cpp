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

    Vec2d operator+(const Vec2d& vector1, const Vec2d& vector2)
    {
        return Vec2d(vector1.x + vector2.x, vector1.y + vector2.y);
    }

    Vec2d Vabs(Vec2d vector)
    {
        return Vec2d(abs(vector.x), abs(vector.y));
    }

    Vec2d Vec2d::Normal()
    {
        return Vec2d(y, -x);
    }

    double Vec2d::DotProduct(const Vec2d& A, const Vec2d& B)
    {
        return (A.x * B.x) + (A.y * B.y);
    }

}
