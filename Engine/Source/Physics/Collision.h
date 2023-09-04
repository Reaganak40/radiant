#pragma once
#include "Polygon/Polygon.h"
#include "Polygon/Rect.h"
#include "Polygon/Circle.h"

namespace Radiant {
	class Collision {
	private:
	public:
		Collision() {}
		~Collision() {}

		/*
			Test for collision betwen two polygons using seperated axis theorem.
		*/
		static bool CheckCollisionSAT(const Polygon& A, const Polygon& B);
		
		/*
			Test for collision betwen a circle and a polygon using seperated axis theorem.
		*/
		static bool CheckCollisionSAT(const Circle& A, const Polygon& B);

		/*
			Tests for collisions of two rects on the same axis using AABB theorem.
		*/
		static bool CheckCollisionAABB(const Rect& A, const Rect& B);

		/*
			Resolves static collisions, and returns true if there was collision resolution.
		*/
		static bool StaticCollisionDiags(Polygon& dynamicPoly, Polygon& staticPoly);



	private:
		static bool PointVsRect(const Vec2d& point, Rect& rect);

		static void GetProjections(const std::vector<Vec2d>& vertices, const Vec2d& axis, double& outMin, double& outMax);
		static void GetProjections(const Vec2d& origin, const double radius, const Vec2d& axis, double& outMin, double& outMax);
	};

}