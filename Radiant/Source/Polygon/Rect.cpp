#include "pch.h"
#include "Rect.h"

namespace Radiant {
	Rect::Rect(Vec2d spawnLocation, double width, double height)
	{
		m_origin = spawnLocation;

		m_vertices.push_back(Vec2d(m_origin.x - (width / 2), m_origin.y - (height / 2)));
		m_vertices.push_back(Vec2d(m_origin.x + (width / 2), m_origin.y - (height / 2)));
		m_vertices.push_back(Vec2d(m_origin.x + (width / 2), m_origin.y + (height / 2)));
		m_vertices.push_back(Vec2d(m_origin.x - (width / 2), m_origin.y + (height / 2)));

		m_indices.push_back(0);
		m_indices.push_back(1);
		m_indices.push_back(2);
		m_indices.push_back(2);
		m_indices.push_back(3);
		m_indices.push_back(0);
	}
}
