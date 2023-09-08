#pragma once
#include "Polygon/Polygon.h"
#include "Physics/Translation.h"

namespace Radiant {

	enum PhysicalProperties {
		ppRigid = 1,
		ppBouncy = 2
	};
	class Pobject {
	private:
		std::shared_ptr<Polygon> m_polygon;
		Translation translation;
		unsigned int m_properties;

	public:
		
		Pobject(std::shared_ptr<Polygon> m_polygon = std::shared_ptr<Polygon>());
		~Pobject();

		const Polygon& GetPolygon() { return *m_polygon; }

		void SetProperties(const unsigned int nProperties);
		void RemoveProperties(const unsigned int rProperties);
		bool HasProperties(const unsigned int propertyQuery);

		friend class Realm;
		friend class Physics;
		friend class Collision;
	};
}