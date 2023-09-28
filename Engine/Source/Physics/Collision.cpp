#include "pch.h"
#include "Collision.h"
#include "Utils/MathTypes.h"
#include "Utils/Utils.h"
#include "Renderer/Renderer.h"
#include "Utils/Input.h"

bool rdt::Collision::CheckCollision(Pobject& source, const Pobject& suspect, const float deltaTime)
{
	
	if (source.m_polygon->CheckProperties(IsRect) && suspect.m_polygon->CheckProperties(IsRect)) {

		if (source.m_polygon->GetRotation() == 0 && suspect.m_polygon->GetRotation() == 0) {
			
			if (source.HasProperties(DontResolve)) {
				return CheckCollisionAABB(source, suspect);
			}

			return SweptAABB(source, suspect, deltaTime);
		}
	}

	return false;
}

bool rdt::Collision::CheckCollisionSAT(const Polygon& A, const Polygon& B)
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

bool rdt::Collision::CheckCollisionSAT(const Circle& A, const Polygon& B)
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

bool rdt::Collision::CheckCollisionAABB(const Pobject& A, const Pobject& B)
{
	std::vector<Vec2d> A_vertices;
	std::vector<Vec2d> B_vertices;

	A.GetHitBox(A_vertices);
	B.GetHitBox(B_vertices);

	return !((A_vertices[1].x < B_vertices[0].x || B_vertices[1].x < A_vertices[0].x) ||
		(A_vertices[2].y < B_vertices[1].y || B_vertices[2].y < A_vertices[1].y));
}

bool rdt::Collision::StaticCollisionDiags(Polygon& dynamicPoly, Polygon& staticPoly)
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

bool rdt::Collision::SweptAABB(Pobject& source, const Pobject& suspect, const float deltaTime)
{
	/*
		Rect:

		sp*------
		  |     |
		  |     |
		  |     |
		  ------* ep
	*/
	if (source.translation.GetVelocity() == Vec2d::Zero()) {
		return false;
	}

	Vec2d ray = source.translation.GetChangeInPosition(deltaTime);

	Vec2d sp = suspect.m_polygon->GetVertices()[3];
	Vec2d ep = suspect.m_polygon->GetVertices()[1];

	sp.x = ceil(sp.x - source.m_polygon->GetWidth() / 2);
	sp.y = ceil(sp.y + source.m_polygon->GetHeight() / 2);

	ep.x = ceil(ep.x + source.m_polygon->GetWidth() / 2);
	ep.y = ceil(ep.y - source.m_polygon->GetHeight() / 2);

	Vec2d start = source.m_polygon->GetOrigin();
	Vec2d contactPoint;
	Vec2d contactNormal;
	float contactTime = 1.0f;

	if (RayVsRect(start, ray, sp, ep, contactPoint, contactNormal, contactTime)) {

		if (source.HasProperties(ppBouncy)) {
			
			source.translation.Translate(*source.m_polygon, (deltaTime * contactTime));

			if (abs(contactNormal.x) > 0) {
				source.translation.m_current_velocity.x *= -1.0;
				source.translation.m_acceleration.x *= -1.0;

			} 
			if (abs(contactNormal.y) > 0) {
				source.translation.m_current_velocity.y *= -1.0;
				source.translation.m_acceleration.y *= -1.0;
			}

			source.translation.Translate(*source.m_polygon, deltaTime * (1-contactTime));

		}
		else {
			source.translation.m_current_velocity += contactNormal * Vabs(source.translation.m_current_velocity) * (1 - contactTime);
		}
		return true;
	}

	return false;
}

bool rdt::Collision::PointVsRect(const Vec2d& point, Rect& rect)
{
	const std::vector<Vec2d>& vertices = rect.GetVertices();
	return (point.x >= vertices[0].x && point.x <= vertices[1].x && point.y >= vertices[1].y && point.y <= vertices[2].y);
}

bool rdt::Collision::RayVsRect(const Vec2d& start, const Vec2d& ray, const Vec2d& rectTopLeft, const Vec2d& rectBottomRight,
	Vec2d& contactPoint, Vec2d& contactNormal, float& contactTime)
{
	Vec2d tNear = (rectTopLeft - start) / ray;
	Vec2d tFar = (rectBottomRight - start) / ray;

	// Check for division by 0
	if (std::isnan(tNear.x)) { tNear.x = -std::numeric_limits<double>::infinity(); }
	if (std::isnan(tNear.y)) { tNear.y = -std::numeric_limits<double>::infinity(); }
	if (std::isnan(tFar.x))  { tFar.x  =  std::numeric_limits<double>::infinity(); }
	if (std::isnan(tFar.y))  { tFar.y  =  std::numeric_limits<double>::infinity(); }

	// Sort
	if (tNear.x > tFar.x) { Utils::Swap(tNear.x, tFar.x); }
	if (tNear.y > tFar.y) { Utils::Swap(tNear.y, tFar.y); }

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
	} else if (tNear.x < tNear.y) {
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

void rdt::Collision::GetProjections(const std::vector<Vec2d>& vertices, const Vec2d& axis, double& outMin, double& outMax)
{
	outMin = std::numeric_limits<float>::max();
	outMax = std::numeric_limits<float>::min();

	for (const auto& vertex : vertices) {
		double product = Vec2d::Dot(vertex, axis);

		outMin = (product < outMin ? product : outMin);
		outMax = (product > outMax ? product : outMax);

	}
}

void rdt::Collision::GetProjections(const Vec2d& origin, const double radius, const Vec2d& axis, double& outMin, double& outMax)
{
	Vec2d direction = axis.Normalize();
	Vec2d directionAndRadius = direction * radius;

	Vec2d p1 = origin + directionAndRadius;
	Vec2d p2 = origin - directionAndRadius;

	outMin = Vec2d::Dot(p1, axis);
	outMax = Vec2d::Dot(p2, axis);

	if (outMin > outMax) {
		Utils::Swap(outMin, outMax);
	}
}
