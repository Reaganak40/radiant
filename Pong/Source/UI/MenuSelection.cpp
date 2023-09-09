#include "MenuSelection.h"

MenuSelection::MenuSelection()
	: m_gui_width(300), m_gui_height(300), m_choice(NoOptionSelected)
{
	m_window_width = Radiant::Renderer::GetWindowWidth();
	m_window_height = Radiant::Renderer::GetWindowHeight();
	m_window_config = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

	Radiant::GuiManager::LoadFont(PONG_FONT, "Source/UI/Fonts/ErosRegular-DOXn3.ttf");
	m_menu_font = Radiant::GuiManager::GetFont(PONG_FONT, 18);
}


MenuSelection::~MenuSelection()
{
}

void MenuSelection::OnUpdate(const float deltaTime)
{
	if (Radiant::Input::CheckWindowResize()) {
		m_window_width = Radiant::Renderer::GetWindowWidth();
		m_window_height = Radiant::Renderer::GetWindowHeight();
	}
}

void MenuSelection::OnRender()
{
	// First render config
	ImGui::SetNextWindowSize(ImVec2(m_gui_width, m_gui_height), ImGuiCond_Appearing);
	ImGui::SetNextWindowPos(ImVec2((float)(m_window_width / 2.0 - (m_gui_width / 2.0)), m_window_height * 0.3f));

	ImGui::PushFont(m_menu_font);

	// Start drawing title
	ImGui::Begin("Menu", NULL, m_window_config);
	
	Radiant::Vec2d mouseCoords = Radiant::Vec2d(ImGui::GetMousePos()) - Radiant::Vec2d(ImGui::GetWindowPos());

	ImGui::NewLine();

	if (AddSelection(mouseCoords, "Play Against Human")) {
		m_choice = PlayAgainstHuman;
	}
	ImGui::NewLine();
	
	if (AddSelection(mouseCoords, "Play Against Computer")) {
		m_choice = PlayAgianstComputer;
	}

	ImGui::End();


	ImGui::PopFont();
}

bool MenuSelection::AddSelection(const Radiant::Vec2d& mouseCoords, const std::string& text)
{
	auto textSize = ImGui::CalcTextSize(text.c_str());
	auto windowWidth = ImGui::GetWindowSize().x;
	
	ImGui::SetCursorPosX((windowWidth / 2) - (textSize.x / 2));

	Radiant::Vec2d cursorPos = ImGui::GetCursorPos();
	
	bool isHovered = false;
	if (mouseCoords.x >= 0 && mouseCoords.x <= windowWidth) {
		if (mouseCoords.y >= cursorPos.y && mouseCoords.y <= (cursorPos.y + textSize.y)) {
			isHovered = true;
		}
	}

	if (isHovered) {

		ImGui::TextColored(ImVec4(255, 0, 0, 255), text.c_str());

		const ImGuiIO& io = ImGui::GetIO();
		if (io.MouseReleased[0]) {
			return true;
		}
	}
	else {
		ImGui::Text(text.c_str());
	}

	return false;
}
