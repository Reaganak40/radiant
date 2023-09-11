#include "pch.h"
#include "Diagnostics.h"
#include "Renderer/Renderer.h"

namespace rdt {
	DiagnosticsGUI::DiagnosticsGUI()
		: m_gui_width(295.0f), m_gui_height(85.0f)
	{
		m_window_width = Renderer::GetWindowWidth();
		m_window_height = Renderer::GetWindowHeight();

		ImGuiIO& io = ImGui::GetIO();
		m_font = io.Fonts->AddFontDefault();
	}

	void DiagnosticsGUI::OnUpdate(const float deltaTime)
	{
	}

	void DiagnosticsGUI::OnRender()
	{
		// First render config
		ImGui::SetNextWindowSize(ImVec2(m_gui_width, m_gui_height), ImGuiCond_Appearing);
		ImGui::SetNextWindowPos(ImVec2(m_window_width - m_gui_width - 10, 10), ImGuiCond_Appearing);

		ImGui::PushFont(m_font);
		// Start drawing window
		ImGui::Begin("Diagnostic Tools");

		// Show framerate
		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::Text("Performance: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		ImGui::PopFont();

	}
}