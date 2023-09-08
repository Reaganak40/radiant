#include "pch.h"
#include "Pobject.h"

namespace Radiant {
	Pobject::Pobject(std::shared_ptr<Polygon> polygon)
		: m_polygon(polygon), translation(Vec2d(0, 0), Vec2d(0, 0)), m_properties(0)
	{
	}

	Pobject::~Pobject()
	{
	}

	void Pobject::SetProperties(const unsigned int nProperties)
	{
		m_properties |= nProperties;
	}

	void Pobject::RemoveProperties(const unsigned int rProperties)
	{
		m_properties ^= rProperties;
	}
	bool Pobject::HasProperties(const unsigned int propertyQuery)
	{
		return bool(m_properties & propertyQuery);
	}
}