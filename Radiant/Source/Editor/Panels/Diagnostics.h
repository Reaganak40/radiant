#pragma once
#include "Gui/Gui.h"

namespace rdt {

	class DiagnosticsGUI : public GuiTemplate {
	private:
		const float m_gui_width;
		const float m_gui_height;

		int m_window_width;
		int m_window_height;

		ImFont* m_font;
	public:
		DiagnosticsGUI();

		void OnUpdate(const float deltaTime) override;
		void OnRender() override;
	};
}