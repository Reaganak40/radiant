#pragma once
#include "Polygon/Polygon.h"
#include "Polygon/Rect.h"
#include "Polygon/Circle.h"

#include "Pobject.h"

namespace rdt {
	class Collision {
	private:
	public:
		Collision() {}
		~Collision() {}


		/*
			Takes two physics objects and chooses the best collision test for the two, returns
			true if there was a collision.
		*/
		static bool CheckCollision(Pobject& source, const Pobject& suspect, const float deltaTime);

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
		static bool CheckCollisionAABB(const Pobject& A, const Pobject& B);

		/*
			Resolves static collisions, and returns true if there was collision resolution.
		*/
		static bool StaticCollisionDiags(Polygon& dynamicPoly, Polygon& staticPoly);

		/*
			Detects and resolves collisions between two rectangles using swept  AABB algorithm.
		*/
		static bool SweptAABB(Pobject& source, const Pobject& suspect, const float deltaTime);

		static bool PointVsRect(const Vec2d& point, Rect& rect);
		
		static bool RayVsRect(const Vec2d& start, const Vec2d& ray, const Vec2d& rectTopLeft, const Vec2d& rectBottomRight, 
			Vec2d& contactPoint, Vec2d& contactNormal, float& contactTime);

	private:
		static void GetProjections(const std::vector<Vec2d>& vertices, const Vec2d& axis, double& outMin, double& outMax);
		static void GetProjections(const Vec2d& origin, const double radius, const Vec2d& axis, double& outMin, double& outMax);
	};

}