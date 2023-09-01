#pragma once
#include "Math/MathTypes.h"
#include "UniqueID.h"

struct Vertex {
	Vec3f position;

	Vertex(Vec3f nPosition)
		: position(nPosition) {}
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