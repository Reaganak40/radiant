#include "pch.h"
#include "Texture.h"

#include <Radiant/Logger.h>

namespace rdt::glCore {
	
	Texture::Texture()
		: m_ID(0), m_image_height(0), m_image_width(0), m_bits_per_pixel(0)
	{
		glGenTextures(1, &m_ID);
		m_texture_slot = GL_CORE_NULL_TEXTURE_SLOT;
		refCount = new int(1);
	}

	Texture::Texture(const Texture& other)
	{
		m_ID = other.m_ID;
		m_image_height = other.m_image_height;
		m_image_width = other.m_image_width;
		m_bits_per_pixel = other.m_bits_per_pixel;
		m_texture_slot = other.m_texture_slot;

		refCount = other.refCount;
		(*refCount)++;
	}

	Texture::~Texture()
	{
		if (--(*refCount) == 0) {
			glDeleteTextures(1, &m_ID);
			delete refCount;
		}
	}

	int Texture::GetImageWidth()
	{
		return m_image_width;
	}

	int Texture::GetImageHeight()
	{
		return m_image_height;
	}


	bool Texture::LoadTexturePNG(const char* textureFilePath)
	{
		unsigned char* imageBuffer;

		// Precondition: filepath already been verified
		if (!std::filesystem::exists(textureFilePath)) {
			RDT_CORE_WARN("Texture - Failed to load from PNG. File not found!");
			return false;
		}
		//stbi_set_flip_vertically_on_load(1);

		imageBuffer = stbi_load(textureFilePath,
			&m_image_width,
			&m_image_height,
			&m_bits_per_pixel, 4);

		glBindTexture(GL_TEXTURE_2D, m_ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
			m_image_width, m_image_height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, imageBuffer);

		stbi_image_free(imageBuffer);

		return true;
	}

	void Texture::SetToNone()
	{
		m_image_width = 1;
		m_image_height = 1;

		glBindTexture(GL_TEXTURE_2D, m_ID);

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
		glBindTextureUnit(textureSlot, m_ID);
		m_texture_slot = textureSlot;
	}
}