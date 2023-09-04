#include "pch.h"
#include "Pobject.h"

namespace Radiant {
	Pobject::Pobject(const Polygon& polygon)
		: m_polygon(polygon), translation(Vec2d(0, 0), Vec2d(0, 0))
	{
	}

	Pobject::~Pobject()
	{
	}
}