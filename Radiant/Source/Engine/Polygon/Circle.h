#pragma once
#include "Polygon.h"

namespace rdt {
	class Circle : public Polygon {
	private:
		struct Impl;
		Impl* m_impl;

	public:
		Circle(Vec2d spawnLocation, double radius);
		~Circle();

		/*
			Gets the radius of the circle.
		*/
		double GetRadius() const;
	};
}