#include "Scoreboard.h"

Scoreboard::Scoreboard(Ball& ballReference)
	: m_gui_width(250.0f), m_gui_height(90.0f), m_ball(ballReference)
{
	m_window_width = Radiant::Renderer::GetWindowWidth();
	m_window_height = Radiant::Renderer::GetWindowHeight();

	m_window_config = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;

	Radiant::GuiManager::LoadFont(PONG_FONT, "Source/UI/Fonts/ErosRegular-DOXn3.ttf");
	scoreboardFont = Radiant::GuiManager::GetFont(PONG_FONT, 48);
	
	m_ball.GetScore(player1_score, player2_score);
}

void Scoreboard::OnUpdate(const float deltaTime)
{
	m_ball.GetScore(player1_score, player2_score);

	if (Radiant::Input::CheckWindowResize()) {
		m_window_width = Radiant::Renderer::GetWindowWidth();
		m_window_height = Radiant::Renderer::GetWindowHeight();
	}
}

void Scoreboard::OnRender()
{
	// First render config
	ImGui::SetNextWindowSize(ImVec2(m_gui_width, m_gui_height), ImGuiCond_Appearing);
	ImGui::SetNextWindowPos(ImVec2(m_window_width/2 - (m_gui_width/2), 40));

	ImGui::PushFont(scoreboardFont);

	// Start drawing scoreboard
	ImGui::Begin("Scoreboard", NULL, m_window_config);

	std::string player1_score_text = std::to_string(player1_score);
	std::string player2_score_text = std::to_string(player2_score);

	ImGui::SetCursorPosX(10);
	ImGui::Text(player1_score_text.c_str());
	ImGui::SameLine();
	
	auto windowWidth = ImGui::GetWindowSize().x;
	auto textWidth = ImGui::CalcTextSize(player2_score_text.c_str()).x;
	ImGui::SetCursorPosX(windowWidth - textWidth - 10);
	ImGui::Text(player2_score_text.c_str());

	ImGui::End();
	ImGui::PopFont();

}
