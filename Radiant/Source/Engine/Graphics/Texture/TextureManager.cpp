#include "pch.h"
#include "TextureManager.h"

#include "Texture.h"
#include "TextureAtlas.h"
#include "Utils/Utils.h"
#include "Logging/Log.h"

namespace rdt {

	constexpr AtlasProfile ATLAS_PROFILE_ENTIRE_TEXTURE = { 0.0f, 0.0f, 1.0f, 1.0f };

	struct TextureManager::Impl {

		// Texture Storage and Lookup
		std::unordered_map<std::string, TextureID> aliasToID;
		std::unordered_map<TextureID, Texture> m_textures;

		// Texture Atlases
		std::unordered_map<TextureID, std::shared_ptr<TextureAtlas>> m_atlases;

		std::array<unsigned int, MAX_TEXTURES> m_texture_slots;
		TextureSlot m_next_slot;

		TextureID idCounter = 0;

		Impl()
		{
			AddNoneTexture();

			m_texture_slots.fill(UNASSIGNED_TEXTURE);
			m_next_slot = 2;
		}

		~Impl()
		{
		}

		bool TextureExists(const std::string& name) {
			return aliasToID.find(name) != aliasToID.end();
		}

		bool TextureExists(TextureID tID) {
			return m_textures.find(tID) != m_textures.end();
		}

		TextureID GenerateID() {
			return ++idCounter;
		}

		void AddNoneTexture()
		{
			aliasToID["None"] = RDT_NULL_TEXTURE_ID;
			m_textures[RDT_NULL_TEXTURE_ID];
			m_textures.at(RDT_NULL_TEXTURE_ID).SetToNone();

			m_textures.at("None").Bind(NONE_TEXTURE);
			m_texture_slots[NONE_TEXTURE] = NONE_TEXTURE;
		}

		TextureID LoadTextureFromPNG(const std::string& name, const std::string& filepath)
		{
			if (TextureExists(name)) {
				RDT_CORE_WARN("TextureManager - Tried to load duplicate texture {}", name);
				return RDT_NULL_TEXTURE_ID;
			}

			if (!Utils::PathExists(filepath)) {
				RDT_CORE_WARN("TextureManager - Texture file '{}' not found.", filepath);
				return RDT_NULL_TEXTURE_ID;
			}

			TextureID nID = GenerateID();
			aliasToID[name] = nID;
			
			m_textures[nID];
			m_textures.at(nID).LoadTexture(filepath);

			return nID;
		}

		TextureID GetTextureID(const std::string& name)
		{
			if (!TextureExists(name)) {
				return RDT_NULL_TEXTURE_ID;
			}

			return aliasToID.at(name);
		}

		Texture& GetTexure(TextureID tID)
		{
			return m_textures.at(tID);
		}

		const char* GetAlias(TextureID tID) {
			for (auto& [name, id] : aliasToID) {
				if (id == tID) {
					return name.c_str();
				}
			}

			return "TextureNotFound";
		}

		TextureAtlas& InitTextureAtlas(TextureID tID)
		{
			if (!TextureExists(tID)) {
				tID = RDT_NULL_TEXTURE_ID;
			}

			if (m_atlases.find(tID) == m_atlases.end()) {
				m_atlases[tID] = std::make_shared<TextureAtlas>(tID);
			}

			return *m_atlases.at(tID);
		}
	};

	// ======================================================================
	TextureManager::Impl* TextureManager::m_impl = nullptr;

	void TextureManager::Initialize()
	{
		Destroy();
		m_impl = new TextureManager::Impl;
	}

	void TextureManager::Destroy()
	{
		if (m_impl != nullptr) {
			delete m_impl;
			m_impl = nullptr;
		}
	}

	TextureID TextureManager::LoadTextureFromPNG(const std::string& name, const std::string& filepath)
	{
		return m_impl->LoadTextureFromPNG(name, filepath);
	}

	TextureID TextureManager::GetTextureID(const std::string& name)
	{
		return m_impl->GetTextureID(name);
	}

	bool TextureManager::TextureExists(TextureID tID)
	{
		return m_impl->TextureExists(tID);
	}

	TextureAtlas& TextureManager::InitTextureAtlas(TextureID tID)
	{
		return m_impl->InitTextureAtlas(tID);
	}

	AtlasProfile TextureManager::NOT_USING_ATLAS()
	{
		return ATLAS_PROFILE_ENTIRE_TEXTURE;
	}

	Texture& TextureManager::GetTexture(TextureID tID)
	{
		return m_impl->GetTexure(tID);
	}

	const char* TextureManager::GetAlias(TextureID tID)
	{
		return m_impl->GetAlias(tID);
	}

	bool TextureManager::ApplyTextureAtlas(Texture* texture, const Vec2i& atlasCoords, std::vector<Vertex>& rectVertices, bool flipHorizontal)
	{
		bool slots_changed = false;
		if (texture->CurrentTextureSlot() == UNASSIGNED_TEXTURE) {
			TextureSlot slot = m_instance->GetNextSlot();
			texture->Bind(slot);
			m_instance->m_impl->m_texture_slots[slot] = slot;
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

		if (flipHorizontal) {
			std::swap(rectVertices[0].texCoords.x, rectVertices[1].texCoords.x);
			std::swap(rectVertices[0].texCoords.y, rectVertices[1].texCoords.y);

			std::swap(rectVertices[2].texCoords.x, rectVertices[3].texCoords.x);
			std::swap(rectVertices[2].texCoords.y, rectVertices[3].texCoords.y);
		}

		return slots_changed;
	}
	
	std::array<unsigned int, MAX_TEXTURES>& TextureManager::GetTextureSlots()
	{
		return m_instance->m_impl->m_texture_slots;
	}

	void TextureManager::AddNoneTexture()
	{
		m_impl->AddNoneTexture();
	}

	TextureSlot TextureManager::GetNextSlot()
	{
		return m_impl->m_next_slot++;
	}

	void TextureManager::NOT_USING_ATLAS(AtlasProfile& profile)
	{
		profile[0] = 0.0f;
		profile[1] = 0.0f;
		profile[2] = 1.0f;
		profile[3] = 1.0f;
	}

	Texture& TextureManager::LoadTextureFromPNGImpl(const std::string& name, const std::string& filepath)
	{
		return m_impl->LoadTextureFromPNG(name, filepath);
	}

	Texture* TextureManager::GetTextureImpl(const std::string& name)
	{
		return m_impl->GetTexture(name);
	}
}
