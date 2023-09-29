#pragma once
#include "Polygon/Polygon.h"
#include "Translation.h"
#include "Ptag.h"

namespace rdt {
	enum PhysicalProperties {
		NoCollision = 1,
		ppBouncy = 2,
		DontResolve = 4,
	};

	class Physics;
}

namespace rdt::core {

	
	class Pobject {
	private:
		std::shared_ptr<Polygon> m_polygon;
		Translation translation;
		unsigned int m_properties;

		std::set<Ptag> m_tags;
		Vec2d m_hitbox_size;
	public:
		
		Pobject(std::shared_ptr<Polygon> m_polygon = std::shared_ptr<Polygon>());
		~Pobject();

		const Polygon& GetPolygon() const { return *m_polygon; }
		const Vec2d& GetHitBoxSize() const { return m_hitbox_size; }

		void GetHitBox(std::vector<Vec2d>& vertices) const;

		void SetProperties(const unsigned int nProperties);
		void RemoveProperties(const unsigned int rProperties);
		bool HasProperties(const unsigned int propertyQuery);

		void AddTag(Ptag ntag);

		bool ShareTags(const Pobject& oObject);

		void SetHitBoxSize(const Vec2d& nSize);


		friend class Realm;
		friend class Physics;
		friend class Collision;
	};
}