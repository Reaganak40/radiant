/*******************************************************************
*	Module:  Graphics (core)
*	File:    Mesh.h
*
*	Author: Reagan Kelley
*
*	This file contains the declaration of Vertex and Mesh, used
*	exclusively by the Renderer in the submission process. Mesh
*	acts as an intermediary between the game engine (main loop)
*	and the render pipeline.
*******************************************************************/
#pragma once
#include "Utils/MathTypes.h"
#include "Texture/TextureAtlas.h"

// Forward Declarations
namespace rdt {
	using UniqueID = unsigned int;
	using TextureID = unsigned int;   // Unique Identifier for a Texture
}
namespace rdt::core {

	struct Vertex {
		Vec3f position;			// x,y,z coordinats of a vertex
		Vec4f color;			// the rgba (normalized) color values of a vertex
		Vec2f texCoords;		// the texture mapping coordinates for this vertex
		float texIndex;         // the texture slot that the target texture is currently binded.

		Vertex(Vec3f nPosition, Vec4f nColor, Vec2f nTexCoords, float nTexIndex)
			: position(nPosition), color(nColor), texCoords(nTexCoords), texIndex(nTexIndex) {}
	};

	struct Mesh {
		UniqueID m_UUID;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		unsigned int layer;
		TextureID textureID;
		AtlasProfile atlasProfile;
		bool flipTexture;

		Mesh();
		~Mesh();
	};
}