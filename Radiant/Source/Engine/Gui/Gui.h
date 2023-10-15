#pragma once
#include "Core.h"
#include "Logging/Log.h"
#include "Utils/MathTypes.h"

namespace rdt {
	class RADIANT_API GuiTemplate {
	protected:
		float m_gui_width;
		float m_gui_height;
		int m_window_width;
		int m_window_height;

		Vec2d m_gui_pos;

	protected:

		/*
			Sets the width and height of the Gui
		*/
		void SetGuiDimensions(int nGuiWidth, int nGuiHeight);


		/*
			Gui template internal procedures to be called before rendering.
		*/
		void PrepareRender();

	public:
		/*
			Sets the position of the Gui
		*/
		void SetGuiPosition(float nX, float nY);
		void SetGuiPositionX(float nX);
		void SetGuiPositionY(float nY);

		GuiTemplate();
		~GuiTemplate();


		virtual void OnUpdate(const float deltaTime) {}
		virtual void OnRender() { RDT_WARN("Using virtual GuiTemplate::OnRender()!"); }
		static void RenderImGui();
	protected:
	};

}