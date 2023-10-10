#include "pch.h"
#include "RenderWindow.h"
#include "Renderer.h"
namespace rdt {
	RenderWindow::RenderWindow()
		: m_texture(nullptr)
	{
		m_gui_width = 1280;
		m_gui_height = 720;

		m_gui_pos.x = (Renderer::GetWindowWidth() / 2) - (m_gui_width / 2);
		m_gui_pos.y = (Renderer::GetWindowHeight() / 2) - (m_gui_height / 2);

	}
	RenderWindow::~RenderWindow()
	{
	}
	
	void RenderWindow::OnBegin()
	{
		// First render config
		ImGui::SetNextWindowSize(ImVec2(m_gui_width, m_gui_height), ImGuiCond_Appearing);
		ImGui::SetNextWindowPos(ImVec2(m_gui_pos.x, m_gui_pos.y), ImGuiCond_Appearing);

		ImGui::Begin("My Scene");
	}

	Vec2d RenderWindow::UpdateAndGetWindowSize()
	{
        // we access the ImGui window size
        m_textureSize = ImGui::GetContentRegionAvail();
		
		return m_textureSize;
	}

	void RenderWindow::OnRender()
	{
        // we get the screen position of the window
        ImVec2 pos = ImGui::GetCursorScreenPos();

        // and here we can add our created texture as image to ImGui
        // unfortunately we need to use the cast to void* or I didn't find another way tbh
        ImGui::GetWindowDrawList()->AddImage(
			m_texture,
            ImVec2(pos.x, pos.y),
            ImVec2(pos.x + m_textureSize.x, pos.y + m_textureSize.y),
            ImVec2(0, 1),
            ImVec2(1, 0)
        );
	}

	void RenderWindow::OnEnd()
	{
        ImGui::End();
	}

	void RenderWindow::AssignTexture(void* nTexture)
	{
		m_texture = nTexture;
	}
}
