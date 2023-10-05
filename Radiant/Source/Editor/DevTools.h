#pragma once
#include "Scene/Layer.h"
#include "Utils/Input.h"
#include "Utils/ConfigReader.h"

namespace rdt::core {

	class DevLayer : public Layer {
	private:
		bool m_showTools;
		std::string m_base_directory;
		std::string m_projectName;

		ConfigReader m_config;

		const std::vector<InputState> controls_ShowTools1{CTRL_KEY_DOWN};
		const std::vector<InputState> controls_ShowTools2{T_KEY_PRESS};

		DevLayer();
		~DevLayer();

		static DevLayer* m_instance;

	public:
		static void Destroy();
		static DevLayer* GetInstance();

		void OnAttach() override final;
		void OnDetach() override final;

		/*
			Function called when the layer is active and the host application
			calls ProcessInput.
		*/
		void OnProcessInput(const float deltaTime) override final;

		/*
			Function called when the layer is active and the host application
			calls Render()
		*/
		void OnRender() override final;
	};
	
	// =====================================================================================
	
	/*
		Panel Layout Macros
	*/ 
	constexpr int PanelMargin = 10;

	constexpr float DiagnosticGuiWidth = 290.0f;
	constexpr float DiagnosticGuiHeight = 85.0f;

	constexpr float ScenePanelGuiWidth = 290.0f;
	constexpr float ScenePanelGuiHeight = 400.0f;

	// =====================================================================================

	/*
		Editor Themes Implementation
	*/
	enum EditorTheme {
		Theme_Gray
	};

	struct ThemeData {
		ImVec4 Titlebackground;
		ImVec4 TitlebackgroundActive;
		ImVec4 TitleBackgroundCollapsed;
		ImVec4 HeaderColor;
		ImVec4 WindowBackground;
		ImVec4 TextColor;
		ImVec4 MenuBarBackground;
	};

	class EditorLayout : public GuiTemplate, public Messenger {
	private:
		/*
			Development dependables
		*/
		Scene* m_scene;
		ThemeData theme_data;

		/*
			Gui Layout data structures
		*/
		enum Dock {
			DockLeft,
			DockRight,
			DockTop,
			DockBottom
		};

		struct GuiConfig {
			float xPos = 0;
			float yPos = 0;
			float width = 0;
			float height = 0;
		};

		/*
			Panels
		*/
		GuiConfig m_diagnostics_panel;
		GuiConfig m_scene_panel;

	public:
		EditorLayout();
		~EditorLayout();

		void OnMessage(Message msg) override final;
		void OnUpdate(const float deltaTime) override;
		void OnRender() override;

		void SetTheme(EditorTheme nTheme);

	private:
		void ThemeBegin();
		void ThemeEnd();
		void ApplyGuiConfig(const GuiConfig& config);
		void SetScenePtr(Scene* ptr);

		/*
			Returns the docking x-position for the Gui to be docked in the
			provided direction with the given margin.
		*/
		int GetDockPosX(Dock docking, int guiWidth, int margin = 0);

		/*
			Returns the docking y-position for the Gui to be docked in the
			provided direction with the given margin.
		*/
		int GetDockPosY(Dock docking, int guiHeight, int margin = 0);

		// =======================================================
		void RenderDiagnosticsPanel();
		// =======================================================
		void RenderScenePanel();
		void AddLayerPanel(Layer* layer);
		void AddGameObjectPanel(GameObject* gobject);
		// =======================================================
		void RenderMenuBar();
		// =======================================================

	};
}