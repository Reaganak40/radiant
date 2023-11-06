#include "pch.h"
#include "Mesh.h"

#include "Utils/UniqueID.h"
#include "Texture/TextureManager.h"

namespace rdt::core {
	Mesh::Mesh()
	{
		m_UUID = GetUniqueID();
		layer = 0;
		textureID = RDT_NULL_TEXTURE_ID;
		atlasProfile = TextureManager::NOT_USING_ATLAS();
		flipTexture = false;
	}
	Mesh::~Mesh()
	{
		FreeUniqueID(m_UUID);
	}
}
