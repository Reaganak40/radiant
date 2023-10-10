#include "pch.h"
#include "Line.h"
#include "PolygonImpl.h"
namespace rdt {
	
	Line::Line(const Vec2d& start, const Vec2d& end)
	{
		AddProperties(IsLine);

		BaseImpl()->m_origin = start;
		BaseImpl()->m_width = abs(end.x - start.x);
		BaseImpl()->m_height = abs(end.y - start.y);

		BaseImpl()->m_vertices.push_back(start);
		BaseImpl()->m_vertices.push_back(end);

		BaseImpl()->m_indices.push_back(0);
		BaseImpl()->m_indices.push_back(1);
	}
	void Line::SetStart(const Vec2d& start)
	{
		BaseImpl()->m_vertices[0] = start;
	}
	void Line::SetEnd(const Vec2d& end)
	{
		BaseImpl()->m_vertices[1] = end;
	}
}
