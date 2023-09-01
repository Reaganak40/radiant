#pragma once
#include "Polygon.h"

namespace Radiant {

	class Triangle : public Polygon {
	private:
	public:
		Triangle(Vec2d spawnLocation, double width, double height);
	};

}