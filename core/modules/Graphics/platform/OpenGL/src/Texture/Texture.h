/*******************************************************************
*	Module: Texture
*
*	This section contains the Texture class which holds image
*	resource data, and openGL bindings.
*******************************************************************/
#pragma once
#include "gl_core_api.h"

#define GL_CORE_NULL_TEXTURE_SLOT 0
#define GL_CORE_NONE_TEXTURE 1

namespace rdt::glCore {
	using TextureSlot = unsigned int;
	using TextureID = unsigned int;

	class GL_CORE_API Texture {
	private:
		TextureSlot m_texture_slot;
		TextureID m_ID;
		int m_image_width;
		int m_image_height;
		int m_bits_per_pixel;
		int* refCount;
	public:
		Texture();
		Texture(const Texture& other);
		~Texture();

		/*
			Returns the OpenGL texture ID for this instance.
		*/
		const TextureID GetID() { return m_ID; }

		/*
			Returns the assigned texture slot for this texture.
		*/
		const TextureSlot CurrentTextureSlot() { return m_texture_slot; }

		/*
			Returns the image texture width (in pixels)
		*/
		int GetImageWidth();

		/*
			Returns the image texture height (in pixels)
		*/
		int GetImageHeight();

		friend class TextureManager;
	private:
		/*
			Load texture from a PNG image. Returns true if successful.
		*/
		bool LoadTexturePNG(const char* textureFilePath);

		/*
			Sets this texture to a white square (effectively a blank texture)
		*/
		void SetToNone();

		/*
			Binds this texture to this provided texture slot
		*/
		void Bind(TextureSlot slot);
	};
}