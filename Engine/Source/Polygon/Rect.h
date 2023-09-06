#pragma once

#include "Polygon.h"
namespace Radiant {
	class Rect : public Polygon {
	private:
	public:
		Rect(Vec2d spawnLocation = Vec2d(0, 0), double width = 0, double height = 0);
		~Rect() {}

		void SetPosition(const Vec2d& nPosition) override final;
		void SetSize(const Vec2d& nSize) override final;

	};
}