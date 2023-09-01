#include "pch.h"
#include "Mesh.h"

MeshCache::MeshCache()
{
}

MeshCache::~MeshCache()
{
}

Mesh& MeshCache::GetMesh(UniqueID meshIdentifier)
{
	if (m_cache.find(meshIdentifier) == m_cache.end()) {
		m_cache[meshIdentifier] = Mesh();
	}

	return m_cache.at(meshIdentifier);
}
