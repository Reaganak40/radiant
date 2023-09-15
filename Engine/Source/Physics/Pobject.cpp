#include "pch.h"
#include "Pobject.h"

namespace rdt {
	Pobject::Pobject(std::shared_ptr<Polygon> polygon)
		: m_polygon(polygon), translation(Vec2d(0, 0), Vec2d(0, 0)), m_properties(0), m_hitbox_size(1, 1)
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

	void Pobject::AddTag(Ptag ntag)
	{
		m_tags.insert(ntag);
	}
	bool Pobject::ShareTags(const Pobject& oObject)
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

		for (const auto& tag : lesser->m_tags) {
			if (bigger->m_tags.contains(tag)) {
				return true;
			}
		}

		return false;
	}

	void Pobject::ResetCollisions()
	{
		m_collided_objects.clear();
	}

	void Pobject::AddCollision(const UniqueID objectID)
	{
		m_collided_objects.insert(objectID);
	}

	bool Pobject::IsCollidedWith(const UniqueID objectID)
	{
		return m_collided_objects.contains(objectID);
	}

	bool Pobject::CollisionDetected()
	{
		return m_collided_objects.size();
	}
	void Pobject::SetHitBoxSize(const Vec2d& nSize)
	{
		m_hitbox_size = nSize;
	}
}