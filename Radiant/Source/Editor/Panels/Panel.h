#pragma once
#include "Gui/Gui.h"
#include "Messaging/Messenger.h"

namespace rdt::core {
	constexpr int PanelMargin = 10;

	constexpr float DiagnosticGuiWidth = 290.0f;
	constexpr float DiagnosticGuiHeight = 85.0f;

	constexpr float ScenePanelGuiWidth = 290.0f;
	constexpr float ScenePanelGuiHeight = 400.0f;

	class Panel : public GuiTemplate, public Messenger {
	private:
	public:
		Panel();
		~Panel();
	};
}