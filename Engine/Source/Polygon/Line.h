#pragma once
#include "Polygon.h"

namespace Radiant {
	class Line : public Polygon {
	private:
	public:
		Line(const Vec2d& start, const Vec2d& end);
		~Line() {}


	};
}