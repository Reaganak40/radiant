#pragma once
#include "Polygon.h"

namespace rdt {
	class RADIANT_API Rect : public Polygon {
	private:
	public:
		Rect(Vec2d spawnLocation = Vec2d(0, 0), double width = 0, double height = 0);
		~Rect() {
		}

		void SetSize(const Vec2d& nSize) override final;
		Vec2d GetBottomLeftCorner();
	};
}