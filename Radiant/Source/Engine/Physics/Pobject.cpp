#include "pch.h"
#include "Pobject.h"
#include "Utils/Utils.h"

namespace rdt::core {
	Pobject::Pobject(std::shared_ptr<Polygon> polygon)
		: m_polygon(polygon), translation(Vec2d(0, 0), Vec2d(0, 0)), m_properties(NoProperties), m_hitbox_size(1, 1)
	{
	}

	Pobject::~Pobject()
	{
	}

	double Pobject::GetHitboxWidth()
	{
		return m_polygon->GetWidth() * m_hitbox_size.x;
	}

	double Pobject::GetHitboxHeight()
	{
		return m_polygon->GetHeight() * m_hitbox_size.y;
	}

	double Pobject::GetHitboxSize()
	{
		return 0.0;
	}

	void Pobject::GetHitBox(std::vector<Vec2d>& vertices) const
	{
		vertices = m_polygon->GetVertices();
		if (m_hitbox_size.x == 1 && m_hitbox_size.y == 1) {
			return;
		}
		
		Vec2d origin = m_polygon->GetOrigin();
		for (int i = 0; i < vertices.size(); i++) {
			Vec2d diff = Utils::GetManhattanDistance(origin, vertices[i]);
			diff.x *= m_hitbox_size.x;
			diff.y *= m_hitbox_size.y;
			vertices[i] = origin + diff;
		}
	}

	void Pobject::SetProperties(PhysicalProperties nProperties)
	{
		m_properties |= nProperties;

		if (bool(nProperties & ppNoGravity)) {
			translation.SetGravity(0);
		}
	}

	void Pobject::RemoveProperties(PhysicalProperties rProperties)
	{
		m_properties ^= rProperties;
	}
	bool Pobject::HasProperties(PhysicalProperties propertyQuery)
	{
		return bool(m_properties & propertyQuery);
	}

	void Pobject::AddTag(UniqueID ntag)
	{
		m_tags.insert(ntag);
	}
	bool Pobject::GetSharedTags(const Pobject& oObject, std::vector<UniqueID>& tags)
	{
		const Pobject* lesser;
		const Pobject* bigger;

		if (oObject.m_tags.size() < m_tags.size()) {
			lesser = &oObject;
			bigger = this;
		}
		else {
			lesser = this;
			bigger = &oObject;
		}

		bool hasSharedTag = false;
		for (const auto& tag : lesser->m_tags) {
			if (bigger->m_tags.contains(tag)) {
				
				tags.push_back(tag);
				hasSharedTag = true;
			}
		}

		return hasSharedTag;
	}

	void Pobject::SetHitBoxSize(const Vec2d& nSize)
	{
		m_hitbox_size = nSize;
	}
	void Pobject::SetGravity(double mps2)
	{
		if (!HasProperties(ppNoGravity)) {
			translation.SetGravity(mps2);
		}
	}
}