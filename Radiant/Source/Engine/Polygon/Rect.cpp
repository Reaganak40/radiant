#include "pch.h"
#include "Rect.h"
#include "PolygonImpl.h"

namespace rdt {
	
	Rect::Rect(Vec2d spawnLocation, double width, double height)
	{
		AddProperties(IsRect);

		BaseImpl()->m_origin = spawnLocation;
		BaseImpl()->m_width = width;
		BaseImpl()->m_height = height;

		BaseImpl()->m_vertices.push_back(Vec2d(BaseImpl()->m_origin.x - (width / 2), BaseImpl()->m_origin.y - (height / 2)));
		BaseImpl()->m_vertices.push_back(Vec2d(BaseImpl()->m_origin.x + (width / 2), BaseImpl()->m_origin.y - (height / 2)));
		BaseImpl()->m_vertices.push_back(Vec2d(BaseImpl()->m_origin.x + (width / 2), BaseImpl()->m_origin.y + (height / 2)));
		BaseImpl()->m_vertices.push_back(Vec2d(BaseImpl()->m_origin.x - (width / 2), BaseImpl()->m_origin.y + (height / 2)));

		BaseImpl()->m_indices.push_back(0);
		BaseImpl()->m_indices.push_back(1);
		BaseImpl()->m_indices.push_back(2);
		BaseImpl()->m_indices.push_back(2);
		BaseImpl()->m_indices.push_back(3);
		BaseImpl()->m_indices.push_back(0);
	}

	void Rect::SetSize(const Vec2d& nSize)
	{
		if (nSize.x == BaseImpl()->m_width && nSize.y == BaseImpl()->m_height) {
			return;
		}

		BaseImpl()->m_width = nSize.x;
		BaseImpl()->m_height = nSize.y;

		BaseImpl()->m_vertices[0] = Vec2d(BaseImpl()->m_origin.x - (BaseImpl()->m_width / 2), BaseImpl()->m_origin.y - (BaseImpl()->m_height / 2));
		BaseImpl()->m_vertices[1] = Vec2d(BaseImpl()->m_origin.x + (BaseImpl()->m_width / 2), BaseImpl()->m_origin.y - (BaseImpl()->m_height / 2));
		BaseImpl()->m_vertices[2] = Vec2d(BaseImpl()->m_origin.x + (BaseImpl()->m_width / 2), BaseImpl()->m_origin.y + (BaseImpl()->m_height / 2));
		BaseImpl()->m_vertices[3] = Vec2d(BaseImpl()->m_origin.x - (BaseImpl()->m_width / 2), BaseImpl()->m_origin.y + (BaseImpl()->m_height / 2));
	}
}