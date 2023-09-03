#include "pch.h"
#include "Collision.h"
#include "Utils/MathTypes.h"

bool Radiant::Collision::CheckCollisionSAT(const Polygon& A, const Polygon& B)
{
	/*
		Step 1: Get the normal of each edge.
	*/
	std::vector<Vec2d> A_normals;
	std::vector<Vec2d> B_normals;

	unsigned int A_vertex_count = A.GetVertices().size();
	for (unsigned int i = 0; i < A_vertex_count; i++) {
		A_normals.push_back(Vec2d(
			A.GetVertices().at((i + 1) % A_vertex_count).x - A.GetVertices().at(i).x,
			A.GetVertices().at((i + 1) % A_vertex_count).y - A.GetVertices().at(i).y).Normal());
	}

	unsigned int B_vertex_count = B.GetVertices().size();
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

void Radiant::Collision::GetProjections(const std::vector<Vec2d>& vertices, const Vec2d& axis, double& outMin, double& outMax)
{
	outMin = std::numeric_limits<float>::max();
	outMax = std::numeric_limits<float>::min();

	for (const auto& vertex : vertices) {
		double product = Vec2d::DotProduct(vertex, axis);

		outMin = (product < outMin ? product : outMin);
		outMax = (product > outMax ? product : outMax);

	}
}