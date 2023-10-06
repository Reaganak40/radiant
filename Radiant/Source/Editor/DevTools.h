#pragma once
#include "Scene/Layer.h"
#include "Utils/Input.h"
#include "Utils/ConfigReader.h"

namespace rdt::core {

	enum DevMessages {
		DM_LaunchTemplateWizard = 0xb33f,
	};

	class DevLayer : public Layer {
	private:
		bool m_showTools;
		std::string m_base_directory;
		std::string m_projectName;
		std::string m_resources_filepath;

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

	/*
		Editor Themes Implementation
	*/
	enum EditorTheme {
		Theme_Codz,
	};

	enum EditorFonts {
		NunitoSans = 1,
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
		int m_menu_bar_height;

		bool m_templateWizardLaunched;
		int m_template_selection_index;
		char m_template_name[60];
		bool m_template_name_edited;

		std::unordered_map<unsigned int, ImFont*> m_fonts;

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

		enum TemplateType {
			T_GameObject,
			T_Layer,
			T_Scene,
		};


		/*
			Panels
		*/
		GuiConfig m_diagnostics_panel;
		GuiConfig m_scene_panel;
		GuiConfig m_template_wizard;

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
		void ApplyGuiConfig(const GuiConfig& config);
		void SetScenePtr(Scene* ptr);
		void AddCenteredText(const std::string& text);
		void InactiveButtonBegin();
		void InactiveButtonEnd();
		void InactiveTextBoxBegin();
		void InactiveTextBoxEnd();
		void CreateFileFromTemplate(TemplateType type, const std::string& name);

		bool ValidTemplateName(const std::string& name, std::string& errorMsg);
		static int MyTextCallback(ImGuiInputTextCallbackData* data);

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
		void RenderMenuBar();
		// =======================================================
		void RenderDiagnosticsPanel();
		// =======================================================
		void RenderScenePanel();
		void AddLayerPanel(Layer* layer);
		void AddGameObjectPanel(GameObject* gobject);
		// =======================================================
		void RenderTemplateWizard();
		// =======================================================

	};
}