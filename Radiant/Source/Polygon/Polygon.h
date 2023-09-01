#pragma once
#include "pch.h"
#include "Math/MathTypes.h"
#include "UniqueID.h"

class Polygon {
private:
	UniqueID m_UUID;
protected:
	std::vector<Vec2d> m_vertices;
	std::vector<unsigned int> m_indices;

	Vec2d m_origin;
public:
	Polygon() : m_UUID(GetUniqueID()) {}
	~Polygon() {}

	const std::vector<Vec2d>& GetVertices() const;
	const std::vector<unsigned int> GetIndices() const;
	const UniqueID GetUUID() const;

	void Move(double dx, double dy);
};