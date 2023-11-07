#include "pch.h"
#include "Texture.h"
#include <stb_image.h>

#include "Utils/Utils.h"

namespace rdt {
	
	Texture::Texture()
		: m_textureID(0), m_image_height(0), m_image_width(0), m_bits_per_pixel(0)
	{
		glGenTextures(1, &m_textureID);
		m_texture_slot = UNASSIGNED_TEXTURE;
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_textureID);
	}

	int Texture::GetImageWidth()
	{
		return m_image_width;
	}

	int Texture::GetImageHeight()
	{
		return m_image_height;
	}


	void Texture::LoadTexture(const std::string& textureFilePath)
	{
		unsigned char* imageBuffer;

		// Precondition: filepath already been verified

		//stbi_set_flip_vertically_on_load(1);

		imageBuffer = stbi_load(textureFilePath.c_str(),
			&m_image_width,
			&m_image_height,
			&m_bits_per_pixel, 4);

		glBindTexture(GL_TEXTURE_2D, m_textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
			m_image_width, m_image_height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, imageBuffer);

		stbi_image_free(imageBuffer);
	}

	void Texture::SetToNone()
	{
		m_image_width = 1;
		m_image_height = 1;

		glBindTexture(GL_TEXTURE_2D, m_textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		unsigned char* data = new unsigned char[m_image_width * m_image_height * 4];

		for (int i = 0; i < m_image_width * m_image_height * 4; i++) {
			data[i] = 255;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
			m_image_width, m_image_height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, data);

		delete[] data;
	}

	void Texture::Bind(TextureSlot textureSlot)
	{
		glBindTextureUnit(textureSlot, m_textureID);
		m_texture_slot = textureSlot;
	}
}