#pragma once
#include "Gui/Gui.h"

namespace rdt {

	class RenderWindow : public GuiTemplate {
	private:
		void* m_texture;
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

	private:
	};
}