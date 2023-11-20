#include "pch.h"
#include "TextureManager.h"
#include "../include/glCore.h"
#include "Logging/Log.h"

namespace glCore {

	TextureManager::TextureManager()
	{
		m_texture_slots.fill(GL_CORE_NULL_TEXTURE_SLOT);
		m_next_slot = 2;
		AddNoneTexture();

		m_texture_slots_changed = false;
	}

	TextureManager::~TextureManager()
	{
	}

	TextureID TextureManager::LoadTextureFromPNG(const std::string& filepath)
	{
		Texture nTexture;
		if (!nTexture.LoadTexturePNG(filepath.c_str())) {
			return GL_CORE_NULL_TEXTURE_ID;
		}
		m_textures[nTexture.GetID()] = nTexture;
		return nTexture.GetID();
	}

	bool TextureManager::TextureExists(TextureID tID)
	{
		return m_textures.find(tID) != m_textures.end();
	}

	Texture& TextureManager::GetTexture(TextureID tID)
	{
		if (!TextureExists(tID)) {
			return m_textures.at(GL_CORE_NULL_TEXTURE_ID);
		}

		return m_textures.at(tID);
	}

	TextureSlotMap& TextureManager::GetTextureSlotMap()
	{
		return m_texture_slots;
	}

	bool TextureManager::BindTexture(TextureID tID)
	{
		if (!TextureExists(tID)) {
			GL_CORE_WARN("TextureManager - Tried to bind unregistered texture [id:{}]", tID);
			return false;
		}

		if (GL_CORE_NULL_TEXTURE_SLOT == m_textures.at(tID).CurrentTextureSlot()) {
			TextureSlot nSlot = NextTextureSlot();

			if (nSlot == GL_CORE_NULL_TEXTURE_SLOT) {
				return false;
			}

			m_texture_slots[nSlot] = nSlot;
			m_textures.at(tID).Bind(nSlot);
			return true;
		}
		return false;
	}

	void TextureManager::BindTextures(const std::unordered_set<TextureID>& textures)
	{
		if (textures.size() > MAX_TEXTURES) {
			return;
		}
		
		std::bitset<MAX_TEXTURES + 2> taken_slots;
		std::vector<TextureID> toAdd;
		toAdd.reserve(textures.size());

		for (auto tID : textures) {
			TextureSlot slot;
			if ((slot = GetTextureSlot(tID)) != GL_CORE_NULL_TEXTURE_SLOT) {
				taken_slots.set(slot, true);
			}
			else {
				toAdd.push_back(tID);
			}
		}

		if (toAdd.size() == 0) {
			return;
		}

		int currentSlot = 2;
		for (auto tID : toAdd) {

			while (taken_slots[currentSlot]) {
				currentSlot++;
			}

			m_texture_slots[currentSlot] = currentSlot;
			m_textures.at(tID).Bind(currentSlot);
		}

		m_texture_slots_changed = true;
	}

	TextureSlot TextureManager::GetTextureSlot(TextureID tID)
	{
		if (!TextureExists(tID)) {
			return GL_CORE_NULL_TEXTURE_SLOT;
		}

		return m_textures.at(tID).CurrentTextureSlot();
	}

	bool TextureManager::TextureSlotsChanged()
	{
		return m_texture_slots_changed;
	}

	void TextureManager::Reset()
	{
		m_texture_slots_changed = false;
	}

	TextureID TextureManager::NextTextureID()
	{
		return ++idCounter;
	}

	TextureSlot TextureManager::NextTextureSlot()
	{
		unsigned int checkStart = m_next_slot;
		while (m_texture_slots[m_next_slot] != GL_CORE_NULL_TEXTURE_SLOT) {
			m_next_slot++;

			if (m_next_slot >= MAX_TEXTURES) {
				m_next_slot = 2;
			}

			if (m_next_slot == checkStart) {
				return GL_CORE_NULL_TEXTURE_SLOT;
			}
		}

		TextureSlot res = m_next_slot;
		m_next_slot++;
		if (m_next_slot >= MAX_TEXTURES) {
			m_next_slot = 2;
		}
		return res;
	}

	void TextureManager::AddNoneTexture()
	{
		m_textures[GL_CORE_NULL_TEXTURE_ID].SetToNone();
		m_textures.at(GL_CORE_NULL_TEXTURE_ID).Bind(GL_CORE_NONE_TEXTURE);
		m_texture_slots[GL_CORE_NONE_TEXTURE] = GL_CORE_NONE_TEXTURE;
	}
}
