#include "pch.h"
#include "Collision.h"
#include "Utils/MathTypes.h"
#include "Utils/Utils.h"
#include "Utils/Input.h"

namespace rdt::core {
	
	bool Collision::CheckCollision(CollisionObject& source, const CollisionObject& suspect, const float deltaTime)
	{
		if (source.isRect && suspect.isRect) {

			if (source.isAxisAligned && suspect.isAxisAligned) {

				if (!source.resolveCollision) {
					return CheckCollisionAABB(source, suspect);
				}

				return SweptAABB(source, suspect, deltaTime);
			}
		}

		return false;
	}

	bool Collision::CheckCollisionSAT(const Polygon& A, const Polygon& B)
	{
		/*
			Step 1: Get the normal of each edge.
		*/
		std::vector<Vec2d> A_normals;
		std::vector<Vec2d> B_normals;

		unsigned int A_vertex_count = (unsigned int)A.GetVertices().size();
		for (unsigned int i = 0; i < A_vertex_count; i++) {
			A_normals.push_back(Vec2d(
				A.GetVertices().at((i + 1) % A_vertex_count).x - A.GetVertices().at(i).x,
				A.GetVertices().at((i + 1) % A_vertex_count).y - A.GetVertices().at(i).y).Normal());
		}

		unsigned int B_vertex_count = (unsigned int)B.GetVertices().size();
		for (unsigned int i = 0; i < B_vertex_count; i++) {
			B_normals.push_back(Vec2d(
				B.GetVertices().at((i + 1) % B_vertex_count).x - B.GetVertices().at(i).x,
				B.GetVertices().at((i + 1) % B_vertex_count).y - B.GetVertices().at(i).y).Normal());
		}

		/*
			Step 2: Check each axis of each normal for seperation
		*/
		for (const auto& axis : A_normals) {
			double Amin, Amax;
			double Bmin, Bmax;

			GetProjections(A.GetVertices(), axis, Amin, Amax);
			GetProjections(B.GetVertices(), axis, Bmin, Bmax);

			if (Amax < Bmin || Bmax < Amin) {
				return false;
			}
		}

		for (const auto& axis : B_normals) {
			double Amin, Amax;
			double Bmin, Bmax;

			GetProjections(A.GetVertices(), axis, Amin, Amax);
			GetProjections(B.GetVertices(), axis, Bmin, Bmax);

			if (Amax < Bmin || Bmax < Amin) {
				return false;
			}
		}

		return true;
	}

	bool Collision::CheckCollisionSAT(const Circle& A, const Polygon& B)
	{
		/*
			Step 1: Get the normal of each edge for the polygon B.
		*/
		std::vector<Vec2d> B_normals;

		unsigned int B_vertex_count = (unsigned int)B.GetVertices().size();
		for (unsigned int i = 0; i < B_vertex_count; i++) {
			B_normals.push_back(Vec2d(
				B.GetVertices().at((i + 1) % B_vertex_count).x - B.GetVertices().at(i).x,
				B.GetVertices().at((i + 1) % B_vertex_count).y - B.GetVertices().at(i).y).Normal());
		}

		/*
			Step 2: Check each axis of each normal for seperation
		*/
		for (const auto& axis : B_normals) {
			double Amin, Amax;
			double Bmin, Bmax;

			GetProjections(A.GetOrigin(), A.GetRadius(), axis, Amin, Amax);
			GetProjections(B.GetVertices(), axis, Bmin, Bmax);

			if (Amax < Bmin || Bmax < Amin) {
				return false;
			}
		}

		/*
			Step 3: Check to see if the polygons closest vertex is in the radius of the circle.
		*/
		Vec2d closestVertex = Utils::GetClosestVertex(B.GetVertices(), A.GetOrigin());

		return true;
	}

	bool Collision::CheckCollisionAABB(const CollisionObject& source, const CollisionObject& suspect)
	{
		return !((source.vertices[1].x < suspect.vertices[0].x || suspect.vertices[1].x < source.vertices[0].x) ||
			(source.vertices[2].y < suspect.vertices[1].y || suspect.vertices[2].y < source.vertices[1].y));
	}

	bool Collision::StaticCollisionDiags(Polygon& dynamicPoly, Polygon& staticPoly)
	{
		Polygon* poly1;
		Polygon* poly2;
		bool displaced = false;

		for (int i = 0; i < 2; i++) {

			if (i == 0) {
				poly1 = &dynamicPoly;
				poly2 = &staticPoly;
			}
			else {
				poly1 = &staticPoly;
				poly2 = &dynamicPoly;
			}

			// Check diagnals of each polygon
			for (unsigned int i = 0; i < poly1->GetVertices().size(); i++) {
				Vec2d p1_lineP1 = poly1->GetOrigin();
				Vec2d p1_lineP2 = poly1->GetVertices()[i];

				Vec2d displacement = Vec2d(0, 0);

				for (unsigned int j = 0; j < poly2->GetVertices().size(); j++) {
					Vec2d p2_lineP1 = poly2->GetVertices()[j];
					Vec2d p2_lineP2 = poly2->GetVertices()[(j + 1) % poly2->GetVertices().size()];

					// Standard "off the shelf" line segment intersection
					double h = (p2_lineP2.x - p2_lineP1.x) * (p1_lineP1.y - p1_lineP2.y) - (p1_lineP1.x - p1_lineP2.x) * (p2_lineP2.y - p2_lineP1.y);
					double t1 = ((p2_lineP1.y - p2_lineP2.y) * (p1_lineP1.x - p2_lineP1.x) + (p2_lineP2.x - p2_lineP1.x) * (p1_lineP1.y - p2_lineP1.y)) / h;
					double t2 = ((p1_lineP1.y - p1_lineP2.y) * (p1_lineP1.x - p2_lineP1.x) + (p1_lineP2.x - p1_lineP1.x) * (p1_lineP1.y - p2_lineP1.y)) / h;

					if (t1 >= 0.0f && t1 < 1.0f && t2 >= 0.0f && t2 < 1.0f)
					{
						displacement.x += (1.0 - t1) * (p1_lineP2.x - p1_lineP1.x);
						displacement.y += (1.0 - t1) * (p1_lineP2.y - p1_lineP1.y);
						displaced = true;
					}
				}

				dynamicPoly.Move(displacement.x, displacement.y);
			}
		}

		return displaced;
	}

