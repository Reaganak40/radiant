#include "pch.h"
#include "Texture.h"
#include <stb/stb_image.h>

#include "Utils/Utils.h"

namespace rdt {
	
	Texture::Texture()
		: m_textureID(0), m_imageBuffer(nullptr), m_image_height(0), m_image_width(0), m_bits_per_pixel(0)
	{
		glGenTextures(1, &m_textureID);
		m_texture_slot = NO_TEXTURE;

		has_texture_atlas = false;
		m_tileWidth = 0.0f;
		m_tileHeight = 0.0f;
		m_numRows = 0;
		m_numCols = 0;

	}

	Texture::~Texture()
	{
		if (m_imageBuffer != nullptr) {
			DeleteTexture();
		}

		glDeleteTextures(1, &m_textureID);
	}

	void Texture::DefineTextureAtlas(unsigned int tileWidth, unsigned int tileHeight, unsigned int numRows, unsigned int numCols)
	{
		has_texture_atlas = true;
		m_tileWidth = (float)tileWidth;
		m_tileHeight = (float)tileHeight;
		m_numRows = numRows;
		m_numCols = numCols;
	}

	void Texture::LoadTexture(const std::string& textureFilePath)
	{
		if (m_imageBuffer != nullptr) {
			DeleteTexture();
		}

		if (!Utils::PathExists(textureFilePath)) {
			printf("Warning: Could not load texture. File not found!\n");
			return;
		}

		//stbi_set_flip_vertically_on_load(1);

		m_imageBuffer = stbi_load(textureFilePath.c_str(),
			&m_image_width,
			&m_image_height,
			&m_bits_per_pixel, 4);

		glBindTexture(GL_TEXTURE_2D, m_textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
			m_image_width, m_image_height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, m_imageBuffer);

	}

	void Texture::Bind(TextureSlot textureSlot)
	{
		glBindTextureUnit(textureSlot, m_textureID);
		m_texture_slot = textureSlot;
	}

	void Texture::DeleteTexture()
	{
		delete[] m_imageBuffer;
		m_imageBuffer = nullptr;
	}
}