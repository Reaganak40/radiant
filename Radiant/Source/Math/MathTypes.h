#pragma once

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