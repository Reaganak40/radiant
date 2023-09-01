#pragma once
#include "pch.h"

namespace Radiant {

#define MAX_TEXTURES 30
	typedef unsigned int TextureID;
	typedef unsigned int TextureSlot;

	class Texture {
	private:
		TextureID m_ID;

		unsigned char* m_imageBuffer;
		int m_image_width;
		int m_image_height;
		int m_bits_per_pixel;

	public:
		Texture();
		~Texture();

		TextureID LoadTexture(const std::string& textureFilePath);
		void Bind(unsigned int textureSlot);
		inline TextureID GetTextureID() { return m_ID; }

	private:
		void DeleteTexture();

	};

}