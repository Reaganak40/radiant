#pragma once

namespace Radiant {

	struct Vec2d {
		double x, y;

		Vec2d(double nX = 0, double nY = 0)
			: x(nX), y(nY) {}

		Vec2d operator+=(const Vec2d& vector) {
			x += vector.x;
			y += vector.y;
			return (*this);
		}

		Vec2d operator*=(const double multiplier) {
			x *= multiplier;
			y *= multiplier;
			return (*this);
		}
	};
	Vec2d operator* (float multiplier, const Vec2d& vector);
	Vec2d operator* (const Vec2d& vector, float multiplier);
	Vec2d operator+ (const Vec2d& vector1, const Vec2d& vector2);
	Vec2d Vabs(Vec2d vector);

	struct Vec3f {
		float x, y, z;

		Vec3f(float nX = 0, float nY = 0, float nZ = 0)
			: x(nX), y(nY), z(nZ) {}
	};

	struct Vec4f {
		float a, b, c, d;

		Vec4f(float nA = 0.0f, float nB = 0.0f, float nC = 0.0f, float nD = 0.0f)
			: a(nA), b(nB), c(nC), d(nD) {}
	};
}