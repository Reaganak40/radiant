#pragma once
#include "Polygon.h"

namespace rdt {
	class Line : public Polygon {
	private:
	public:
		Line(const Vec2d& start, const Vec2d& end);
		~Line() {}

		void SetStart(const Vec2d& start);
		void SetEnd(const Vec2d& end);
	};
}