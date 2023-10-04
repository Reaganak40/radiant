#pragma once
#include "Gui/Gui.h"
#include "Messaging/Messenger.h"
#include "Utils/Color.h"

namespace rdt::core {
	constexpr int PanelMargin = 10;

	constexpr float DiagnosticGuiWidth = 290.0f;
	constexpr float DiagnosticGuiHeight = 85.0f;

	constexpr float ScenePanelGuiWidth = 290.0f;
	constexpr float ScenePanelGuiHeight = 400.0f;


	enum EditorTheme {
		Theme_Gray
	};

	struct ThemeData {
		ImVec4 Titlebackground;
		ImVec4 TitlebackgroundActive;
		ImVec4 WindowBackground;
		ImVec4 TextColor;
	};

	class Panel : public GuiTemplate, public Messenger {
	private:
		static ThemeData theme_data;
	public:
		Panel();
		~Panel();

		static void SetTheme(EditorTheme nTheme);

	protected:

		void ThemeBegin();
		void ThemeEnd();
	};
}