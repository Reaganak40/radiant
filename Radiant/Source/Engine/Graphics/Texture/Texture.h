#pragma once

#include "Core.h"
#include "Utils/UniqueID.h"
#include "Utils/MathTypes.h"

#define MAX_TEXTURES 30
#define UNASSIGNED_TEXTURE 125
#define NONE_TEXTURE 1

namespace rdt {
	typedef unsigned int TextureSlot;
	using TextureID = unsigned int;   // Unique Identifier for a Texture

	class Texture {
	private:
		TextureID m_textureID;
		TextureSlot m_texture_slot;

		int m_image_width;
		int m_image_height;
		int m_bits_per_pixel;

	public:
		Texture();
		~Texture();

		inline TextureID GetID() { return m_textureID; }

		int GetImageWidth();

		int GetImageHeight();

		friend class TextureManager;

	private:
		void LoadTexture(const std::string& textureFilePath);
		void SetToNone();

		void Bind(TextureSlot slot);
		const TextureSlot CurrentTextureSlot() { return m_texture_slot; }
	};
}