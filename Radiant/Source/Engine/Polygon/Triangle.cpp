#include "pch.h"
#include "Triangle.h"

namespace rdt {
	Triangle::Triangle(Vec2d spawnLocation, double width, double height)
	{
		m_origin = spawnLocation;

		m_vertices.push_back(Vec2d(m_origin.x - (width / 2), m_origin.y - (height / 2)));
		m_vertices.push_back(Vec2d(m_origin.x + (width / 2), m_origin.y - (height / 2)));
		m_vertices.push_back(Vec2d(m_origin.x, m_origin.y + (height / 2)));

		m_indices.push_back(0);
		m_indices.push_back(1);
		m_indices.push_back(2);
	}
}