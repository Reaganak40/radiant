#pragma once
#include "Utils/UniqueID.h"
#include "Utils/MathTypes.h"

namespace rdt::core {
	struct PolygonImpl {
		UniqueID m_UUID;
		double m_rotation;
		unsigned int m_properties;
		std::vector<Vec2d> m_vertices;
		std::vector<unsigned int> m_indices;

		Vec2d m_origin;
		double m_width;
		double m_height;

		PolygonImpl();
		~PolygonImpl();

		void Copy(const PolygonImpl* src);
	};
}