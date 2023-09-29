#include "Title.h"

Title::Title()
	: m_gui_width(rdt::Renderer::GetWindowWidth() * 0.75f), m_gui_height(rdt::Renderer::GetWindowHeight() * 0.30f)
{
	m_window_width = rdt::Renderer::GetWindowWidth();
	m_window_height = rdt::Renderer::GetWindowHeight();
	m_window_config = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;

	rdt::GuiManager::LoadFont(PONG_FONT, "Source/UI/Fonts/ErosRegular-DOXn3.ttf");
	m_title_font = rdt::GuiManager::GetFont(PONG_FONT, 120);
}

Title::~Title()
{
}

void Title::OnUpdate(const float deltaTime)
{
	if (rdt::Input::CheckWindowResize()) {
		m_window_width = rdt::Renderer::GetWindowWidth();
		m_window_height = rdt::Renderer::GetWindowHeight();
	}
}

void Title::OnRender()
{
	// First render config
	ImGui::SetNextWindowSize(ImVec2(m_gui_width, m_gui_height), ImGuiCond_Appearing);
	ImGui::SetNextWindowPos(ImVec2(m_window_width / 2 - (m_gui_width / 2), 40));

	ImGui::PushFont(m_title_font);

	// Start drawing title
	ImGui::Begin("Title", NULL, m_window_config);

	auto textWidth = ImGui::CalcTextSize("Pong").x;
	auto windowWidth = ImGui::GetWindowSize().x;
	ImGui::SetCursorPosX((windowWidth / 2) - (textWidth / 2));
	ImGui::Text("Pong");
	ImGui::End();

	ImGui::PopFont();
}
