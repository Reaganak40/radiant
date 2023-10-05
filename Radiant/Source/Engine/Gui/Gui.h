#pragma once
#include "Core.h"
#include "Logging/Log.h"
#include "Utils/MathTypes.h"

namespace rdt {
	class RADIANT_API GuiTemplate {
	private:
		float m_gui_width;
		float m_gui_height;

	protected:
		int m_window_width;
		int m_window_height;

		Vec2i m_gui_pos;

	protected:

		/*
			Sets the width and height of the Gui
		*/
		void SetGuiDimensions(int nGuiWidth, int nGuiHeight);

		/*
			Sets the position of the Gui
		*/
		void SetGuiPosition(int nX, int nY);

		/*
			Gui template internal procedures to be called before rendering.
		*/
		void PrepareRender();

	public:
		GuiTemplate();
		~GuiTemplate();


		virtual void OnUpdate(const float deltaTime) {}
		virtual void OnRender() { RDT_WARN("Using virtual GuiTemplate::OnRender()!"); }
		static void RenderImGui();
	protected:
	};

}