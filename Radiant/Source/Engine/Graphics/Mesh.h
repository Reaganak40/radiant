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

// Forward Declarations
namespace rdt {
	using UniqueID = unsigned int;
	using TextureID = unsigned int;   // Unique Identifier for a Texture
	using ModelID = unsigned int;

	struct Transform;
}

// Required Definitions for Struct/Class Members
#include "Utils/MathTypes.h"
#include "Texture/TextureAtlas.h"


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
		ModelID modelID;
		TextureID textureID;
		AtlasProfile atlasProfile;

		Vec2f position;
		float rotation;
		Vec2f scale;

		bool flipTexture;
		Vec4f fillColor;
		unsigned int layer;

		Mesh();

		/*
			Resets a mesh back to its default constructor
		*/
		void Reset();

		/*
			Applied a transform to the mesh
		*/
		void UseTransform(const Transform& transform);
	};
}