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
	class Pobject {
	private:
		std::shared_ptr<Polygon> m_polygon;
		Translation translation;
		unsigned int m_properties;

		std::set<Ptag> m_tags;
		std::set<UniqueID> m_collided_objects;
	public:
		
		Pobject(std::shared_ptr<Polygon> m_polygon = std::shared_ptr<Polygon>());
		~Pobject();

		const Polygon& GetPolygon() { return *m_polygon; }

		void SetProperties(const unsigned int nProperties);
		void RemoveProperties(const unsigned int rProperties);
		bool HasProperties(const unsigned int propertyQuery);

		void AddTag(Ptag ntag);

		bool ShareTags(const Pobject& oObject);

		void ResetCollisions();
		void AddCollision(const UniqueID objectID);
		bool IsCollidedWith(const UniqueID objectID);
		bool CollisionDetected();

		friend class Realm;
		friend class Physics;
		friend class Collision;
	};
}