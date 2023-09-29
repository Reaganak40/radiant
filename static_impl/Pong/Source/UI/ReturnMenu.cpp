#include "ReturnMenu.h"

ReturnMenu::ReturnMenu()
	: m_gui_width(130), m_gui_height(25), m_return_requested(false)
{
	m_window_width = rdt::Renderer::GetWindowWidth();
	m_window_height = rdt::Renderer::GetWindowHeight();
	m_window_config = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

	m_return_font = rdt::GuiManager::GetFont(GUI_DEFAULT_FONT, GUI_DEFAULT_FONT_SIZE);
}

ReturnMenu::~ReturnMenu()
{
}

void ReturnMenu::OnUpdate(const float deltaTime)
{
}

void ReturnMenu::OnRender()
{
	// First render config
	ImGui::SetNextWindowSize(ImVec2(m_gui_width, m_gui_height), ImGuiCond_Appearing);
	ImGui::SetNextWindowPos(ImVec2(m_window_width - m_gui_width - 3, m_window_height - m_gui_height - 10));

	ImGui::PushFont(m_return_font);

	// Start drawing title
	ImGui::Begin("Return", NULL, m_window_config);

	auto textWidth = ImGui::CalcTextSize("Return to Menu").x;
	auto windowWidth = ImGui::GetWindowSize().x;
	ImGui::SetCursorPosX((windowWidth / 2) - (textWidth / 2));


	if (ImGui::IsWindowHovered()) {
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(255, 0, 0, 255));
		ImGui::Text("Return to Menu");
		ImGui::PopStyleColor();

		const ImGuiIO& io = ImGui::GetIO();
		if (io.MouseReleased[0]) {
			m_return_requested = true;
		}
	}
	else {
		ImGui::Text("Return to Menu");
	}

	ImGui::End();


	ImGui::PopFont();
}
