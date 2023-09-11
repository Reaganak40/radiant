#pragma once
#include "Pong.h"

enum MenuOption {
	NoOptionSelected = 0,
	PlayAgainstHuman,
	PlayAgianstComputer,
	ExitGame
};
class MenuSelection : public rdt::GuiTemplate {
private:
	const float m_gui_width;
	const float m_gui_height;

	int m_window_width;
	int m_window_height;

	ImGuiWindowFlags m_window_config;

	ImFont* m_menu_font;

	MenuOption m_choice;

public:
	MenuSelection();
	~MenuSelection();

	void OnUpdate(const float deltaTime) override;
	void OnRender() override;

	MenuOption GetSelection() { return m_choice; }
	void ResetSelection() { m_choice = NoOptionSelected; }
private:
	bool AddSelection(const rdt::Vec2d& mouseCoords, const std::string& text);
};