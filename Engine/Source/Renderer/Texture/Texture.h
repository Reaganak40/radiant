#pragma once

#include "Utils/UniqueID.h"
#include "Utils/MathTypes.h"

typedef unsigned int TextureSlot;
typedef unsigned int TextureID;

#define NO_TEXTURE 0
#define MAX_TEXTURES 30

namespace rdt {
	class Texture {
	private:
		TextureID m_textureID;
		TextureSlot m_texture_slot;

		unsigned char* m_imageBuffer;
		int m_image_width;
		int m_image_height;
		int m_bits_per_pixel;

		bool has_texture_atlas;
		float m_tileWidth;
		float m_tileHeight;
		unsigned int m_numRows;
		unsigned int m_numCols;
		unsigned int m_tile_gap;


	public:
		Texture();
		~Texture();

		inline TextureID GetID() { return m_textureID; }

		/*
			Adds a texture atlas to the texture, which can be used for reference tile maps.
		*/
		void DefineTextureAtlas(unsigned int tileWidth, unsigned int tileHeight, unsigned int numRows, unsigned int numCols, unsigned int tileGap);

		friend class TextureManager;

	private:
		void LoadTexture(const std::string& textureFilePath);
		void Bind(TextureSlot slot);
		const TextureSlot CurrentTextureSlot() { return m_texture_slot; }
		void DeleteTexture();

	};
}