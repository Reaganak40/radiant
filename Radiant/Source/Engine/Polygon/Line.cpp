#include "pch.h"
#include "Line.h"

namespace rdt {
	
	Line::Line(const Vec2d& start, const Vec2d& end)
	{
		AddProperties(IsLine);

		m_origin = start;
		m_width = abs(end.x - start.x);
		m_height = abs(end.y - start.y);

		m_vertices.push_back(start);
		m_vertices.push_back(end);

		m_indices.push_back(0);
		m_indices.push_back(1);
	}
	void Line::SetStart(const Vec2d& start)
	{
		m_vertices[0] = start;
	}
	void Line::SetEnd(const Vec2d& end)
	{
		m_vertices[1] = end;
	}
}
