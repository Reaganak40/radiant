#pragma once

// Forward Declarations
namespace rdt {
	struct DebugComponent;
	using Entity = unsigned int;
	using glTextureID = unsigned int;
}

#include "OOComponents/Scene/Layer.h"
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
		NunitoSans_Bold,
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

		static std::unordered_map<std::string, int> m_combo_selections;
		static std::unordered_map<std::string, bool> m_checkbox_selections;

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
		static int GetComboxBoxResult(const char* label);
		static int AddComboBox(const char* label, const char** options, unsigned int optionCount, const char* preview = nullptr);
		static void AddIcon(const char* unicode, size_t size = 18);
		static int MyTextCallback(ImGuiInputTextCallbackData* data);
		static bool* GetCheckboxSelection(const std::string& checkbox_label);
		static DebugComponent* GetDebugComponent(Entity entity);
		static void ApplyGuiConfig(PanelConfig& config);
		static glTextureID GetImGuiTextureData(TextureID tID, int& imageWidth, int& imageHeight);
		// ===========================================================

	private:
		
		// ===============================================
		void ProcessMessages();
		void OnOpenPanelRequest(OpenPanelRequestData* data);
		void OnChangeThemeRequest(ChangeThemeRequestData* data);
		// ===============================================
		void OpenPanel(PanelType panel);
		// ===============================================

		void OnFirstRender();
		void AddFont(EditorFont name, std::string& ttfFile, const std::vector<unsigned int>& sizes);
		void SetScenePtr(Scene* ptr);
	};
}