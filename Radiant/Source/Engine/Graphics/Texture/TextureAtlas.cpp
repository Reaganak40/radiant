#include "pch.h"
#include "TextureAtlas.h"

#include "Texture.h"
#include "TextureManager.h"
#include "Utils/MathTypes.h"
#include "Logging/Log.h"

namespace rdt {

	TextureAtlas::TextureAtlas(TextureID textureSource)
		: m_definitions(nullptr), m_texture(RDT_NULL_TEXTURE_ID), m_textureWidth(0), m_textureHeight(0)
	{
		hasLayout = false;
		m_tileWidth = 0.0f;
		m_tileHeight = 0.0f;
		m_numRows = 0;
		m_numCols = 0;
		m_tile_gap = 0;

		if (TextureManager::TextureExists(textureSource)) {
			RDT_ERROR("TextureAtlas - Texture Source [ID:{}] not found", textureSource);
			return;
		}

		m_texture = textureSource;
		m_textureWidth = TextureManager::GetTexture(textureSource).GetImageWidth();
		m_textureHeight = TextureManager::GetTexture(textureSource).GetImageHeight();
	}

	TextureAtlas::~TextureAtlas()
	{
		if (m_definitions != nullptr) {
			delete m_definitions;
		}
	}

	void TextureAtlas::AddAtlasLayout(unsigned int tileWidth, unsigned int tileHeight, unsigned int numRows, unsigned int numCols, unsigned int tileGap)
	{
		hasLayout = true;
		m_tileWidth = tileWidth;
		m_tileHeight = tileHeight;
		m_numRows = numRows;
		m_numCols = numCols;
		m_tile_gap = tileGap;
	}

	void TextureAtlas::AddSprite(int spriteIdentifier, Vec2i rectOrigin, Vec2i rectSize)
	{
		if (m_definitions == nullptr) {
			m_definitions = new std::vector<profile_definition>();
		}

		m_definitions->push_back({ spriteIdentifier, rectOrigin, rectSize });
	}

	bool TextureAtlas::Compile()
	{
		m_sprite_lookup.clear();
		
		if (m_texture == RDT_NULL_TEXTURE_ID) {
			RDT_CORE_ERROR("TextureAtlas - Compilation failed. Tried to Use NoneTexture.");
			return false;
		}

		if (hasLayout) {
			if ((((m_tileWidth + m_tile_gap + m_tile_gap) * m_numCols)) != m_textureWidth) {
				RDT_CORE_ERROR("TextureAtlas - Compilation failed for '{}', AtlasLayout is not pixel perfect with texture width.", TextureManager::GetAlias(m_texture));
				return false;
			}

			if ((((m_tileHeight + m_tile_gap + m_tile_gap) * m_numRows)) != m_textureHeight) {
				RDT_CORE_ERROR("TextureAtlas - Compilation failed for '{}', AtlasLayout is not pixel perfect with texture height.", TextureManager::GetAlias(m_texture));
				return false;
			}
		}
		
		if (m_definitions != nullptr) {
			for (auto& definition : *m_definitions) {
				if (m_sprite_lookup.find(definition.identifer) != m_sprite_lookup.end()) {
					RDT_CORE_ERROR("TextureAtlas -  Compilation failed for '{}', Duplicate sprite identifer [{}]", TextureManager::GetAlias(m_texture), definition.identifer);
					return false;
				}

				m_sprite_lookup[definition.identifer];
				auto& profile = m_sprite_lookup.at(definition.identifer);
				profile.normalizedStartX = definition.origin.x / (float)m_textureWidth;
				profile.normalizedStartY = definition.origin.y / (float)m_textureHeight;
				profile.normalizedWidth = definition.size.x / (float)m_textureHeight;
				profile.normalizedHeight = definition.size.y / (float)m_textureHeight;
			}

			delete m_definitions;
			m_definitions = nullptr;
		}

		return true;
	}

	AtlasProfile TextureAtlas::GetProfile(unsigned int atlasX, unsigned int atlasY)
	{

		if (atlasX >= m_numCols || atlasY >= m_numRows) {
			RDT_WARN("TextureAtlas - AtlasCoords ({}, {}), is out of bounds for texture ['{}']", atlasX, atlasY, TextureManager::GetAlias(m_texture));
			return TextureManager::NOT_USING_ATLAS();
		}
		if (!hasLayout) {
			return TextureManager::NOT_USING_ATLAS();
		}

		AtlasProfile res;
		res.normalizedStartX = (m_tileWidth + m_tile_gap + m_tile_gap) * atlasX / m_textureWidth;
		res.normalizedStartY = (m_tileHeight + m_tile_gap + m_tile_gap) * atlasY / m_textureHeight;
		res.normalizedWidth = (m_tileWidth + m_tile_gap + m_tile_gap) / m_textureWidth;
		res.normalizedHeight = (m_tileHeight + m_tile_gap + m_tile_gap) / m_textureHeight;

		return res;
	}
	AtlasProfile TextureAtlas::GetProfile(int spriteIdentifier)
	{
		if (m_sprite_lookup.find(spriteIdentifier) == m_sprite_lookup.end()) {
			RDT_WARN("TextureAtlas - Identifier [{}] not found for texutre ['{}'], using entire texture.", spriteIdentifier, TextureManager::GetAlias(m_texture));
			return TextureManager::NOT_USING_ATLAS();
		}

		return m_sprite_lookup.at(spriteIdentifier);
	}
}
