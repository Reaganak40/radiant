#include "pch.h"
#include "Pobject.h"

namespace rdt {
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
}