#pragma once
#include "Polygon.h"

namespace Radiant {
	class Circle : public Polygon {
	private:
		double m_radius;
	public:
		Circle(Vec2d spawnLocation, double radius);
		~Circle();

		/*
			Gets the radius of the circle.
		*/
		double GetRadius() const;
	};
}