	bool Collision::SweptAABB(CollisionObject& source, const CollisionObject& suspect, const float deltaTime)
	{
		if (source.rigidBody->velocity == Vec2d::Zero()) {
			return false;
		}

		Vec2d ray = source.rigidBody->GetChangeInPosition(deltaTime);
		Vec2d sp = suspect.vertices[3];
		Vec2d ep = suspect.vertices[1];

		sp.x = ceil(sp.x - source.size.x / 2);
		sp.y = ceil(sp.y + source.size.y / 2);

		ep.x = ceil(ep.x + source.size.x / 2);
		ep.y = ceil(ep.y - source.size.y / 2);

		Vec2d start = source.midpoint;
		Vec2d contactPoint;
		Vec2d contactNormal;
		float contactTime = 1.0f;

		if (RayVsRect(start, ray, sp, ep, contactPoint, contactNormal, contactTime)) {

			source.rigidBody->velocity += contactNormal * Vabs(source.rigidBody->velocity) * (1 - contactTime);
			source.m_collision_detected = true;
			return true;
		}

		return false;
	}

	bool Collision::PointVsRect(const Vec2d& point, Rect& rect)
	{
		const std::vector<Vec2d>& vertices = rect.GetVertices();
		return (point.x >= vertices[0].x && point.x <= vertices[1].x && point.y >= vertices[1].y && point.y <= vertices[2].y);
	}

	bool Collision::RayVsRect(const Vec2d& start, const Vec2d& ray, const Vec2d& rectTopLeft, const Vec2d& rectBottomRight,
		Vec2d& contactPoint, Vec2d& contactNormal, float& contactTime)
	{
		Vec2d tNear = (rectTopLeft - start) / ray;
		Vec2d tFar = (rectBottomRight - start) / ray;

		// Check for division by 0
		if (std::isnan(tNear.x)) { tNear.x = -std::numeric_limits<double>::infinity(); }
		if (std::isnan(tNear.y)) { tNear.y = -std::numeric_limits<double>::infinity(); }
		if (std::isnan(tFar.x)) { tFar.x = std::numeric_limits<double>::infinity(); }
		if (std::isnan(tFar.y)) { tFar.y = std::numeric_limits<double>::infinity(); }

		// Sort
		if (tNear.x > tFar.x) { std::swap(tNear.x, tFar.x); }
		if (tNear.y > tFar.y) { std::swap(tNear.y, tFar.y); }

		// Check for intersection
		if (tNear.x > tFar.y || tNear.y > tFar.x) { return false; }

		if (tNear.x == -std::numeric_limits<double>::infinity() &&
			tFar.x == std::numeric_limits<double>::infinity()) {

			if (((start + ray).x < rectTopLeft.x) || ((start + ray).x > rectBottomRight.x)) {
				return false;
			}
		}

		if (tNear.y == -std::numeric_limits<double>::infinity() &&
			tFar.y == std::numeric_limits<double>::infinity()) {

			if (((start + ray).y > rectTopLeft.y) || ((start + ray).y < rectBottomRight.y)) {
				return false;
			}
		}


		double tHitNear = Utils::Max(tNear.x, tNear.y);
		double tHitFar = Utils::Min(tFar.x, tFar.y);

		// Collision not in direction of ray
		if (tHitNear < 0.0 || tHitNear > 1.0) {
			return false;
		}

		if ((tNear.x < 0.0 && tNear.y < 0.0)) {
			return false;
		}

		contactPoint = start + (tHitNear * ray);
		contactTime = (float)tHitNear;

		if (tNear.x > tNear.y) {
			if (ray.x < 0) {
				contactNormal = Vec2d(1, 0);
			}
			else {
				contactNormal = Vec2d(-1, 0);
			}
		}
		else if (tNear.x < tNear.y) {
			if (ray.y < 0) {
				contactNormal = Vec2d(0, 1);
			}
			else {
				contactNormal = Vec2d(0, -1);
			}
		}
		else {
			contactNormal = Vec2d::Zero();
		}

		return true;
	}

	void Collision::GetProjections(const std::vector<Vec2d>& vertices, const Vec2d& axis, double& outMin, double& outMax)
	{
		outMin = std::numeric_limits<float>::max();
		outMax = std::numeric_limits<float>::min();

		for (const auto& vertex : vertices) {
			double product = Vec2d::Dot(vertex, axis);

			outMin = (product < outMin ? product : outMin);
			outMax = (product > outMax ? product : outMax);

		}
	}

	void Collision::GetProjections(const Vec2d& origin, const double radius, const Vec2d& axis, double& outMin, double& outMax)
	{
		Vec2d direction = axis.Normalize();
		Vec2d directionAndRadius = direction * radius;

		Vec2d p1 = origin + directionAndRadius;
		Vec2d p2 = origin - directionAndRadius;

		outMin = Vec2d::Dot(p1, axis);
		outMax = Vec2d::Dot(p2, axis);

		if (outMin > outMax) {
			std::swap(outMin, outMax);
		}
	}
}