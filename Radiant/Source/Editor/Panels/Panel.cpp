#include "pch.h"
#include "Panel.h"

namespace rdt::core {
	
	ThemeData Panel::theme_data;

	Panel::Panel()
	{
	}
	Panel::~Panel()
	{
	}
	void Panel::SetTheme(EditorTheme nTheme)
	{
		switch (nTheme) {
		case Theme_Gray:
			theme_data.Titlebackground = { 0.75f, 0.75f, 0.75f, 1.0f };
			theme_data.TitlebackgroundActive = { 0.75f, 0.75f, 0.75f, 1.0f };
			theme_data.WindowBackground = { 0.96f, 0.96f,0.96f, 0.95f };
			theme_data.TextColor = { 0.0f, 0.0f, 0.0f, 1.0f };
			break;
		}
	}
	void Panel::ThemeBegin()
	{
		ImGui::PushStyleColor(ImGuiCol_TitleBg, theme_data.Titlebackground);
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, theme_data.TitlebackgroundActive);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, theme_data.WindowBackground);
		ImGui::PushStyleColor(ImGuiCol_Text, theme_data.TextColor);
	}
	void Panel::ThemeEnd()
	{
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
	}
}
