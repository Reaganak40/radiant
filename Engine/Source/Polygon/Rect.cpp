#include "pch.h"
#include "Rect.h"

namespace rdt {
	
	Rect::Rect(Vec2d spawnLocation, double width, double height)
	{
		AddProperties(IsRect);

		m_origin = spawnLocation;
		m_width = width;
		m_height = height;

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

	void Rect::SetPosition(const Vec2d& nPosition)
	{
		if (m_origin == nPosition) {
			return;
		}

		m_origin = nPosition;
		m_vertices[0] = Vec2d(m_origin.x - (m_width / 2), m_origin.y - (m_height / 2));
		m_vertices[1] = Vec2d(m_origin.x + (m_width / 2), m_origin.y - (m_height / 2));
		m_vertices[2] = Vec2d(m_origin.x + (m_width / 2), m_origin.y + (m_height / 2));
		m_vertices[3] = Vec2d(m_origin.x - (m_width / 2), m_origin.y + (m_height / 2));
	}

	void Rect::SetSize(const Vec2d& nSize)
	{
		if (nSize.x == m_width && nSize.y == m_height) {
			return;
		}

		m_width = nSize.x;
		m_height = nSize.y;

		m_vertices[0] = Vec2d(m_origin.x - (m_width / 2), m_origin.y - (m_height / 2));
		m_vertices[1] = Vec2d(m_origin.x + (m_width / 2), m_origin.y - (m_height / 2));
		m_vertices[2] = Vec2d(m_origin.x + (m_width / 2), m_origin.y + (m_height / 2));
		m_vertices[3] = Vec2d(m_origin.x - (m_width / 2), m_origin.y + (m_height / 2));
	}
}