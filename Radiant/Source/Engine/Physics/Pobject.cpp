#include "pch.h"
#include "Pobject.h"

namespace rdt::core {
	Pobject::Pobject(std::shared_ptr<Polygon> polygon)
		: m_polygon(polygon), translation(Vec2d(0, 0), Vec2d(0, 0)), m_properties(NoProperties), m_hitbox_size(1, 1)
	{
	}

	Pobject::~Pobject()
	{
	}

	void Pobject::GetHitBox(std::vector<Vec2d>& vertices) const
	{
		if (!m_polygon->CheckProperties(IsRect)) {
			return;
		}
		vertices = m_polygon->GetVertices();

		if (m_hitbox_size.x != 1 || m_hitbox_size.y != 1) {
			Vec2d origin = m_polygon->GetOrigin();
			double width = vertices[1].x - vertices[0].x;
			double height = vertices[2].y - vertices[1].y;
			width *= m_hitbox_size.x;
			height *= m_hitbox_size.y;

			vertices[0].x = origin.x - (width / 2);
			vertices[1].x = origin.x + (width / 2);
			vertices[2].x = origin.x + (width / 2);
			vertices[3].x = origin.x - (width / 2);

			vertices[0].y = origin.y - (height / 2);
			vertices[1].y = origin.y - (height / 2);
			vertices[2].y = origin.y + (height / 2);
			vertices[3].y = origin.y + (height / 2);
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