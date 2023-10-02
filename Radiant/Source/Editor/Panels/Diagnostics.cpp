#include "pch.h"
#include "Diagnostics.h"
#include "Graphics/Renderer.h"

namespace rdt::core {
	DiagnosticsGUI::DiagnosticsGUI()
	{
		SetGuiDimensions(DiagnosticGuiWidth, DiagnosticGuiHeight);
		SetGuiPosition(
			GetDockPosX(DockRight, PanelMargin),
			GetDockPosY(DockTop, PanelMargin)
		);
	}

	void DiagnosticsGUI::OnUpdate(const float deltaTime)
	{
	}

	void DiagnosticsGUI::OnRender()
	{
		PrepareRender();

		// Start drawing window
		ImGui::Begin("Diagnostic Tools");

		// Show framerate
		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::Text("Performance: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

	}
	void DiagnosticsGUI::OnMessage(Message msg)
	{
	}
}