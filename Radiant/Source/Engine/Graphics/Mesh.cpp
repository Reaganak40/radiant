#include "pch.h"
#include "Mesh.h"

#include "Utils/UniqueID.h"
#include "Texture/TextureManager.h"
#include "Model.h"
#include "ECS/CommonComponents.h"

namespace rdt::core {
	Mesh::Mesh()
	{
		Reset();
	}
	void Mesh::Reset()
	{
		modelID = RDT_NULL_MODEL_ID;
		layer = 0;
		textureID = RDT_NULL_TEXTURE_ID;
		atlasProfile = TextureManager::NOT_USING_ATLAS();
		flipTexture = false;

		position = Vec2f::Zero();
		rotation = 0.0f;
		scale = Vec2f::Zero();

		fillColor.x1 = 0.0f;
		fillColor.x2 = 0.0f;
		fillColor.x3 = 0.0f;
		fillColor.x4 = 1.0f;
	}

	void Mesh::UseTransform(const Transform& transform)
	{
		position = transform.position;
		rotation = transform.rotation;
		scale = transform.scale;
	}
}
