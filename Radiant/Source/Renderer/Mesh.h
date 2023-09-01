#pragma once
#include "Utils/MathTypes.h"
#include "Utils/UniqueID.h"

namespace Radiant {

	struct Vertex {
		Vec3f position;
		Vec4f color;

		Vertex(Vec3f nPosition, Vec4f nColor)
			: position(nPosition), color(nColor) {}
	};

	struct Mesh {
		UniqueID m_UUID;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		unsigned int layer;

		Mesh() : m_UUID(GetUniqueID()), layer(0) {}
	};




	class MeshCache {
	private:
		std::unordered_map<UniqueID, Mesh> m_cache;
	public:
		MeshCache();
		~MeshCache();

		Mesh& GetMesh(UniqueID meshIdentifier);
	};
}