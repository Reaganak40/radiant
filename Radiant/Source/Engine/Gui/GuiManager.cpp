#include "pch.h"
#include "GuiManager.h"
#include "Utils/Utils.h"
#include "Logging/Log.h"

namespace rdt::core {
	
	GuiManager* GuiManager::m_instance = nullptr;

	GuiManager::GuiManager()
	{
		LoadFontImpl(GUI_DEFAULT_FONT, "");
		ImGuiIO& io = ImGui::GetIO();
		m_fonts.at(GUI_DEFAULT_FONT).font_ptrs[GUI_DEFAULT_FONT_SIZE] = io.Fonts->AddFontDefault();
	}

	GuiManager::~GuiManager()
	{
	}

	GuiManager* GuiManager::GetInstance()
	{
		if (m_instance == nullptr) {
			m_instance = new GuiManager;
		}

		return m_instance;
	}

	void GuiManager::Initialize()
	{
		Destroy();
		GetInstance();
	}

	void GuiManager::Destroy()
	{
		if (m_instance != nullptr) {
			delete m_instance;
			m_instance = nullptr;
		}
	}

	void GuiManager::LoadFontImpl(int magicWord, const std::string& filepath)
	{
		if (m_fonts.find(magicWord) != m_fonts.end()) {
			RDT_CORE_WARN("GuiManager - MagicWord {} already in-use. No font loaded.", magicWord);
			return;
		}

		if (!Utils::PathExists(filepath) && magicWord != GUI_DEFAULT_FONT) {
			RDT_CORE_WARN("GuiManager - ttf file '{}' does not exist. No font loaded.", filepath);
			return;
		}

		m_fonts[magicWord];
		m_fonts.at(magicWord).filepath = filepath;
	}

	void GuiManager::LoadIconsImpl(int magicWord, const std::string& filepath, const ImWchar* iconRanges, float alignSizeOffset)
	{
		if (m_fonts.find(magicWord) != m_fonts.end()) {
			RDT_CORE_WARN("GuiManager - MagicWord {} already in-use. No font loaded.", magicWord);
			return;
		}

		if (!Utils::PathExists(filepath) && magicWord != GUI_DEFAULT_FONT) {
			RDT_CORE_WARN("GuiManager - ttf file '{}' does not exist. No font loaded.", filepath);
			return;
		}

		m_fonts[magicWord];
		m_fonts.at(magicWord).filepath = filepath;

		m_fonts.at(magicWord).IsIconFont = true;
		memcpy_s(m_fonts.at(magicWord).icons_ranges, sizeof(ImWchar) * 3, iconRanges, sizeof(ImWchar) * 3);
		m_fonts.at(magicWord).size_align_offset = alignSizeOffset;
	}

	ImFont* GuiManager::GetFontImpl(const int magicWord, unsigned int fontSize)
	{
		if (m_fonts.find(magicWord) == m_fonts.end()) {
			return nullptr;
		}

		if (magicWord == GUI_DEFAULT_FONT) {
			fontSize = GUI_DEFAULT_FONT_SIZE;
		}

		if (m_fonts.at(magicWord).font_ptrs.find(fontSize) == m_fonts.at(magicWord).font_ptrs.end()) {
			ImGuiIO& io = ImGui::GetIO();
			ImFont* font = nullptr;
			
			auto& cache = m_fonts.at(magicWord);
			if (cache.IsIconFont) {
				float nFontSize = (float)fontSize * cache.size_align_offset;

				ImFontConfig icons_config;
				icons_config.MergeMode = false;
				icons_config.PixelSnapH = true;
				icons_config.GlyphMinAdvanceX = nFontSize;

				font = io.Fonts->AddFontFromFileTTF(cache.filepath.c_str(), nFontSize, &icons_config, cache.icons_ranges);
			}
			else {
				font = io.Fonts->AddFontFromFileTTF(cache.filepath.c_str(), (float)fontSize);
			}

			m_fonts.at(magicWord).font_ptrs[fontSize] = font;
		}

		return m_fonts.at(magicWord).font_ptrs.at(fontSize);
	}

}