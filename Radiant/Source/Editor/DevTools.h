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
		ConsolePanel,
		GameWindowPanel,
		GameWindowSettingsPanel,
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
	};

	// =====================================================================================
	
	enum ContainerType {
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

	class Container {
	public:
		struct Impl;
	private:
		Impl* m_impl;
		ContainerType m_type;
		PanelConfig m_config;
	public:

		Container(ContainerType type = DockEndOfOptions);
		~Container();

		/*
			Sets the container type, defining its location and size
			in the MDI application.
		*/
		void SetContainerType(ContainerType type);

		/*
			Returns the current type of this container.
		*/
		ContainerType GetType();

		/*
			Renders the container with ImGui and its child panels
		*/
		void RenderContainer();

		/*
			Adds a new panel to the container
		*/
		void AddToContainer(Panel* nPanel);

		/*
			Removes a currently bound panel of the given type.
		*/
		void RemoveFromContainer(PanelType type);

		/*
			Gets the panel configuration (editable)
		*/
		PanelConfig& GetConfig();

	private:
	};

	struct ContainerNode {
		Container container;
		ContainerNode* nodeUp = nullptr;
		ContainerNode* nodeDown = nullptr;
		ContainerNode* nodeLeft = nullptr;
		ContainerNode* nodeRight = nullptr;

		void SetContainerType(ContainerType type) {
			container.SetContainerType(type);
		}

		ContainerType GetType() {
			return container.GetType();
		}
	};

	enum CommonWorkspaces {
		Default,
		Fullscreen,
	};

	using Workspace = std::unordered_map<ContainerType, ContainerNode*>;

	class PanelManager {
	private:
		std::unordered_map<PanelType, Panel*> m_panels;
		std::unordered_map<PanelType, ContainerType> m_panel_locations;

		std::unordered_map<int, Workspace> m_workspaces;
		int m_workspace_id;

		std::unordered_map<PanelType, void*> m_messages;

		enum TreeDirection
		{
			TD_LEFT,
			TD_RIGHT,
			TD_UP,
			TD_DOWN,
			TD_NOMOVEMENT
		};

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
		void AssignToContainer(PanelType panel, ContainerType container);

		/*
			Removes a panel from its currently bound container, making it inactive.
		*/
		void RemoveFromContainer(PanelType panel);

		/*
			Renders all panels as a multiple document interface
		*/
		void RenderMDI();

		/*
			Gets the current broadcasted data from each panel.
		*/
		const std::unordered_map<PanelType, void*>& GetMessages();

		/*
			Registers a new container type to the current workspace
		*/
		void RegisterContainer(ContainerType type);

	private:

		/*
			Switches the current workspace to the one with the registered workspace ID
		*/
		void SetCurrentWorkspace(int workspaceID);

		/*
			Returns the currently active workspace
		*/
		Workspace& GetCurrentWorkspace();


		/*
			Returns true if a container of the given type exists
		*/
		bool ContainerExists(ContainerType type);

		/*
			Gets the config for a container given its type
		*/
		PanelConfig& GetContainerConfig(ContainerType type);

		/*
			Gets the root of the container tree, which is always the DockFill
			container.
		*/
		ContainerNode* GetHead();

		ContainerType GetNextExpected(ContainerType current, ContainerType target, TreeDirection& next);
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
	class Editor : public GuiTemplate, public Messenger {
	public:
		static std::string sourcePath;
		static std::string templatePath;
		static Scene* m_scene;
		static std::unordered_map<EditorFont, std::unordered_map<unsigned int, ImFont*>> m_fonts;
		static GameRenderWindow* m_gameWindow;
		static int m_gameWindowId;
	private:
		PanelManager m_panel_manager;
		ConfigReader* m_config;
		bool first_render;
		bool m_showTools;
		const std::vector<InputState> controls_ShowTools1{ CTRL_KEY_DOWN };
		const std::vector<InputState> controls_ShowTools2{ T_KEY_PRESS };
	public:
		Editor();
		~Editor();

		void OnMessage(Message msg) override final;
		void OnUpdate(const float deltaTime) override;
		void OnRender() override;

		void SetTheme(EditorTheme nTheme);
		void SetSourcePath(const std::string& path);

		void InitResources(std::string& resourcePath);

		void AddConfigPtr(ConfigReader* ptr);
		void ApplyConfig();

		// ===========================================================
		/* Utility functions used by many panels */

		static float GetButtonWidth(const char* label);
		static float GetButtonHeight(const char* label);
		static void AddCenteredText(const std::string& text);
		static void InactiveButtonBegin();
		static void InactiveButtonEnd();
		static void InactiveTextBoxBegin();
		static void InactiveTextBoxEnd();
		static int MyTextCallback(ImGuiInputTextCallbackData* data);
		static void ApplyGuiConfig(PanelConfig& config);
		// ===========================================================

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
	};
}