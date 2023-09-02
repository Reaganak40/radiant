#include "pch.h"
#include "Texture.h"
#include <stb/stb_image.h>

namespace Radiant {

	Texture::Texture()
		: m_ID(0), m_imageBuffer(nullptr), m_image_height(0), m_image_width(0), m_bits_per_pixel(0)
	{
		glGenTextures(1, &m_ID);
	}

	Texture::~Texture()
	{
		if (m_imageBuffer != nullptr) {
			DeleteTexture();
		}

		glDeleteTextures(1, &m_ID);
	}

	TextureID Texture::LoadTexture(const std::string& textureFilePath)
	{
		if (m_imageBuffer != nullptr) {
			DeleteTexture();
		}

		//stbi_set_flip_vertically_on_load(1);

		m_imageBuffer = stbi_load(textureFilePath.c_str(),
			&m_image_width,
			&m_image_height,
			&m_bits_per_pixel, 4);

		glBindTexture(GL_TEXTURE_2D, m_ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
			m_image_width, m_image_height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, m_imageBuffer);

		return m_ID;
	}

	void Texture::Bind(unsigned int textureSlot)
	{
		glBindTextureUnit(textureSlot, m_ID);
	}

	void Texture::DeleteTexture()
	{
		delete[] m_imageBuffer;
		m_imageBuffer = nullptr;
	}

}