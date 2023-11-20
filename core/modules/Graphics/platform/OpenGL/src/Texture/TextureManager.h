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

// Forward Declarations
namespace glCore {
	using TextureSlot = unsigned int;
	using TextureID = unsigned int;
	class Texture;
}

// Indicates that no texture is identified with this ID.
#define GL_CORE_NULL_TEXTURE_ID 0

// Max textures supported at a given time
#define MAX_TEXTURES 30

namespace glCore {
	
	using TextureSlotMap = std::array<unsigned int, MAX_TEXTURES>;

	class TextureManager {
	private:
		// Texture Storage and Lookup
		std::unordered_map<TextureID, Texture> m_textures;

		// OpenGL texture slot map
		TextureSlotMap m_texture_slots;
		TextureSlot m_next_slot;
		bool m_texture_slots_changed;

		TextureID idCounter = 0;

	public:
		TextureManager();
		~TextureManager();

		/*
			Loads a texture from a png file. Returns the TextureID for future
			lookups.
		*/
		TextureID LoadTextureFromPNG(const std::string& filepath);

		/*
			Returns true if a texture with the provided ID exists.
		*/
		bool TextureExists(TextureID tID);

		/*
			Returns a reference to a texture through its textureID
		*/
		Texture& GetTexture(TextureID tID);

		/*
			Returns the current texture slot map configuration
		*/
		TextureSlotMap& GetTextureSlotMap();

		/*
			Binds the texture indicated by the provided ID to a texture slot.
			Returns true if the texure slot map has changed. 
		*/
		bool BindTexture(TextureID tID);

		/*
			Binds all thw textures in the given set. If the number of textures
			in the set exceeds the maximum textures, none will be set.
		*/
		void BindTextures(const std::unordered_set<TextureID>& textures);

		/*
			Gets the texture slot for a texure through its textureID.
			Returns RDT_NULL_TEXTURE_SLOT if not found.
		*/
		TextureSlot GetTextureSlot(TextureID tID);

		/*
			Returns true if the texture slots have been changed since the last reset.
		*/
		bool TextureSlotsChanged();

		/*
			Resets the flags of the texture manager.
		*/
		void Reset();
	private:

		TextureID NextTextureID();
		TextureSlot NextTextureSlot();
		void AddNoneTexture();
	};
}