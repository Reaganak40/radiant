#pragma once
#include "Polygon/Polygon.h"
#include "Physics/Translation.h"

namespace Radiant {
	class Pobject {
	private:
		std::shared_ptr<Polygon> m_polygon;
	public:
		Translation translation;
		
		Pobject(std::shared_ptr<Polygon> m_polygon = std::shared_ptr<Polygon>());
		~Pobject();

		const Polygon& GetPolygon() { return *m_polygon; }

		friend class Physics;
		friend class Collision;
	};
}