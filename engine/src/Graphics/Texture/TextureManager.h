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
	typedef unsigned int TextureSlot;
	struct AtlasProfile;
	class TextureAtlas;
	class Texture;

	namespace core {
		class RendererGL;
		struct Vertex;
		class glDrawCall;
		class Editor;
	}
}

// Indicates that no texture is identified with this ID.
#define RDT_NULL_TEXTURE_ID 0

// Max textures supported at a given time
#define MAX_TEXTURES 30

namespace rdt {
	
	using TextureID = unsigned int;   // Unique Identifier for a Texture
	using TextureSlotMap = std::array<unsigned int, MAX_TEXTURES>;

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
			Returns the name of the texture registered with this textureID
		*/
		static const char* GetTextureAlias(TextureID tID);

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

		/*
			Sets an atlas profile to indicate that it is not using
			an atlas. (will use the full dimensions of the texture)
		*/
		static AtlasProfile NOT_USING_ATLAS();

		friend class Animation;
		friend class AnimationManager;
		friend class TextureAtlas;
		friend class core::RendererGL;
		friend class core::glDrawCall;
		friend class core::Editor;
	private:
		
		/*
			Returns a reference to a texture through its textureID
		*/
		static Texture& GetTexture(TextureID tID);

		/*
			Returns the name/alias of a texture through its TextureID
		*/
		static const char* GetAlias(TextureID tID);

		/*
			Returns the current texture slot map configuration
		*/
		static TextureSlotMap& GetTextureSlotMap();

		/*
			Binds the texture indicated by the provided ID to a texture slot.
			Returns true if the texure slot map has changed. 
		*/
		static bool BindTexture(TextureID tID);

		/*
			Gets the texture slot for a texure through its textureID.
			Returns RDT_NULL_TEXTURE_SLOT if not found.
		*/
		static TextureSlot GetTextureSlot(TextureID tID);

		void AddNoneTexture();
	};
}