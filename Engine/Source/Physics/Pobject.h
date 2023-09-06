#pragma once
#include "Polygon/Polygon.h"
#include "Physics/Translation.h"

namespace Radiant {
	class Pobject {
	private:
		Polygon m_polygon;
	public:
		Translation translation;
		
		Pobject() {}
		Pobject(const Polygon& polygon);
		~Pobject();

		const Polygon& GetPolygon() { return m_polygon; }

		friend class Physics;
		friend class Collision;
	};
}