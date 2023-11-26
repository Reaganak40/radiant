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

		// OpenGL texture slot map
		TextureSlotMap m_texture_slots;
		TextureSlot m_next_slot;

		TextureID idCounter = 0;

		Impl()
		{
			m_texture_slots.fill(RDT_NULL_TEXTURE_SLOT);
			m_next_slot = 2;
			AddNoneTexture();
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

			m_textures.at(RDT_NULL_TEXTURE_ID).Bind(RDT_NONE_TEXTURE);
			m_texture_slots[RDT_NONE_TEXTURE] = RDT_NONE_TEXTURE;
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
			m_textures.at(nID).LoadTexturePNG(filepath);

			return nID;
		}

		TextureID GetTextureID(const std::string& name)
		{
			if (!TextureExists(name)) {
				return RDT_NULL_TEXTURE_ID;
			}

			return aliasToID.at(name);
		}

		const char* GetTextureAlias(TextureID tID)
		{
			for (auto& [name, id] : aliasToID) {
				if (id == tID) {
					return name.c_str();
				}
			}
			return "NoNameFound";
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

		std::shared_ptr<TextureAtlas> GetTextureAtlas(TextureID tID)
		{
			if (m_atlases.find(tID) == m_atlases.end()) {
				return nullptr;
			}
			return m_atlases.at(tID);
		}

		TextureSlot GetNextSlot() {

			unsigned int checkStart = m_next_slot;
			while (m_texture_slots[m_next_slot] != RDT_NULL_TEXTURE_SLOT) {
				m_next_slot++;

				if (m_next_slot >= MAX_TEXTURES) {
					m_next_slot = 2;
				}

				if (m_next_slot == checkStart) {
					return RDT_NULL_TEXTURE_SLOT;
				}
			}

			TextureSlot res = m_next_slot;
			m_next_slot++;
			if (m_next_slot >= MAX_TEXTURES) {
				m_next_slot = 2;
			}
			return res;
		}

		bool BindTexture(TextureID tID)
		{
			if (!TextureExists(tID)) {
				RDT_CORE_WARN("TextureManager - Tried to bind unregistered texture [id:{}]", tID);
				return false;
			}

			if (RDT_NULL_TEXTURE_SLOT == m_textures.at(tID).CurrentTextureSlot()) {
				TextureSlot nSlot = GetNextSlot();

				if (nSlot == RDT_NULL_TEXTURE_SLOT) {
					return false;
				}

				m_texture_slots[nSlot] = nSlot;
				m_textures.at(tID).Bind(nSlot);
				return true;
			}
			return false;
		}

		TextureSlot GetTextureSlot(TextureID tID)
		{
			if (!TextureExists(tID)) {
				return RDT_NULL_TEXTURE_SLOT;
			}

			return m_textures.at(tID).CurrentTextureSlot();
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

	const char* TextureManager::GetTextureAlias(TextureID tID)
	{
		return m_impl->GetTextureAlias(tID);
	}

	bool TextureManager::TextureExists(TextureID tID)
	{
		return m_impl->TextureExists(tID);
	}

	TextureAtlas& TextureManager::InitTextureAtlas(TextureID tID)
	{
		return m_impl->InitTextureAtlas(tID);
	}

	std::shared_ptr<TextureAtlas> TextureManager::GetTextureAtlas(TextureID tID)
	{
		return m_impl->GetTextureAtlas(tID);
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

	std::array<unsigned int, MAX_TEXTURES>& TextureManager::GetTextureSlotMap()
	{
		return m_impl->m_texture_slots;
	}

	bool TextureManager::BindTexture(TextureID tID)
	{
		return m_impl->BindTexture(tID);
	}

	TextureSlot TextureManager::GetTextureSlot(TextureID tID)
	{
		return m_impl->GetTextureSlot(tID);
	}

	void TextureManager::AddNoneTexture()
	{
		m_impl->AddNoneTexture();
	}
}
