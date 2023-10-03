#include "pch.h"
#include "Gui.h"
#include "Graphics/Renderer.h"

namespace rdt {
	void GuiTemplate::SetGuiDimensions(int nGuiWidth, int nGuiHeight)
	{
		m_gui_width = nGuiWidth;
		m_gui_height = nGuiHeight;
	}
	void GuiTemplate::SetGuiPosition(int nX, int nY)
	{
		m_gui_pos.x = nX;
		m_gui_pos.y = nY;
	}

	int GuiTemplate::GetDockPosX(Dock docking, int margin)
	{
		switch (docking) {
		case DockLeft:
			return margin;
		case DockRight:
			return m_window_width - m_gui_width - margin;
		case DockTop:
			return 0;
		case DockBottom:
			return 0;
		}

		return 0;
	}

	int GuiTemplate::GetDockPosY(Dock docking, int margin)
	{
		switch (docking) {
		case DockLeft:
			return 0;
		case DockRight:
			return 0;
		case DockTop:
			return margin;
		case DockBottom:
			return m_window_height - m_gui_height - margin;
		}

		return 0;
	}

	void GuiTemplate::PrepareRender()
	{
		// First render config
		ImGui::SetNextWindowSize(ImVec2(m_gui_width, m_gui_height), ImGuiCond_Appearing);
		ImGui::SetNextWindowPos(ImVec2(m_gui_pos.x, m_gui_pos.y), ImGuiCond_Appearing);
	}

	GuiTemplate::GuiTemplate()
	{
		m_window_width = Renderer::GetWindowWidth();
		m_window_height = Renderer::GetWindowHeight();
		m_gui_width = 100;
		m_gui_height = 100;
	}
	GuiTemplate::~GuiTemplate()
	{
	}
	void GuiTemplate::RenderImGui()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}