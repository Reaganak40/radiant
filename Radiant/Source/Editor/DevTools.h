#pragma once
#include "Scene/Layer.h"
#include "Utils/Input.h"
#include "Utils/ConfigReader.h"
#include "Graphics/RenderWindow.h"

namespace rdt::core {

	enum DevMessages {
		DM_LaunchTemplateWizard = 0xb33f,
	};

	class DevLayer : public Layer {
	private:
		std::string m_base_directory;
		std::string m_projectName;
		std::string m_resources_filepath;

		ConfigReader m_config;

		DevLayer();
		~DevLayer();

		static DevLayer* m_instance;

	public:
		static void Destroy();
		static DevLayer* GetInstance();

		void OnAttach() override final;
		void OnDetach() override final;

		void OnMessage(Message msg) override final;

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

	private:
	};

	// =====================================================================================

	class GameWindowPanel : public RenderWindow {
	private:
		bool update_pos;
	public:
		GameWindowPanel();
		~GameWindowPanel();

		void OnBegin() override final;
		void OnEnd() override final;
		void TriggerUpdatePos();
	};

	// =====================================================================================
	/*
		Editor Themes Implementation
	*/
	enum EditorTheme {
		Theme_Codz,
	};

	enum EditorFont {
		NunitoSans = 1,
		ForkAwesome,
	};

	struct ThemeData {
		ImVec4 Titlebackground;
		ImVec4 TitlebackgroundActive;
		ImVec4 TitleBackgroundCollapsed;
		ImVec4 HeaderColor;
		ImVec4 HeaderHoverColor;
		ImVec4 WindowBackground;
		ImVec4 TextColor;
		ImVec4 MenuBarBackground;
		ImVec4 PopupBackground;
	};

	// =====================================================================================
	/*
		Editor Layout Implementation
	*/
	class EditorLayout : public GuiTemplate, public Messenger {
	private:
		/*
			Development dependables
		*/
		Scene* m_scene;
		ThemeData theme_data;
		std::string sourcePath;
		std::string templatePath;
		bool first_render;
		float m_menu_bar_height;

		bool m_templateWizardLaunched;
		int m_template_selection_index;
		char m_template_name[60];
		bool m_template_name_edited;

		bool m_showTools;
		const std::vector<InputState> controls_ShowTools1{ CTRL_KEY_DOWN };
		const std::vector<InputState> controls_ShowTools2{ T_KEY_PRESS };

		std::unordered_map<EditorFont, std::unordered_map<unsigned int, ImFont*>> m_fonts;

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
			bool update = false;
		};

		enum TemplateType {
			T_GameObject,
			T_Layer,
			T_Scene,
		};

		/*
			Panels
		*/
		GameWindowPanel* m_game_window_panel;
		int m_gameWindowId;

		GuiConfig m_diagnostics_panel;
		GuiConfig m_scene_panel;
		GuiConfig m_template_wizard;
		GuiConfig m_game_window_settings_panel;
		GuiConfig m_console_panel;

	public:
		EditorLayout();
		~EditorLayout();

		void OnMessage(Message msg) override final;
		void OnUpdate(const float deltaTime) override;
		void OnRender() override;

		void SetTheme(EditorTheme nTheme);
		void SetSourcePath(const std::string& path);

		void InitResources(std::string& resourcePath);

	private:
		void OnFirstRender();
		void AddFont(EditorFont name, std::string& ttfFile, const std::vector<unsigned int>& sizes);
		void ApplyGuiConfig(GuiConfig& config);
		void SetScenePtr(Scene* ptr);
		void AddCenteredText(const std::string& text);
		void InactiveButtonBegin();
		void InactiveButtonEnd();
		void InactiveTextBoxBegin();
		void InactiveTextBoxEnd();
		void CreateFileFromTemplate(TemplateType type, const std::string& name);
		float GetButtonWidth(const char* label);
		float GetButtonHeight(const char* label);

		bool ValidTemplateName(const std::string& name, std::string& errorMsg);
		static int MyTextCallback(ImGuiInputTextCallbackData* data);

		/*
			Returns the docking x-position for the Gui to be docked in the
			provided direction with the given margin.
		*/
		float GetDockPosX(Dock docking, float guiWidth, float margin = 0);

		/*
			Returns the docking y-position for the Gui to be docked in the
			provided direction with the given margin.
		*/
		float GetDockPosY(Dock docking, float guiHeight, float margin = 0);


		// =======================================================
		void RenderMenuBar();
		// =======================================================
		void RenderDiagnosticsPanel();
		// =======================================================
		void RenderScenePanel();
		void AddLayerPanel(Layer* layer);
		void AddGameObjectPanel(GameObject* gobject);
		// =======================================================
		void RenderGameWindowSettingsPanel();
		// =======================================================
		void RenderConsolePanel();
		// =======================================================
		void RenderTemplateWizard();
		// =======================================================

	};
}