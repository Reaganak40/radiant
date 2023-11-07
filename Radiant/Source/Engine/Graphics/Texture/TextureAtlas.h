/*******************************************************************
*	Module:  Graphics
*	File:    TextureAtlas.h
*
*	Author: Reagan Kelley
*
*   Texture Atlases reference a texture, and defines a sprite
*	sheet, and creates a map between texture atlas coordinates
*	and texture image coordinates.
*******************************************************************/
#pragma once

// Forward Declarations
namespace rdt {
	using TextureID = unsigned int;   // Unique Identifier for a Texture
	class Vec2i;
}

namespace rdt {

	struct AtlasProfile {
		float normalizedStartX;
		float normalizedStartY;
		float normalizedWidth;
		float normalizedHeight;
	};

	class TextureAtlas {
	private:

		TextureID m_texture;
		unsigned int m_textureWidth;
		unsigned int m_textureHeight;

		// Atlas Layout
		bool hasLayout;
		unsigned int m_tileWidth;
		unsigned int m_tileHeight;
		unsigned int m_numRows;
		unsigned int m_numCols;
		unsigned int m_tile_gap;

		// Used before compilation for adding sprites
		struct profile_definition {
			int identifer;
			Vec2i origin;
			Vec2i size;
		};

		std::vector<profile_definition>* m_definitions;
		std::unordered_map<int, AtlasProfile> m_sprite_lookup;

	public:

		TextureAtlas(TextureID textureSource);
		~TextureAtlas();

		/*
			Defines the common layout of a texture image, that being where each sprite in the sprite sheet
			is equally distributed through the image. Special circumstances can be made later, but if a
			sprite sheet is formatted nicely, this allows for easy sprite lookups.

			tileWidth/tileHeight: The pixel dimensions of a single sprite
			numRows/numCols: How many sprite slots exist in the y(rows), and x(cols) directions.
			tileGap: If not 0, a tile gap is the number of pixels in between each sprite box.
		*/
		void AddAtlasLayout(unsigned int tileWidth, unsigned int tileHeight, unsigned int numRows, unsigned int numCols, unsigned int tileGap = 0);
	
		/*
			Defines a sprite in the texture atlas, from its pixel origin (top left corner) to
			its bottom right corner via its size.
		*/
		void AddSprite(int spriteIdentifier, Vec2i rectOrigin, Vec2i rectSize);
		
		/*
			Takes the atlas definitions, and compiles it, checking for errors,
			and generates profiles. Returns false if compile
			is unsuccessful.
		*/
		bool Compile();

		/*
			Returns a profile (the texture coordinates), for the provided
			atlas coordinates
		*/
		AtlasProfile GetProfile(unsigned int atlasX, unsigned int atlasY);

		/*
			Returns a profile (the texture coordinate), for the provided
			spriteIdentifier.
		*/
		AtlasProfile GetProfile(int spriteIdentifier);

		/*
			Gets the ID of the texture this atlas defines.
		*/
		TextureID GetTextureID();

	};
}