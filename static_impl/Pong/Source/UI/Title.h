#pragma once
#include "Pong.h"

class Title : public rdt::GuiTemplate {
private:
	const float m_gui_width;
	const float m_gui_height;

	int m_window_width;
	int m_window_height;

	ImGuiWindowFlags m_window_config;

	ImFont* m_title_font;

public:
	Title();
	~Title();

	void OnUpdate(const float deltaTime) override;
	void OnRender() override;
};