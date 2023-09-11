#pragma once
#include "Utils/MathTypes.h"
#include "Utils/UniqueID.h"
#include "Texture/Texture.h"

namespace rdt {

	struct Vertex {
		Vec3f position;			// x,y,z coordinats of a vertex
		Vec4f color;			// the rgba (normalized) color values of a vertex
		Vec2f texCoords;		// the texture mapping coordinates for this vertex
		unsigned int texIndex;  // the texture slot that the target texture is currently binded.

		Vertex(Vec3f nPosition, Vec4f nColor, Vec2f nTexCoords, unsigned int nTexIndex)
			: position(nPosition), color(nColor), texCoords(nTexCoords), texIndex(nTexIndex) {}
	};

	struct Mesh {
		UniqueID m_UUID;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		unsigned int layer;
		Texture* texture;
		Vec2i texAtlasCoords;

		Mesh() : m_UUID(GetUniqueID()), layer(0), texture(nullptr), texAtlasCoords(Vec2i::Zero()) {}
	};
}