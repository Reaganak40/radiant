#pragma once
#include "Core.h"
#include "Logging/Log.h"
#include "Utils/MathTypes.h"

namespace rdt {
	class RADIANT_API GuiTemplate {
	private:
		int m_window_width;
		int m_window_height;
		
		float m_gui_width;
		float m_gui_height;

		Vec2i m_gui_pos;

	protected:

		enum Dock {
			DockLeft,
			DockRight,
			DockTop,
			DockBottom
		};

		/*
			Sets the width and height of the Gui
		*/
		void SetGuiDimensions(int nGuiWidth, int nGuiHeight);

		/*
			Sets the position of the Gui
		*/
		void SetGuiPosition(int nX, int nY);

		/*
			Returns the docking x-position for the Gui to be docked in the 
			provided direction with the given margin.
		*/
		int GetDockPosX(Dock docking, int margin = 0);

		/*
			Returns the docking y-position for the Gui to be docked in the
			provided direction with the given margin.
		*/
		int GetDockPosY(Dock docking, int margin = 0);

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