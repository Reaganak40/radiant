#pragma once

namespace Radiant {

	struct Vec2d {
		double x, y;

		Vec2d(double nX = 0, double nY = 0)
			: x(nX), y(nY) {}
	};

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