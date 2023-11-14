#pragma once

// Forward Declarations
namespace rdt {
	class Scene;
	struct DebugComponent;
	using Entity = unsigned int;
	using glTextureID = unsigned int;
	using SceneID = unsigned int;
	using TextureID = unsigned int;
}

#include "Gui/Gui.h"
#include "Utils/ConfigReader.h"
#include "Graphics/RenderWindow.h"

namespace rdt::core {
	// =====================================================================================
	class GameRenderWindow : public RenderWindow {
	private:
		bool update_pos;
	public:
		GameRenderWindow();
		~GameRenderWindow();

		void OnBegin() override final;
		void OnEnd() override final;
		void TriggerUpdatePos();
	};
	// =====================================================================================
	enum PanelType {
		PT_NAP, // Not a Panel
		MenuBar,
		DiagnosticsPanel,
		ScenePanel,
		EntityHierarchyPanel,
		ConsolePanel,
		GameWindowPanel,
		GameWindowSettingsPanel,
		TemplateWizard,
		EntityWizard,
	};

	struct OpenPanelRequestData;
	struct ChangeThemeRequestData;

	struct PanelConfig {
		float xPos = 0;
		float yPos = 0;
		float width = 0;
		float height = 0;
		bool update = false;
	};

	class Panel {
	public:
		struct Impl;
	private:
		Impl* m_impl;
		PanelType m_type;
		PanelConfig m_config;
	public:

		Panel(PanelType type = PT_NAP);

		~Panel();

		/*
			Creates a new Panel of the provided type
		*/
		void SetPanelType(PanelType type);

		/*
			Call ImGui pipeline to render Panel
		*/
		void Render();

		/*
			Get the ouput data of the panel
		*/
		void* GetData();

		/*
			Gets the panel configuration (editable)
		*/
		PanelConfig& GetConfig();

		/*
			Gets the type of panel implementation for this instance
		*/
		PanelType GetType();

		/*
			Sets the flag to show the panel to true or false
		*/
		void SetShow(bool shouldShow);
	};

	// =====================================================================================
	class PanelManager {
	private:
		std::unordered_map<PanelType, Panel*> m_panels;
		std::unordered_map<PanelType, void*> m_messages;

	public:
		PanelManager();
		~PanelManager();

		/*
			Registers/Creates a new Panel instance of the provided type.
			Only 1 panel allowed per type.
		*/
		void RegisterPanel(PanelType type);

		/*
			Opens a panel, launching its window, and showing itself in the MDI
		*/
		void OpenPanel(PanelType type);

		/*
			Renders all panels as a multiple document interface
		*/
		void RenderMDI();

		/*
			Gets the current broadcasted data from each panel.
		*/
		const std::unordered_map<PanelType, void*>& GetMessages();

	private:
	};
	// =====================================================================================
	enum EditorTheme;
	enum EditorFont;

	class Editor : public GuiTemplate {
	private:
		Editor();
		~Editor();
		static Editor* m_instance;
	public:

		/*
			Creates a new instance of the Editor
		*/
		static void Initialize();

		/*
			Destroys the editor singleton
		*/
		static void Destroy();

		/*
			Renders (and updates) the Editor
		*/
		void OnRender() override final;

		/*
			Notifies the editor of the current scene.
		*/
		static void SetCurrentScene(std::shared_ptr<Scene> currentScene);

	private:
		std::string m_radiant_resources_filepath;
		std::string m_radiant_template_filepath;

		// Project Directory Layout
		std::string m_projectName;
		std::string m_project_base_directory;
		std::string m_project_source_directory;
		ConfigReader m_config;

		// Editor Resources
		bool m_showEditor;
		PanelManager m_panel_manager;

		/*
			Initialzation Procedures
		*/
		void SetSourcePath(const std::string& path);
		void InitResources();
		void AddFont(EditorFont name, std::string& ttfFile, const std::vector<unsigned int>& sizes);
		void ApplyConfig();
		void SetTheme(EditorTheme nTheme);

	public:
		static std::shared_ptr<Scene> m_scene;
		static std::unordered_map<EditorFont, std::unordered_map<unsigned int, ImFont*>> m_fonts;
		static GameRenderWindow* m_gameWindow;
		static int m_gameWindowId;
		static std::unordered_map<std::string, int> m_combo_selections;
		static std::unordered_map<std::string, bool> m_checkbox_selections;

		/* 
			Utility functions used by many panels
		*/
		static const std::string& GetProjectSourcePath();
		static const std::string& GetRadiantTemplatePath();
		static float GetButtonWidth(const char* label);
		static float GetButtonHeight(const char* label);
		static void AddCenteredText(const std::string& text);
		static void InactiveButtonBegin();
		static void InactiveButtonEnd();
		static void InactiveTextBoxBegin();
		static void InactiveTextBoxEnd();
		static int GetComboxBoxResult(const char* label);
		static int AddComboBox(const char* label, const char** options, unsigned int optionCount, const char* preview = nullptr);
		static void AddIcon(const char* unicode, size_t size = 18);
		static int MyTextCallback(ImGuiInputTextCallbackData* data);
		static bool* GetCheckboxSelection(const std::string& checkbox_label);
		static DebugComponent* GetDebugComponent(Entity entity);
		static void ApplyGuiConfig(PanelConfig& config);
		static glTextureID GetImGuiTextureData(TextureID tID, int& imageWidth, int& imageHeight);
	};
}