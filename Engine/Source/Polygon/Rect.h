#pragma once

#include "Polygon.h"
namespace Radiant {
	class Rect : public Polygon {
	private:
	public:
		Rect(Vec2d spawnLocation, double width, double height);
		~Rect() {}

	};
}