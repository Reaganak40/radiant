/*******************************************************************
*	Module:  Graphics (API)
*	File:    TextureManager.h
*
*	Author: Reagan Kelley
*
*   TextureManager is a core singleton that controls the creation,
*	management, and destruction of textures and texture atlases.
*******************************************************************/
#pragma once
#include "Core.h"

// Forward Declarations
namespace rdt {
	using TextureID = unsigned int;   // Unique Identifier for a Texture
	struct AtlasProfile;
	class TextureAtlas;
	class Texture;

	namespace core {
		class RendererGL;
		class RenderLayer;
		struct Mesh;
	}
}

// Indicates that no texture is identified with this ID.
#define RDT_NULL_TEXTURE_ID 0

namespace rdt {

	class RADIANT_API TextureManager {
	private:
		struct Impl;
		static Impl* m_impl;

	public:
		/*
			Creates a new instance of the TextureManager.
		*/
		static void Initialize();

		/*
			Destroys the current instance of the TextureManger.
		*/
		static void Destroy();

		/*
			Loads a texture from a png file. Returns the TextureID for future
			lookups.
		*/
		static TextureID LoadTextureFromPNG(const std::string& name, const std::string& filepath);

		/*
			Returns the TextureID belonging to the provided name/alias. Returns 0
			if not found.
		*/
		static TextureID GetTextureID(const std::string& name);

		/*
			Returns true if a texture with the provided ID exists.
		*/
		static bool TextureExists(TextureID tID);

		/*
			Assigns a texture atlas to the indicated texture. Returns a reference
			to it, so it can be further configurated and compiled.
		*/
		static TextureAtlas& InitTextureAtlas(TextureID tID);

		/*
			Returns a shared pointer to an already initialized texture atlas
			for the given the texture. Otherwise, returns nullptr.
		*/
		static std::shared_ptr<TextureAtlas> GetTextureAtlas(TextureID tID);

		friend class Animation;
		friend class AnimationManager;
		friend class TextureAtlas;
		friend class core::RendererGL;
		friend class core::RenderLayer;
		friend struct core::Mesh;
	private:
		
		/*
			Sets an atlas profile to indicate that it is not using
			an atlas. (will use the full dimensions of the texture)
		*/
		static AtlasProfile NOT_USING_ATLAS();

		/*
			Returns a reference to a texture through its textureID
		*/
		static Texture& GetTexture(TextureID tID);

		/*
			Returns the name/alias of a texture through its TextureID
		*/
		static const char* GetAlias(TextureID tID);

		/*
			Applies the provided texture to the vertices of the provided rectangle. If the texture
			has a texture atlas, the atlasCoords will be applied to get the correct sprite in the
			tile map.
			Returns true if the texture slots changed.
		*/
		static bool ApplyTextureAtlas(Texture* texture, const Vec2i& atlasCoords, std::vector<core::Vertex>& rectVertices, bool flipHorizontal = false);
		
		static std::array<unsigned int, MAX_TEXTURES>& GetTextureSlots();

		void AddNoneTexture();

		TextureSlot GetNextSlot();

	};
}