#include "pch.h"
#include "Circle.h"

namespace Radiant {
	
	Circle::Circle(Vec2d spawnLocation, double radius)
		: m_radius(radius)
	{
		m_origin = spawnLocation;
		
		m_vertices.push_back(Vec2d(m_origin.x - radius, m_origin.y - radius));
		m_vertices.push_back(Vec2d(m_origin.x + radius, m_origin.y - radius));
		m_vertices.push_back(Vec2d(m_origin.x + radius, m_origin.y + radius));
		m_vertices.push_back(Vec2d(m_origin.x - radius, m_origin.y + radius));

		m_indices.push_back(0);
		m_indices.push_back(1);
		m_indices.push_back(2);
		m_indices.push_back(2);
		m_indices.push_back(3);
		m_indices.push_back(0);
	}

	Circle::~Circle()
	{
	}
	double Circle::GetRadius() const
	{
		return m_radius;
	}
}
