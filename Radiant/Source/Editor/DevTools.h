#pragma once
#include "Components/Scene/Layer.h"
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

	/*
		ImGui Panel that contains the game viewport
	*/	
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
	enum PanelType {
		PT_NAP, // Not a Panel
		MenuBar,
		DiagnosticsPanel,
		TemplateWizard,
		EntityWizard,
	};
	struct MenuBarData;

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

	private:
		void ApplyGuiConfig();
	};

	// =====================================================================================
	enum PanelPlacement {
		NotActive,
		DockBorderTop,
		DockTop,
		DockFill,
		DockBottom,
		DockLeft,
		DockRight,
		DockBorderLeft,
		DockBorderRight,
		DockBorderBottom,
		Floating,
		DockEndOfOptions
	};
	enum CommonWorkspaces {
		Default,
		Fullscreen,
	};
	class PanelManager {
	private:
		std::unordered_map<PanelType, Panel> m_panels;

		struct Workspace {
			std::unordered_map<PanelType, PanelPlacement> m_panel_placements;
			std::vector<Panel&> m_active_panels[DockEndOfOptions];

			// Defines the parent container for all panels the reside in that placement option
			PanelConfig m_dock_containers[DockEndOfOptions];
		};

		std::unordered_map<int, Workspace> m_workspaces;
		int m_workspace_id;

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
			Sets the panel location and visibility
		*/
		void SetPanelPlacement(PanelType panel, PanelPlacement placement);

		/*
			Renders all panels as a multiple document interface
		*/
		void RenderMDI();

		/*
			Gets the current broadcasted data from each panel.
		*/
		const std::unordered_map<PanelType, void*>& GetMessages();

	private:

		/*
			Returns the currently active workspace
		*/
		Workspace& ActiveWorkspace();

		/*
			Updates the config information for all dock containers
		*/
		void UpdateDockConainers(PanelPlacement updatedContainer);

		/*
			Adds a panel to the described dock container
		*/
		void AddToDockContainer(PanelPlacement container, PanelType panel);
	};

	// =====================================================================================
	/*
		Editor Themes Implementation
	*/
	enum EditorTheme {
		Theme_Codz,
		Nightingale,
		ET_NAT, // Not a theme
	};

	enum EditorFont {
		NunitoSans = 1,
		ForkAwesome,
	};

	// =====================================================================================
	/*
		Editor Layout Implementation
	*/
	class EditorLayout : public GuiTemplate, public Messenger {
	private:
		PanelManager m_panel_manager;

		/*
			Development dependables
		*/
		Scene* m_scene;
		std::string sourcePath;
		std::string templatePath;
		bool first_render;

		// Template Wizard
		bool m_templateWizardLaunched;
		int m_template_selection_index;
		char m_template_name[60];
		bool m_template_name_edited;

		// Entity Wizard
		bool m_entityWizardLaunched;
		char m_entity_name[60];

		bool m_showTools;
		const std::vector<InputState> controls_ShowTools1{ CTRL_KEY_DOWN };
		const std::vector<InputState> controls_ShowTools2{ T_KEY_PRESS };

		std::unordered_map<EditorFont, std::unordered_map<unsigned int, ImFont*>> m_fonts;
		
		std::string m_last_log;

		ConfigReader* m_config;

		/*
			Gui Layout data structures
		*/
		enum Dock {
			DockLeft,
			DockRight,
			DockTop,
			DockBottom
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

	public:
		EditorLayout();
		~EditorLayout();

		void OnMessage(Message msg) override final;
		void OnUpdate(const float deltaTime) override;
		void OnRender() override;

		void SetTheme(EditorTheme nTheme);
		void SetSourcePath(const std::string& path);

		void InitResources(std::string& resourcePath);

		void AddConfigPtr(ConfigReader* ptr);
		void ApplyConfig();

	private:
		
		// ===============================================
		void ProcessMessages();
		void OnMenuBarMessage(MenuBarData* data);
		// ===============================================
		void OpenPanel(PanelType panel);
		// ===============================================

		void OnFirstRender();
		void AddFont(EditorFont name, std::string& ttfFile, const std::vector<unsigned int>& sizes);
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
		void RenderEntityWizard();
		// =======================================================

	};
}