#include "pch.h"
#include "TextureManager.h"

namespace rdt {

	TextureManager* TextureManager::m_instance = nullptr;

	TextureManager::TextureManager()
	{
		m_texture_slots.fill(NO_TEXTURE);
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
		if (texture->CurrentTextureSlot() == NO_TEXTURE) {
			for (TextureSlot i = 1; i < MAX_TEXTURES; i++) {
				if (m_instance->m_texture_slots[i] == NO_TEXTURE) {
					texture->Bind(i);
					m_instance->m_texture_slots[i] = i;
					slots_changed = true;
					break;
				}
			}
		}
		unsigned int texIndex = texture->CurrentTextureSlot();

		if (texture->has_texture_atlas) {

			rectVertices[0].texCoords.x = (atlasCoords.x * texture->m_tileWidth) / texture->m_image_width;
			rectVertices[0].texCoords.y = ((atlasCoords.y + 1) * texture->m_tileHeight) / texture->m_image_height;

			rectVertices[1].texCoords.x = ((atlasCoords.x + 1) * texture->m_tileWidth) / texture->m_image_width;
			rectVertices[1].texCoords.y = ((atlasCoords.y + 1) * texture->m_tileHeight) / texture->m_image_height;


			rectVertices[2].texCoords.x = ((atlasCoords.x + 1) * texture->m_tileWidth) / texture->m_image_width;
			rectVertices[2].texCoords.y = (atlasCoords.y * texture->m_tileHeight) / texture->m_image_height;


			rectVertices[3].texCoords.x = (atlasCoords.x * texture->m_tileWidth) / texture->m_image_width;
			rectVertices[3].texCoords.y = (atlasCoords.y * texture->m_tileHeight) / texture->m_image_height;

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
