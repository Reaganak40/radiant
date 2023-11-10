/*******************************************************************
*	Module:  Graphics (core)
*	File:    Texture.h
*
*	Author: Reagan Kelley
*
*	This file contains the Texture class which holds image resource
	data, and openGL bindings.
*******************************************************************/
#pragma once

#define UNASSIGNED_TEXTURE 125
#define NONE_TEXTURE 1

namespace rdt {
	using TextureSlot = unsigned int;
	using glTextureID = unsigned int;

	class Texture {
	private:
		TextureSlot m_texture_slot;
		glTextureID m_ID;
		int m_image_width;
		int m_image_height;
		int m_bits_per_pixel;

	public:
		Texture();
		~Texture();

		/*
			Returns the OpenGL texture ID for this instance.
		*/
		const glTextureID GetID() { return m_ID; }

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
			Load texture from a PNG image
		*/
		void LoadTexturePNG(const std::string& textureFilePath);
		
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