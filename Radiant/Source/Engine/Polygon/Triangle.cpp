#include "pch.h"
#include "Triangle.h"
#include "PolygonImpl.h"

namespace rdt {
	Triangle::Triangle(Vec2d spawnLocation, double width, double height)
	{
		BaseImpl()->m_origin = spawnLocation;

		BaseImpl()->m_vertices.push_back(Vec2d(BaseImpl()->m_origin.x - (width / 2), BaseImpl()->m_origin.y - (height / 2)));
		BaseImpl()->m_vertices.push_back(Vec2d(BaseImpl()->m_origin.x + (width / 2), BaseImpl()->m_origin.y - (height / 2)));
		BaseImpl()->m_vertices.push_back(Vec2d(BaseImpl()->m_origin.x, BaseImpl()->m_origin.y + (height / 2)));

		BaseImpl()->m_indices.push_back(0);
		BaseImpl()->m_indices.push_back(1);
		BaseImpl()->m_indices.push_back(2);
	}
}