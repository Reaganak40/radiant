#pragma once
#include "Polygon/Polygon.h"
#include "Translation.h"
#include "Ptag.h"

namespace rdt {

	class Physics;
}

namespace rdt::core {

	
	class Pobject {
	private:
		std::shared_ptr<Polygon> m_polygon;
		Translation translation;
		PhysicalProperties m_properties;

		std::set<UniqueID> m_tags;
		Vec2d m_hitbox_size;
	public:
		
		Pobject(std::shared_ptr<Polygon> m_polygon = std::shared_ptr<Polygon>());
		~Pobject();

		const Polygon& GetPolygon() const { return *m_polygon; }
		const Vec2d& GetHitBoxSize() const { return m_hitbox_size; }

		double GetHitboxWidth();
		double GetHitboxHeight();

		double GetHitboxSize();

		void GetHitBox(std::vector<Vec2d>& vertices) const;

		void SetProperties(PhysicalProperties nProperties);
		void RemoveProperties(PhysicalProperties rProperties);
		bool HasProperties(PhysicalProperties propertyQuery);

		void AddTag(UniqueID ntag);

		bool GetSharedTags(const Pobject& oObject, std::vector<UniqueID>& tags);

		void SetHitBoxSize(const Vec2d& nSize);

		void SetGravity(double mps2);


		friend class Realm;
		friend class Physics;
		friend class Collision;
	};
}