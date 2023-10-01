#include "pch.h"
#include "TextureManager.h"

namespace rdt {
	using namespace core;

	TextureManager* TextureManager::m_instance = nullptr;

	TextureManager::TextureManager()
	{
		m_texture_slots.fill(UNASSIGNED_TEXTURE);
		m_next_slot = 2;

		AddNoneTexture();
	}

	TextureManager::~TextureManager()
	{
	}

	void TextureManager::Initialize()
	{
		Destroy();
		m_instance = new TextureManager;
	}

	void TextureManager::Destroy()
	{
		if (m_instance != nullptr) {
			delete m_instance;
			m_instance = nullptr;
		}
	}

	bool TextureManager::ApplyTextureAtlas(Texture* texture, const Vec2i& atlasCoords, std::vector<Vertex>& rectVertices)
	{
		bool slots_changed = false;
		if (texture->CurrentTextureSlot() == UNASSIGNED_TEXTURE) {
			TextureSlot slot = m_instance->GetNextSlot();
			texture->Bind(slot);
			m_instance->m_texture_slots[slot] = slot;
			slots_changed = true;
		}
		float texIndex = (float)texture->CurrentTextureSlot();

		if (texture->has_texture_atlas) {

			float leftX  = ((atlasCoords.x * (texture->m_tileWidth + texture->m_tile_gap)) + (texture->m_tile_gap / 2)) / texture->m_image_width;
			float rightX = (((atlasCoords.x + 1) * (texture->m_tileWidth + texture->m_tile_gap)) + (texture->m_tile_gap / 2)) / texture->m_image_width;

			float topY    = ((atlasCoords.y * (texture->m_tileHeight + texture->m_tile_gap)) + (texture->m_tile_gap / 2)) / texture->m_image_height;
			float bottomY = (((atlasCoords.y + 1) * (texture->m_tileHeight + texture->m_tile_gap)) + (texture->m_tile_gap / 2)) / texture->m_image_height;

			rectVertices[0].texCoords.x = leftX;
			rectVertices[0].texCoords.y = bottomY;

			rectVertices[1].texCoords.x = rightX;
			rectVertices[1].texCoords.y = bottomY;


			rectVertices[2].texCoords.x = rightX;
			rectVertices[2].texCoords.y = topY;


			rectVertices[3].texCoords.x = leftX;
			rectVertices[3].texCoords.y = topY;

		}
		else {
			rectVertices[0].texCoords = { 0.0f, 1.0f };
			rectVertices[1].texCoords = { 1.0f, 1.0f };
			rectVertices[2].texCoords = { 1.0f, 0.0f };
			rectVertices[3].texCoords = { 0.0f, 0.0f };
		}

		for (int i = 0; i < 4; i++) {
			rectVertices[i].texIndex = texIndex;
		}

		return slots_changed;
	}
	
	std::array<unsigned int, MAX_TEXTURES>& TextureManager::GetTextureSlots()
	{
		return m_instance->m_texture_slots;
	}

	void TextureManager::AddNoneTexture()
	{
		m_textures["None"] = Texture();
		m_textures.at("None").SetToNone();
		m_textures.at("None").Bind(NONE_TEXTURE);
		m_texture_slots[NONE_TEXTURE] = NONE_TEXTURE;

	}

	TextureSlot TextureManager::GetNextSlot()
	{
		return m_next_slot++;
	}

	Texture& TextureManager::LoadTextureFromPNGImpl(const std::string& name, const std::string& filepath)
	{
		if (m_textures.find(name) == m_textures.end()) {
			m_textures[name] = Texture();
			m_textures.at(name).LoadTexture(filepath);
		}

		return m_textures.at(name);
	}

	Texture* TextureManager::GetTextureImpl(const std::string& name)
	{
		if (m_textures.find(name) == m_textures.end()) {
			printf("Warning: Could not find texture [%s]\n", name.c_str());
			return nullptr;
		}

		return &m_textures.at(name);
	}
}
