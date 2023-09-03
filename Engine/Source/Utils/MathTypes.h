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

		/*
			Returns the normal of this vector.
		*/
		Vec2d Normal();

		static double DotProduct(const Vec2d& A, const Vec2d& B);
	};
	Vec2d operator* (float multiplier, const Vec2d& vector);
	Vec2d operator* (const Vec2d& vector, float multiplier);
	Vec2d operator+ (const Vec2d& vector1, const Vec2d& vector2);

	/*
		Returns an absolute vector with all positive values.
	*/
	Vec2d Vabs(Vec2d vector);

	struct Vec3f {
		float x, y, z;

		Vec3f(float nX = 0, float nY = 0, float nZ = 0)
			: x(nX), y(nY), z(nZ) {}
	};

	struct Vec4f {
		float x1, x2, x3, x4;

		Vec4f(float nA = 0.0f, float nB = 0.0f, float nC = 0.0f, float nD = 0.0f)
			: x1(nA), x2(nB), x3(nC), x4(nD) {}
	};
}