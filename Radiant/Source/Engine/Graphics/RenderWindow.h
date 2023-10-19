#pragma once
#include "Gui/Gui.h"

namespace rdt {

	class RenderWindow : public GuiTemplate {
	private:
		void* m_texture;
		Vec2d m_textureSize;
		float m_title_bar_offset;
	public:
		RenderWindow();
		~RenderWindow();

		void AssignTexture(void* nTexture);

		virtual void OnUpdate(const float deltaTime) {}

		/*
			Called before OnRender to customize RenderWindows. This MUST call ImGui::Begin()
		*/
		virtual void OnBegin();

		/*
			Called in between OnBegin and OnRender to update the window size info and pass it
			to the Renderer.
		*/
		Vec2d UpdateAndGetWindowSize();

		void OnRender() override final;

		/*
			Called before OnRender to customize RenderWindows. This MUST call ImGui::End()
		*/
		virtual void OnEnd();

		/*
			Returns the xy coordinates of this window on its last rendered.
		*/
		Vec2d GetLastPosition();

		/*
			Returns the width and height of the window when it was last rendered.
		*/
		Vec2d GetLastSize();

	private:
	};
}