#include "pch.h"
#include "GuiManager.h"
#include "Utils/Utils.h"

namespace rdt {
	
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
			return;
		}

		if (!Utils::PathExists(filepath) && magicWord != GUI_DEFAULT_FONT) {
			printf("Warning: [%s] does not exist.\n", filepath.c_str());
			return;
		}
		m_fonts[magicWord] = FontCache();
		m_fonts.at(magicWord).filepath = filepath;
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
			ImFont* font = io.Fonts->AddFontFromFileTTF(m_fonts.at(magicWord).filepath.c_str(), (float)fontSize);

			m_fonts.at(magicWord).font_ptrs[fontSize] = font;
		}

		return m_fonts.at(magicWord).font_ptrs.at(fontSize);
	}

}