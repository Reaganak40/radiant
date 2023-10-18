#include "pch.h"
#include "Gui.h"
#include "Graphics/Renderer.h"

namespace rdt {
	void GuiTemplate::SetGuiDimensions(int nGuiWidth, int nGuiHeight)
	{
		m_gui_width = (float)nGuiWidth;
		m_gui_height = (float)nGuiHeight;
	}
	void GuiTemplate::SetGuiPosition(float nX, float nY)
	{
		m_gui_pos.x = nX;
		m_gui_pos.y = nY;
	}

	void GuiTemplate::SetGuiPositionX(float nX)
	{
		m_gui_pos.x = nX;
	}

	void GuiTemplate::SetGuiPositionY(float nY)
	{
		m_gui_pos.y = nY;
	}

	Vec2f GuiTemplate::GetGuiDimensions()
	{
		return { m_gui_width, m_gui_height };
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