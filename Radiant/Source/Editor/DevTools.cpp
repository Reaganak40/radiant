#include "pch.h"
#include "DevTools.h"
#include "IconsForkAwesome.h"

#include "Messaging/MessageTypes.h"
#include "Graphics/Renderer.h"
#include "Physics/Physics.h"
#include "Utils/Utils.h"

#include "Gui/GuiManager.h"

namespace fs = std::filesystem;

namespace rdt::core {

	DevLayer* DevLayer::m_instance = nullptr;

	DevLayer::DevLayer()
	{
		RDT_CORE_WARN("Developer tools are enabled");

		RegisterToMessageBus("DevLayer");

		Editor* layout;
		RegisterGUI(layout = new Editor);

		m_base_directory = Utils::GetCWD();
		RDT_CORE_INFO("DevTools base directory: {}", m_base_directory);

		fs::path configFile = fs::path(m_base_directory) / fs::path("radiant.ini");
		m_config.SetTargetFile(configFile.generic_string());
		m_config.Read();

		fs::path sourceFile = fs::path(m_base_directory) / fs::path("Source");
		layout->SetSourcePath(sourceFile.generic_string());

		if (m_config.GetAttribute("Core", "ProjectName", m_projectName)) {
			RDT_CORE_TRACE("DevTools enabled for '{}'", m_projectName);
		}
		else {
			RDT_CORE_WARN("No project found!");
		}

		if (m_config.GetAttribute("Core", "Resources", m_resources_filepath)) {
			RDT_CORE_TRACE("Resources found at '{}'", m_resources_filepath);
			layout->InitResources(m_resources_filepath);
		}
		else {
			RDT_CORE_WARN("No resource filepath found!");
		}

		layout->AddConfigPtr(&m_config);
		layout->ApplyConfig();

		Renderer::SetBackgroundColor({0.2f, 0.2f, 0.2f, 1.0f});
	}
	DevLayer::~DevLayer()
	{
		m_config.Write();
	}

	void DevLayer::Destroy()
	{
		if (m_instance != nullptr) {
			delete m_instance;
			m_instance = nullptr;
		}
	}

	DevLayer* DevLayer::GetInstance()
	{
		if (m_instance == nullptr) {
			m_instance = new DevLayer;
		}

		return m_instance;
	}

	void DevLayer::OnAttach()
	{
		for (auto& gui : GetGUIs()) {
			Renderer::AttachGui(gui);
		}
	}

	void DevLayer::OnDetach()
	{
		for (auto& gui : GetGUIs()) {
			Renderer::DetachGui(gui);
		}
	}
	void DevLayer::OnMessage(Message msg)
	{
	
	}
	void DevLayer::OnProcessInput(const float deltaTime)
	{
		Layer::OnProcessInput(deltaTime);
	}

	void DevLayer::OnRender()
	{
		Layer::OnRender();
	}

	// ==============================================================================

	constexpr float GameWindowPanelGuiWidth = 1115;
	constexpr float GameWindowPanelGuiHeight = 653.0f;
	constexpr float PanelMargin = 7.5f;

	GameRenderWindow::GameRenderWindow()
	{
		m_gui_width = GameWindowPanelGuiWidth;
		m_gui_height = GameWindowPanelGuiHeight;
		update_pos = true;
	}

	GameRenderWindow::~GameRenderWindow()
	{
	}

	void GameRenderWindow::OnBegin()
	{
		ImGui::PushFont(GuiManager::GetFont(NunitoSans, 18));

		// First render config
		//ImGui::SetNextWindowSize(ImVec2(m_gui_width, m_gui_height), ImGuiCond_Appearing);
		
		/*if (update_pos) {
			ImGui::SetNextWindowPos(ImVec2((float)m_gui_pos.x, (float)m_gui_pos.y), ImGuiCond_Always);
			update_pos = false;
		}*/
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(1.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ResizeGrip, 0);

		ImGuiWindowFlags flags = 0;

		ImGui::Begin("Game Window", (bool*)0, flags);
		if (ImGui::GetMouseCursor() == ImGuiMouseCursor_ResizeNWSE) {
			ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
		}
	}

	void GameRenderWindow::OnEnd()
	{
		ImGui::End();
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();

		ImGui::PopFont();
	}

	void GameRenderWindow::TriggerUpdatePos()
	{
		update_pos = true;
	}

	// ==============================================================================
	/*
		PopUpWIndow Implementation
	*/

	namespace PanelDefaults {
		constexpr float DiagnosticGuiWidth = 300.0f;
		constexpr float DiagnosticGuiHeight = 105.0f;

		constexpr float ScenePanelGuiWidth = 300.0f;
		constexpr float ScenePanelGuiHeight = 400.0f;

		constexpr float ConsolePanelWidth = GameWindowPanelGuiWidth;
		constexpr float ConsolePanelHeight = 300.0f;

		constexpr float TemplateWizardGuiWidth = 500.0f;
		constexpr float TemplateWizardGuiHeight = 575.0f;

		constexpr float GameWindowSettingsPanelWidth = 250.0f;
	}

	struct Panel::Impl {

		/*
			Virtual function to render ImGui Window
		*/
		virtual void OnRender() {}

		/*
			Virtual function to get data from popup window.
		*/
		virtual void* OnGetData() { return nullptr; }

		/*
			Virtual function to set the config from the panel wrapper.
		*/
		virtual void ApplyConfigPreferences(PanelConfig& config) {}
	};

	// =========================================================
	struct MenuBarData {
		PanelType openPanelRequest;
		EditorTheme changeThemeRequest;

		MenuBarData() { Reset(); }

		void Reset() {
			openPanelRequest = PT_NAP;
			changeThemeRequest = ET_NAT;
		}
	};
	struct MenuBarImpl : public Panel::Impl {

		MenuBarData m_data;
		bool m_request_made;

		void OnRender() override final {

			if (m_request_made) {
				m_data.Reset();
				m_request_made = false;
			}

			if (ImGui::BeginMainMenuBar()) {

				if (ImGui::BeginMenu("File")) {
					if (ImGui::MenuItem("Create")) {
					}
					if (ImGui::MenuItem("Open", "Ctrl+O")) {
					}
					if (ImGui::MenuItem("Save", "Ctrl+S")) {
					}
					if (ImGui::MenuItem("Save as..")) {
					}
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Tools")) {
					if (ImGui::MenuItem("Template Wizard")) {
						m_data.openPanelRequest = TemplateWizard;
						m_request_made = true;
					}
					if (ImGui::MenuItem("Add Entity Definition")) {
						m_data.openPanelRequest = EntityWizard;
						m_request_made = true;
					}
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("View")) {

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Themes")) {

					if (ImGui::MenuItem("Codz")) {
						m_data.changeThemeRequest = Theme_Codz;
						m_request_made = true;
					}
					if (ImGui::MenuItem("Nightingale")) {
						m_data.changeThemeRequest = Nightingale;
						m_request_made = true;
					}

					ImGui::EndMenu();
				}

				ImGui::EndMainMenuBar();
			}
		}

		void* OnGetData() {
			if (m_request_made) {
				return &m_data;
			}

			return nullptr;
		}
	};
	// =========================================================
	struct DiagnosticsPanelImpl : public Panel::Impl {

		void ApplyConfigPreferences(PanelConfig& config) override final {
			config.width = PanelDefaults::DiagnosticGuiWidth;
			config.height = PanelDefaults::DiagnosticGuiHeight;

			config.xPos = 100;
			config.yPos = 100;
		}
		
		void OnRender() override final {
			
			// Start drawing window
			ImGui::Begin("Diagnostic Tools");

			// Show framerate
			Vec2d screenCoords = Input::GetMouseCoords(MouseCond::SCREEN_COORDS);
			Vec2d worldCoords = Input::GetMouseCoords(MouseCond::WORLD_COORDS);
			ImGui::Text("Mouse screen-coordinates: (%.2f %2.f)", screenCoords.x, screenCoords.y);
			ImGui::Text("Mouse world-coordinates: (%.2f %2.f)", worldCoords.x, worldCoords.y);
			ImGui::Text("Performance: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

			ImGui::End();
		}
	};
	// =========================================================
	struct GameWindowPanelImpl : public Panel::Impl {

		GameWindowPanelImpl()
		{
			Editor::m_gameWindow->SetShouldShow(true);

			Editor::m_gameWindow->SetGuiPositionY(88);
			Editor::m_gameWindow->SetGuiPositionX((Renderer::GetWindowWidth() / 2)
				- (Editor::m_gameWindow->GetGuiDimensions().x / 2));
		}
	};
	// =========================================================
	struct GameWindowSettingsPanelImpl : public Panel::Impl {
		void OnRender() override final {
			ImGuiWindowFlags windowConfig = 0;
			windowConfig |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;

			ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.7f, 0.7f, 0.7f, 0.9f });
			ImGui::Begin("##GameWindowSettingsPanel", (bool*)0, windowConfig);

			ImGui::PushFont(Editor::m_fonts[ForkAwesome][18]);
			ImGui::PushStyleColor(ImGuiCol_Button, { 1.0f, 1.0f, 1.0f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.8f, 0.8f, 0.8f, 1.0f });

			/* Play Button */
			ImGui::PushStyleColor(ImGuiCol_Text, { 0.2f, 0.8f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.2f, 0.9f, 0.2f, 0.2f });
			ImGui::SetCursorPos({ 10, 10 });
			if (ImGui::Button(ICON_FK_PLAY)) {

			}
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::SameLine();

			/* Pause Button */
			ImGui::PushStyleColor(ImGuiCol_Text, { 0.8f, 0.2f, 0.2f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.9f, 0.2f, 0.2f, 0.2f });
			if (ImGui::Button(ICON_FK_PAUSE)) {

			}
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::SameLine();

			/* Restart Button */
			ImGui::PushStyleColor(ImGuiCol_Text, { 0.1f, 0.1f, 0.1f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.7f, 0.7f, 0.7f, 0.2f });
			if (ImGui::Button(ICON_FK_REFRESH)) {

			}
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::SameLine();

			/* Expand Window Button*/
			ImGui::PushStyleColor(ImGuiCol_Text, { 0.1f, 0.1f, 0.1f, 1.0f });
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.7f, 0.7f, 0.7f, 0.2f });
			if (Renderer::UsingDefaultViewport()) {
				ImGui::SetCursorPosX(ImGui::GetWindowWidth() - Editor::GetButtonWidth(ICON_FK_PAUSE) - 10);
				if (ImGui::Button(ICON_FK_COMPRESS)) {

					if (Renderer::IsFullscreen()) {
						Renderer::DisableFullscreen();
					}

					Renderer::SetDefaultViewport(false);
					Input::SetTargetRenderWindow(Editor::m_gameWindowId);
					
					/*m_diagnostics_panel.yPos = GetDockPosY(DockTop, m_diagnostics_panel.height, PanelMargin) + m_menu_bar_height;
					m_game_window_settings_panel.xPos = GetDockPosX(DockRight, m_game_window_settings_panel.width + m_diagnostics_panel.width + PanelMargin, PanelMargin);
					m_game_window_settings_panel.yPos = GetDockPosY(DockTop, m_game_window_settings_panel.height, PanelMargin) + m_menu_bar_height;
					m_diagnostics_panel.update = true;
					m_game_window_settings_panel.update = true;*/
				}
			}
			else {
				ImGui::SetCursorPosX(ImGui::GetWindowWidth() - (2 * Editor::GetButtonWidth(ICON_FK_PAUSE)) - 18);

				if (ImGui::Button(ICON_FK_EXPAND)) {
					Renderer::SetDefaultViewport(true);
					Input::SetTargetRenderWindow(-1);
					
					/*m_showTools = false;
					m_game_window_settings_panel.yPos = PanelMargin;
					m_game_window_settings_panel.xPos = GetDockPosX(DockRight, m_game_window_settings_panel.width, PanelMargin);
					m_diagnostics_panel.yPos = m_game_window_settings_panel.yPos + m_game_window_settings_panel.height + PanelMargin;*/
				}

				ImGui::SameLine();
				if (ImGui::Button(ICON_FK_ARROWS_ALT)) {
					Renderer::SetDefaultViewport(true);
					Renderer::EnableFullscreen();
					Input::SetTargetRenderWindow(-1);

					/*m_showTools = false;
					m_game_window_settings_panel.yPos = PanelMargin;
					m_game_window_settings_panel.xPos = GetDockPosX(DockRight, m_game_window_settings_panel.width, PanelMargin);
					m_diagnostics_panel.yPos = m_game_window_settings_panel.yPos + m_game_window_settings_panel.height + PanelMargin;*/
				}
			}
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();


			ImGui::PopStyleColor();
			ImGui::PopStyleColor();

			ImGui::PopFont();

			ImGui::End();
			ImGui::PopStyleColor();
		}
	};
	// =========================================================
	struct ScenePanelImpl : public Panel::Impl {

		void ApplyConfigPreferences(PanelConfig& config) override final {
			config.width = PanelDefaults::ScenePanelGuiWidth;
			config.height = PanelDefaults::ScenePanelGuiHeight;

			config.xPos = 500;
			config.yPos = 100;
		}


		void AddGameObjectPanel(GameObject* gobject) {
			std::string name = typeid(*gobject).name();
			std::string panel_header = name.substr(6, name.size() - 6) + ": " + gobject->GetName();

			ImGui::Indent(10);
			if (ImGui::CollapsingHeader(panel_header.c_str())) {

				ImGui::Text("GameObjectID: %d", gobject->GetID());
				if (gobject->GetModelID() != 0 && gobject->GetRealmID() != 0) {
					ImGui::Text("RealmID: %d", gobject->GetRealmID());
					ImGui::Text("ModelID: %d", gobject->GetModelID());

					Vec2d pos = Physics::GetPosition(gobject->GetRealmID(), gobject->GetModelID());
					ImGui::Text("Position: (%.2f, %.2f)", pos.x, pos.y);

					Vec2d vel = Physics::GetVelocity(gobject->GetRealmID(), gobject->GetModelID());
					ImGui::Text("Velocity: (%.2f, %.2f)", vel.x, vel.y);

				}
				else {
					ImGui::Text("Not Registered to any realms.");
				}

			}
			ImGui::Unindent(10);
		}

		void AddLayerPanel(Layer* layer) {
			std::string name = typeid(*layer).name();
			std::string panel_header = "Layer: " + name.substr(6, name.size() - 6);

			if (ImGui::CollapsingHeader(panel_header.c_str())) {

				size_t count;
				GameObject** objects = layer->GetGameObjects(&count);
				for (size_t i = 0; i < count; i++) {
					AddGameObjectPanel(objects[i]);
				}
			}
		}
		void OnRender() override final {
			if (ImGui::Begin("Scene Panel")) {

				if (Editor::m_scene == nullptr) {
					ImGui::Text("No Scene Selected!");
				}
				else {
					ImGui::Text("Scene: %s", Editor::m_scene->GetName().c_str());

					size_t count;
					Layer** layers = Editor::m_scene->GetLayers(&count);
					for (size_t i = 0; i < count; i++) {
						AddLayerPanel(layers[i]);
					}
				}
			}

			ImGui::End();
		}
	};
	// =========================================================
	struct ConsolePanelImpl : public Panel::Impl {

		std::string m_last_log = "";

		void ApplyConfigPreferences(PanelConfig& config) override final {
			config.width = PanelDefaults::ConsolePanelWidth;
			config.height = PanelDefaults::ConsolePanelHeight;

			config.xPos = 200;
			config.yPos = 900;
		}

		void OnRender() override final {
			ImGui::Begin("Console Panel");

			int index = (int)Log::GetLogCount() - 1;
			std::string log;
			Color logColor;
			while (index >= 0) {
				Log::GetLog(index--, log, logColor);

				ImGui::PushStyleColor(ImGuiCol_Text, logColor.GetImGuiColor());
				ImGui::Text("%s", log.c_str());
				ImGui::PopStyleColor();
			}

			if (m_last_log != log) {
				ImGui::SetScrollHereY(0.999f);
				m_last_log = log;
			}

			ImGui::End();
		}
	};
	// =========================================================
	struct TemplateWizardImpl : public Panel::Impl {

		bool m_templateWizardLaunched = false;
		int m_template_selection_index;
		char m_template_name[60];
		bool m_template_name_edited;

		enum TemplateType {
			T_GameObject,
			T_Layer,
			T_Scene,
		};

		void CreateFileFromTemplate(TemplateType type, const std::string& name)
		{

			fs::path filepath = fs::path(Editor::sourcePath);
			fs::path template_h = fs::path(Editor::templatePath);
			fs::path template_cpp = fs::path(Editor::templatePath);


			switch (type) {
			case T_GameObject:
				filepath = filepath / fs::path("GameObjects");
				template_h = template_h / fs::path("gameObject.h.txt");
				template_cpp = template_cpp / fs::path("gameObject.cpp.txt");
				break;
			case T_Layer:
				filepath = filepath / fs::path("Layers");
				template_h = template_h / fs::path("layer.h.txt");
				template_cpp = template_cpp / fs::path("layer.cpp.txt");
				break;
			case T_Scene:
				filepath = filepath / fs::path("Scenes");
				template_h = template_h / fs::path("scene.h.txt");
				template_cpp = template_cpp / fs::path("scene.cpp.txt");
				break;
			default:
				return;
			}

			if (!Utils::PathExists(template_h.generic_string())) {
				RDT_CORE_WARN("Could not locate template header at '{}'", template_h.generic_string());
				return;
			}

			if (!Utils::PathExists(template_cpp.generic_string())) {
				RDT_CORE_WARN("Could not locate template source at '{}'", template_cpp.generic_string());
				return;
			}

			if (!fs::is_directory(filepath) || !fs::exists(filepath)) {
				fs::create_directory(filepath);
			}
			std::string headerFile = (filepath / fs::path(name + ".h")).generic_string();
			std::string srcFile = (filepath / fs::path(name + ".cpp")).generic_string();

			if (Utils::PathExists(headerFile) || Utils::PathExists(srcFile)) {
				RDT_CORE_WARN("Could not create from template, duplicate '{}' found!", name);
				return;
			}

			std::string contents;

			// Write header file;
			Utils::ReadTextFile(template_h.generic_string(), contents);
			Utils::ReplaceAll(contents, "TEMPLATENAME", name);
			Utils::WriteFile(headerFile, contents);
			contents.clear();

			// Write src file
			Utils::ReadTextFile(template_cpp.generic_string(), contents);
			Utils::ReplaceAll(contents, "TEMPLATENAME", name);
			Utils::WriteFile(srcFile, contents);

		}

		bool ValidTemplateName(const std::string& name, std::string& errorMsg) {
			if (name.length() == 0) {
				errorMsg = "Template name cannot be empty!";
				return false;
			}

			for (int i = 0; i < name.length(); i++) {
				if (name[i] >= 'A' && name[i] <= 'Z') {
					continue;
				}
				if (name[i] >= 'a' && name[i] <= 'z') {
					continue;
				}
				if (name[i] >= '0' && name[i] <= '9') {

					if (i == 0) {
						errorMsg = "Template cannot start with a number!";
						return false;
					}
					continue;
				}
				if (name[i] == '_') {
					continue;
				}

				errorMsg = "Invalid character '";
				errorMsg += name[i];
				errorMsg += "'!";
				return false;
			}
			return true;
		}

		void OnRender() override final {
			if (!m_templateWizardLaunched) {
				return;
			}

			ImGuiWindowFlags windowConfig = 0;
			windowConfig |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize;

			if (ImGui::Begin("##Template Wizard", &m_templateWizardLaunched, windowConfig)) {

				bool createRequested = false;

				ImGui::PushFont(Editor::m_fonts[NunitoSans][36]);
				Editor::AddCenteredText("Template Wizard");
				ImGui::PopFont();
				ImGui::PushFont(Editor::m_fonts[NunitoSans][24]);
				Editor::AddCenteredText("-- Create New File -- ");
				ImGui::NewLine();

				ImGui::Indent(80);

				const char* items[] = { "Game Object", "Layer", "Scene" };
				const char* preview = m_template_selection_index < 0 ? "Template Options" : items[m_template_selection_index];

				if (ImGui::BeginCombo("##TemplateOptions", preview))
				{

					for (int n = 0; n < IM_ARRAYSIZE(items); n++)
					{
						const bool is_selected = (m_template_selection_index == n);
						if (ImGui::Selectable(items[n], is_selected)) {
							m_template_selection_index = n;
							strcpy_s(m_template_name, 60, (std::string("Name of ") + items[n]).c_str());
							m_template_name_edited = false;
						}

						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}
				ImGui::NewLine();
				ImGuiInputTextFlags textFlags;
				textFlags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackEdit;

				if (m_template_selection_index < 0) {
					textFlags |= ImGuiInputTextFlags_ReadOnly;
					Editor::InactiveTextBoxBegin();
				}

				if (ImGui::InputText("##Filename", m_template_name, 60, textFlags, Editor::MyTextCallback, (void*)&m_template_name_edited)) {
					createRequested = true;
				}

				if (m_template_selection_index < 0) {
					Editor::InactiveTextBoxEnd();
				}

				bool validName = false;
				if (m_template_name_edited) {
					std::string errorMsg;
					validName = ValidTemplateName(m_template_name, errorMsg);

					if (!validName) {
						ImGui::NewLine();
						ImGui::PushStyleColor(ImGuiCol_Text, { 0.9f, 0.2f, 0.2f, 1.0f });
						ImGui::Text("* %s", errorMsg.c_str());
						ImGui::PopStyleColor();
					}
				}

				ImGui::Unindent(80);
				ImGui::Indent(195);

				ImGui::SetCursorPosY(500);

				if (!validName) { Editor::InactiveButtonBegin(); }
				if (ImGui::Button("Create File") && validName) {
					createRequested = true;
				}
				if (!validName) { Editor::InactiveButtonEnd(); }

				ImGui::Unindent(160);
				ImGui::PopFont();

				if (validName && createRequested) {
					CreateFileFromTemplate((TemplateType)m_template_selection_index, m_template_name);
					m_templateWizardLaunched = false;
				}
			}

			ImGui::End();
		}
	};
	// =========================================================
	struct EntityWizardImpl : public Panel::Impl {

		bool m_entityWizardLaunched;
		char m_entity_name[60];
		bool m_template_name_edited;

		void OnRender() override final {
			if (!m_entityWizardLaunched) {
				return;
			}

			ImGuiWindowFlags windowConfig = 0;
			windowConfig |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize;

			if (ImGui::Begin("##Entity Wizard", &m_entityWizardLaunched, windowConfig)) {
				ImGui::PushFont(Editor::m_fonts[NunitoSans][36]);
				Editor::AddCenteredText("Entity Wizard");
				ImGui::PopFont();
				ImGui::PushFont(Editor::m_fonts[NunitoSans][24]);
				Editor::AddCenteredText("-- Create Entity Definition -- ");
				ImGui::PopFont();

				ImGui::NewLine();
				ImGuiInputTextFlags textFlags;
				textFlags = ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackEdit;

				if (ImGui::InputText("##Filename", m_entity_name, 60, textFlags, Editor::MyTextCallback, (void*)&m_template_name_edited)) {
				}
			}

			ImGui::End();
		}
	};
	// =========================================================

	Panel::Panel(PanelType type)
	{
		m_impl = nullptr;
		SetPanelType(type);
	}

	Panel::~Panel()
	{
		if (m_impl != nullptr) {
			delete m_impl;
		}
	}

	void Panel::SetPanelType(PanelType type)
	{
		if (m_impl != nullptr) {
			delete m_impl;
		}

		m_type = type;

		switch (type)
		{
		case PanelType::PT_NAP:
			m_impl = nullptr;
			break;
		case PanelType::MenuBar:
			m_impl = new MenuBarImpl;
			break;
		case PanelType::DiagnosticsPanel:
			m_impl = new DiagnosticsPanelImpl;
			break;
		case PanelType::ScenePanel:
			m_impl = new ScenePanelImpl;
			break;
		case PanelType::ConsolePanel:
			m_impl = new ConsolePanelImpl;
			break;
		case PanelType::GameWindowSettingsPanel:
			m_impl = new GameWindowSettingsPanelImpl;
			break;
		case PanelType::GameWindowPanel:
			m_impl = new GameWindowPanelImpl;
			break;
		case PanelType::TemplateWizard:
			m_impl = new TemplateWizardImpl;
			break;
		case PanelType::EntityWizard:
			m_impl = new EntityWizardImpl;
			break;
		default:
			RDT_CORE_ERROR("Unknown popup window type!");
			m_impl = new Panel::Impl;
			break;
		}

		m_impl->ApplyConfigPreferences(m_config);
	}

	void Panel::Render()
	{
		Editor::ApplyGuiConfig(m_config);
		m_impl->OnRender();
	}

	void* Panel::GetData()
	{
		return m_impl->OnGetData();
	}

	PanelConfig& Panel::GetConfig()
	{
		return m_config;
	}

	PanelType Panel::GetType()
	{
		return m_type;
	}

	// ==============================================================================
	struct Container::Impl {
		std::vector<Panel*> m_panels;

		Impl()
		{
		}

		~Impl()
		{
		}

		virtual void OnRenderContainer() {}

		int FindPanel(PanelType type) {
			int index = 0;
			for (auto panel : m_panels) {
				if (panel->GetType() == type) {
					return index;
				}
				index++;
			}

			return -1;
		}

		void OnAddPanel(Panel* nPanel) {
			if (FindPanel(nPanel->GetType()) >= 0) {
				RDT_CORE_ERROR("Container::Impl - Tried to add duplicate panel: {}", nPanel->GetType());
				return;
			}

			m_panels.push_back(nPanel);
		}

		void OnRemovePanel(PanelType type) {
			int index = FindPanel(type);

			if (index < 0) {
				RDT_CORE_ERROR("Container::Impl - Tried to remove panel that isn't in this container: {}", type);
				return;
			}

			m_panels.erase(m_panels.begin() + index);
		}

		const static ImGuiWindowFlags ContainerWindowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoScrollbar
			| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;
	};
	// ==============================================================================
	struct FillContainerImpl : public Container::Impl {
		std::string name = "##" + std::string(typeid(this).name());

		void OnRenderContainer() override final {

			ImGui::Begin(name.c_str(), (bool*)0, ContainerWindowFlags);
			ImGui::End();
		}
	};
	// ==============================================================================
	struct DockLeftContainerImpl : public Container::Impl {
		std::string name = "##" + std::string(typeid(this).name());

		void OnRenderContainer() override final {

			ImGui::Begin(name.c_str(), (bool*)0, ContainerWindowFlags);
			ImGui::End();
		}
	};
	// ==============================================================================
	struct DockTopContainerImpl : public Container::Impl {
		std::string name = "##" + std::string(typeid(this).name());

		void OnRenderContainer() override final {

			ImGui::Begin(name.c_str(), (bool*)0, ContainerWindowFlags);
			ImGui::End();
		}
	};
	// ==============================================================================
	struct DockBottomContainerImpl : public Container::Impl {
		std::string name = "##" + std::string(typeid(this).name());

		void OnRenderContainer() override final {

			ImGui::Begin(name.c_str(), (bool*)0, ContainerWindowFlags);
			ImGui::End();
		}
	};
	// ==============================================================================
	struct DockRightContainerImpl : public Container::Impl {
		std::string name = "##" + std::string(typeid(this).name());

		void OnRenderContainer() override final {

			ImGui::Begin(name.c_str(), (bool*)0, ContainerWindowFlags);
			ImGui::End();
		}
	};
	// ==============================================================================

	Container::Container(ContainerType type)
	{
		m_impl = nullptr;
		SetContainerType(type);
	}

	Container::~Container()
	{
		if (m_impl != nullptr) {
			delete m_impl;
		}
	}
	
	void Container::SetContainerType(ContainerType type)
	{
		if (m_impl != nullptr) {
			delete m_impl;
		}

		m_type = type;

		switch (type) {
		case ContainerType::DockFill:
			m_impl = new FillContainerImpl;
			break;
		case ContainerType::DockLeft:
			m_impl = new DockLeftContainerImpl;
			break;
		case ContainerType::DockTop:
			m_impl = new DockTopContainerImpl;
			break;
		case ContainerType::DockBottom:
			m_impl = new DockBottomContainerImpl;
			break;
		case ContainerType::DockRight:
			m_impl = new DockRightContainerImpl;
			break;
		default:
			m_impl = new Container::Impl;
			break;
		}
	}

	ContainerType Container::GetType()
	{
		return m_type;
	}

	void Container::RenderContainer()
	{
		Editor::ApplyGuiConfig(m_config);
		m_impl->OnRenderContainer();
	}

	void Container::AddToContainer(Panel* nPanel)
	{
		m_impl->OnAddPanel(nPanel);
	}

	void Container::RemoveFromContainer(PanelType type)
	{
		m_impl->OnRemovePanel(type);
	}

	PanelConfig& Container::GetConfig()
	{
		return m_config;
	}

	// ==============================================================================

	/*
		Docking Layout Defualts
	*/

	namespace LayoutDefaults {
		constexpr float BorderTopHeight = 15.0f;
	}

	PanelManager::PanelManager()
	{
		m_workspaces[CommonWorkspaces::Default];
		m_workspaces[CommonWorkspaces::Fullscreen];
		SetCurrentWorkspace(CommonWorkspaces::Default);

		RegisterContainer(ContainerType::DockFill);
		auto& fillConfig = GetHead()->container.GetConfig();
		fillConfig.xPos = 0;
		fillConfig.yPos = 0;
		fillConfig.width = Renderer::GetWindowWidth();
		fillConfig.height = Renderer::GetWindowHeight();
	}

	PanelManager::~PanelManager()
	{
		for (auto& [type, panel] : m_panels) {
			delete panel;
		}

		for (auto& [id, workspace] : m_workspaces) {
			for (auto& [type, container] : workspace) {
				delete container;
			}
		}
	}

	void PanelManager::RegisterPanel(PanelType type)
	{
		m_panels[type] = new Panel(type);
	}

	void PanelManager::AssignToContainer(PanelType panel, ContainerType container)
	{
		if (m_panels.find(panel) == m_panels.end()) {
			RDT_CORE_ERROR("PanelManager - Tried to place unregistered panel");
			return;
		}

		// Remove panel from its presently bounded container
		if (m_panel_locations.at(panel) != ContainerType::NotActive) {
			RemoveFromContainer(panel);
		}

		// Update container tree if container node doesn't exist
		if (!ContainerExists(container)) {
			RegisterContainer(container);
		}

		// Add panel to container
		GetCurrentWorkspace().at(container)->container.AddToContainer(m_panels.at(panel));
		m_panel_locations.at(panel) = container;
	}

	void PanelManager::RemoveFromContainer(PanelType panel)
	{
		GetCurrentWorkspace().at(m_panel_locations.at(panel))->container.RemoveFromContainer(panel);
		m_panel_locations.at(panel) = ContainerType::NotActive;
	}

	void PanelManager::SetCurrentWorkspace(int workspaceID)
	{
		if (m_workspaces.find(workspaceID) == m_workspaces.end()) {
			RDT_CORE_ERROR("PanelManager - Could not find workspace with ID: {}", workspaceID);
			return;
		}

		m_workspace_id = workspaceID;
	}

	Workspace& PanelManager::GetCurrentWorkspace()
	{
		return m_workspaces.at(m_workspace_id);
	}

	void PanelManager::RegisterContainer(ContainerType type)
	{
		auto& workspace = GetCurrentWorkspace();
		if (ContainerExists(type)) {
			RDT_CORE_ERROR("PanelManager - Tried to register duplicate container type: {}", type);
			return;
		}
		
		workspace[type] = new ContainerNode;
		workspace.at(type)->SetContainerType(type);

		if (type == ContainerType::DockFill) {
			return;
		}

		auto& nConfig = GetContainerConfig(type);
		if (type == ContainerType::DockLeft) {
			nConfig.xPos = 0;
			nConfig.yPos = 0;
			nConfig.width = Renderer::GetWindowWidth() * 0.225f;
			nConfig.height = Renderer::GetWindowHeight();
			float dw = nConfig.width + PanelMargin;

			if (ContainerExists(ContainerType::DockFill)) {
				auto& oConfig = GetContainerConfig(ContainerType::DockFill);
				oConfig.xPos += dw;
				oConfig.width -= dw;
				oConfig.update = true;
			}

			if (ContainerExists(ContainerType::DockTop)) {
				auto& oConfig = GetContainerConfig(ContainerType::DockTop);
				oConfig.xPos += dw;
				oConfig.width -= dw;
				oConfig.update = true;
			}

			if (ContainerExists(ContainerType::DockBottom)) {
				auto& oConfig = GetContainerConfig(ContainerType::DockBottom);
				oConfig.xPos += dw;
				oConfig.width -= dw;
				oConfig.update = true;
			}

			return;
		}

		if (type == ContainerType::DockTop) {

			nConfig.xPos = 0;
			nConfig.yPos = 0;

			nConfig.height = Renderer::GetWindowHeight() * 0.10f;
			float dh = nConfig.height + PanelMargin;
			
			if (ContainerExists(ContainerType::DockFill)) {
				auto& oConfig = GetContainerConfig(ContainerType::DockFill);
				
				nConfig.width = oConfig.width;
				nConfig.xPos = oConfig.xPos;

				oConfig.yPos += dh;
				oConfig.height -= dh;
			}

			return;
		}

		if (type == ContainerType::DockBottom) {

			nConfig.xPos = 0;
			nConfig.yPos = 0;

			nConfig.height = Renderer::GetWindowHeight() * 0.25f;
			float dh = nConfig.height + PanelMargin;

			if (ContainerExists(ContainerType::DockFill)) {
				auto& oConfig = GetContainerConfig(ContainerType::DockFill);
				
				nConfig.width = oConfig.width;
				nConfig.xPos = oConfig.xPos;

				oConfig.height -= dh;
				nConfig.yPos = oConfig.yPos + oConfig.height + PanelMargin;
			}

			return;
		}

		if (type == ContainerType::DockRight) {
			nConfig.xPos = 0;
			nConfig.yPos = 0;
			nConfig.width = Renderer::GetWindowWidth() * 0.225f;
			nConfig.height = Renderer::GetWindowHeight();

			float dw = nConfig.width + PanelMargin;

			if (ContainerExists(ContainerType::DockFill)) {
				auto& oConfig = GetContainerConfig(ContainerType::DockFill);

				oConfig.width -= dw;
				nConfig.xPos = oConfig.xPos + oConfig.width + PanelMargin;
			}

			if (ContainerExists(ContainerType::DockTop)) {
				auto& oConfig = GetContainerConfig(ContainerType::DockTop);
				oConfig.width -= dw;
			}

			if (ContainerExists(ContainerType::DockBottom)) {
				auto& oConfig = GetContainerConfig(ContainerType::DockBottom);
				oConfig.width -= dw;
			}
		}
	}

	const std::unordered_map<PanelType, void*>& PanelManager::GetMessages()
	{
		for (auto& [type, panel] : m_panels) {
			m_messages[type] = panel->GetData();
		}

		return m_messages;
	}

	bool PanelManager::ContainerExists(ContainerType type)
	{
		return GetCurrentWorkspace().find(type) != GetCurrentWorkspace().end();
	}

	PanelConfig& PanelManager::GetContainerConfig(ContainerType type)
	{
		return GetCurrentWorkspace().at(type)->container.GetConfig();
	}

	ContainerNode* PanelManager::GetHead()
	{
		return GetCurrentWorkspace().at(DockFill);
	}

	ContainerType PanelManager::GetNextExpected(ContainerType current, ContainerType target, TreeDirection& next)
	{
		switch (current) {
		case ContainerType::DockFill:
			switch (target) {
			case ContainerType::DockLeft:
				next = TreeDirection::TD_LEFT;
				return ContainerType::DockLeft;
			case ContainerType::DockTop:
				next = TreeDirection::TD_UP;
				return ContainerType::DockTop;
			case ContainerType::DockRight:
				next = TreeDirection::TD_RIGHT;
				return ContainerType::DockRight;
			case ContainerType::DockBottom:
				next = TreeDirection::TD_DOWN;
				return ContainerType::DockBottom;
			default:
				next = TreeDirection::TD_NOMOVEMENT;
				return ContainerType::NotActive;
			}
			break;
		default:
			next = TreeDirection::TD_NOMOVEMENT;
			break;
		}

		return ContainerType::NotActive;
	}

	void PanelManager::RenderMDI()
	{
		auto& workspace = GetCurrentWorkspace();

		for (auto& [type, panel] : m_panels) {
			panel->Render();
		}
	}

	// ==============================================================================
	/*
		Editor Gui Implementation
	*/

	// Static Variables
	std::string Editor::sourcePath = "";
	std::string Editor::templatePath = "";
	Scene* Editor::m_scene = nullptr;
	std::unordered_map<EditorFont, std::unordered_map<unsigned int, ImFont*>> Editor::m_fonts;
	GameRenderWindow* Editor::m_gameWindow = nullptr;
	int Editor::m_gameWindowId = -1;

	/*
		Panel Layout Macros
	*/

	

	Editor::Editor()
		: m_config(nullptr)
	{
		m_showTools = true;
		
		RegisterToMessageBus("EditorLayout");
		SetTheme(Theme_Codz);

		ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		GuiManager::EnableDockOverViewport();

		m_gameWindowId = Renderer::AddRenderWindow(m_gameWindow = new GameRenderWindow);
		Renderer::SetDefaultViewport(false);
		Input::SetTargetRenderWindow(m_gameWindowId);
		m_gameWindow->SetGuiPositionY(88);
		m_gameWindow->SetGuiPositionX((m_window_width / 2) - (m_gameWindow->GetGuiDimensions().x / 2));


		m_panel_manager.RegisterContainer(ContainerType::DockTop);
		m_panel_manager.RegisterContainer(ContainerType::DockLeft);
		m_panel_manager.RegisterContainer(ContainerType::DockBottom);
		m_panel_manager.RegisterContainer(ContainerType::DockRight);
		
		m_panel_manager.RegisterPanel(MenuBar);
		m_panel_manager.RegisterPanel(DiagnosticsPanel);
		m_panel_manager.RegisterPanel(ScenePanel);
		m_panel_manager.RegisterPanel(GameWindowPanel);
		m_panel_manager.RegisterPanel(ConsolePanel);

		/*m_panel_manager.RegisterPanel(MenuBar);
		m_panel_manager.RegisterPanel(DiagnosticsPanel);
		m_panel_manager.RegisterPanel(ScenePanel);
		m_panel_manager.RegisterPanel(ConsolePanel);
		m_panel_manager.RegisterPanel(GameWindowPanel);
		m_panel_manager.RegisterPanel(GameWindowSettingsPanel);

		m_panel_manager.AssignToContainer(MenuBar, ContainerType::DockBorderTop);
		m_panel_manager.AssignToContainer(DiagnosticsPanel, ContainerType::DockRight);
		m_panel_manager.AssignToContainer(ScenePanel, ContainerType::DockRight);
		m_panel_manager.AssignToContainer(ConsolePanel, ContainerType::DockBottom);
		m_panel_manager.AssignToContainer(GameWindowPanel, ContainerType::DockFill);
		m_panel_manager.AssignToContainer(GameWindowSettingsPanel, ContainerType::DockTop);*/
	}

	Editor::~Editor()
	{
	}
	void Editor::OnMessage(Message msg)
	{
	}

	void Editor::OnUpdate(const float deltaTime)
	{
		auto broadcast = MessageBus::GetBroadcast("SceneManager");
		if (broadcast == nullptr) {
			return;
		}

		for (auto it = begin(*broadcast); it != end(*broadcast); ++it) {
			switch (it->type) {
			case MT_SceneChanged:
				SetScenePtr(((SceneChangedData*)it->data)->ptr);
				break;
			}
		}

		if (Input::CheckInput(controls_ShowTools1) && Input::CheckInput(controls_ShowTools2)) {
			if (Renderer::UsingDefaultViewport()) {
				m_showTools = !m_showTools;
			}
		}
	}

	void Editor::OnRender()
	{
		if (!m_showTools) {
			return;
		}

		m_panel_manager.RenderMDI();

		bool isFullscreen = Renderer::UsingDefaultViewport();
	}

	void Editor::SetTheme(EditorTheme nTheme)
	{
		ImGuiStyle& style = ImGui::GetStyle();
		switch (nTheme) {
		case Theme_Codz:
			// From: https://github.com/ocornut/imgui/issues/707
			style.WindowRounding = 5.3f;
			style.FrameRounding = 2.3f;
			style.ScrollbarRounding = 0;

			style.Colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 0.95f);
			style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);
			style.Colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.10f, 0.85f);
			style.Colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 0.65f);
			style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			style.Colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.01f, 1.00f);
			style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
			style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
			style.Colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.83f);
			style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.40f, 0.40f, 0.80f, 0.20f);
			style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.87f);
			style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.01f, 0.01f, 0.02f, 0.80f);
			style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
			style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
			style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
			style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
			style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
			style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
			style.Colors[ImGuiCol_Button] = ImVec4(0.48f, 0.72f, 0.89f, 0.49f);
			style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.69f, 0.99f, 0.68f);
			style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
			style.Colors[ImGuiCol_Header] = ImVec4(0.30f, 0.69f, 1.00f, 0.53f);
			style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.44f, 0.61f, 0.86f, 1.00f);
			style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.38f, 0.62f, 0.83f, 1.00f);
			style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
			style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
			style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
			style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);

			Log::SetLogColor(LogLevel::L_TRACE, WHITE);

			break;

		case Nightingale:
			style.WindowRounding = 5.3f;
			style.FrameRounding = 2.3f;
			style.ScrollbarRounding = 0;

			ImVec4 black = ImVec4(0.1f, 0.1f, 0.1f, 0.95f);
			ImVec4 white = ImVec4(0.95f, 0.95f, 0.95f, 1.0f);
			ImVec4 header(0.153f, 0.157f, 0.161f, 1.0f);
			ImVec4 base(0.38f, 0.404f, 0.478f, 0.95f);
			ImVec4 border(0.847f, 0.851f, 0.855f, 1.0f);

			style.Colors[ImGuiCol_Text] = white;
			style.Colors[ImGuiCol_TextDisabled] = white;
			style.Colors[ImGuiCol_WindowBg] = base;
			style.Colors[ImGuiCol_PopupBg] = base;
			style.Colors[ImGuiCol_Border] = border;
			style.Colors[ImGuiCol_BorderShadow] = border;
			style.Colors[ImGuiCol_FrameBg] = header;
			style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.90f, 0.80f, 0.80f, 0.40f);
			style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.90f, 0.65f, 0.65f, 0.45f);
			style.Colors[ImGuiCol_TitleBg] = header;
			style.Colors[ImGuiCol_TitleBgCollapsed] = header;
			style.Colors[ImGuiCol_TitleBgActive] = header;
			style.Colors[ImGuiCol_MenuBarBg] = header;
			style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.25f, 0.30f, 0.60f);
			style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.55f, 0.53f, 0.55f, 0.51f);
			style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.56f, 1.00f);
			style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.91f);
			style.Colors[ImGuiCol_CheckMark] = ImVec4(0.90f, 0.90f, 0.90f, 0.83f);
			style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.70f, 0.70f, 0.70f, 0.62f);
			style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 0.84f);
			style.Colors[ImGuiCol_Button] = ImVec4(0.48f, 0.72f, 0.89f, 0.49f);
			style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.69f, 0.99f, 0.68f);
			style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.80f, 0.50f, 0.50f, 1.00f);
			style.Colors[ImGuiCol_Header] = header;
			style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.44f, 0.61f, 0.86f, 1.00f);
			style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.38f, 0.62f, 0.83f, 1.00f);
			style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
			style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
			style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
			style.Colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
			style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.00f, 1.00f, 0.35f);

			Log::SetLogColor(LogLevel::L_TRACE, WHITE);
			break;
		}

		if (m_config != nullptr) {
			m_config->SetAttribute("DevTools", "Theme", nTheme);
		}
	}

	void Editor::SetSourcePath(const std::string& path)
	{
		if (Utils::PathExists(path)) {
			sourcePath = path;
		}
		else {
			RDT_CORE_WARN("EditorLayout - Source path does not exist: {}", path);
		}
	}

	void Editor::InitResources(std::string& resourcePath)
	{
		fs::path fontFolder = fs::path(resourcePath) / fs::path("fonts");
		std::string ttfFile;
		
		// Set the default font
		ttfFile = (fontFolder / fs::path("NunitoSans_7pt_Condensed-Medium.ttf")).generic_string();
		GuiManager::LoadFont(NunitoSans, ttfFile);
		GuiManager::SetDefaultFont(NunitoSans, 18);

		AddFont(NunitoSans, ttfFile, std::vector<unsigned int>{18, 24, 36});

		// Load Icons from ForkAwesome
		ttfFile = (fontFolder / fs::path(FONT_ICON_FILE_NAME_FK)).generic_string();
		static const ImWchar icons_ranges[] = { ICON_MIN_FK, ICON_MAX_16_FK, 0 };

		GuiManager::LoadIcons(ForkAwesome, ttfFile, icons_ranges);
		m_fonts[ForkAwesome][36] = GuiManager::GetFont(ForkAwesome, 36);
		m_fonts[ForkAwesome][13] = GuiManager::GetFont(ForkAwesome, 13);
		m_fonts[ForkAwesome][18] = GuiManager::GetFont(ForkAwesome, 18);

		fs::path templateFolder = fs::path(resourcePath) / fs::path("templates");
		if (!fs::is_directory(templateFolder) || !fs::exists(templateFolder)) { 
			RDT_CORE_WARN("Could not find templates at '{}'", templateFolder.generic_string());
			return;
		}
		templatePath = templateFolder.generic_string();

	}

	void Editor::AddConfigPtr(ConfigReader* ptr)
	{
		m_config = ptr;
	}

	void Editor::ProcessMessages()
	{
		for (auto& [panelType, message] : m_panel_manager.GetMessages()) {

			if (message == nullptr) {
				continue;
			}

			switch (panelType) {
			case MenuBar:
				OnMenuBarMessage((MenuBarData*)message);
				break;
			}
		}
	}

	void Editor::OnMenuBarMessage(MenuBarData* data)
	{
		if (data->changeThemeRequest != ET_NAT) {
			SetTheme(data->changeThemeRequest);
		}

		if (data->openPanelRequest != PT_NAP) {
			OpenPanel(data->openPanelRequest);
		}
	}

	void Editor::OpenPanel(PanelType panel)
	{
	}

	void Editor::ApplyConfig()
	{
		if (m_config == nullptr) {
			return;
		}

		std::string config_theme;
		if (m_config->GetAttribute("DevTools", "Theme", config_theme)) {
			int themeVal = std::stoi(config_theme);

			if (themeVal >= EditorTheme::Theme_Codz && themeVal <= EditorTheme::Nightingale) {
				SetTheme((EditorTheme)themeVal);
			}
			else {
				RDT_CORE_WARN("Could not find theme associated with id: {}", themeVal);
			}
		}
	}

	void Editor::OnFirstRender()
	{
		/*ImGui::PushFont(m_fonts[ForkAwesome][18]);
		m_game_window_settings_panel.width = GameWindowSettingsPanelWidth;
		m_game_window_settings_panel.height = GetButtonHeight(ICON_FK_PAUSE) + 20;
		m_game_window_settings_panel.xPos = ((float)m_game_window_panel->GetLastPosition().x) + m_game_window_panel->GetGuiDimensions().x - m_game_window_settings_panel.width;
		m_game_window_settings_panel.yPos = GetDockPosY(DockTop, m_game_window_settings_panel.height, PanelMargin) + m_menu_bar_height;
		ImGui::PopFont();

		m_game_window_panel->SetGuiPositionY(m_game_window_settings_panel.yPos + m_game_window_settings_panel.height + PanelMargin);
		m_game_window_panel->TriggerUpdatePos();

		m_diagnostics_panel.width = m_window_width - (m_game_window_settings_panel.xPos + m_game_window_settings_panel.width + (PanelMargin * 2));
		m_diagnostics_panel.height = DiagnosticGuiHeight;
		m_diagnostics_panel.xPos = GetDockPosX(DockRight, m_diagnostics_panel.width, PanelMargin);
		m_diagnostics_panel.yPos = GetDockPosY(DockTop, m_diagnostics_panel.height, PanelMargin) + m_menu_bar_height;

		m_scene_panel.width = m_diagnostics_panel.width;
		m_scene_panel.height = m_window_height - (m_diagnostics_panel.yPos + m_diagnostics_panel.height + (PanelMargin * 2));
		m_scene_panel.xPos = GetDockPosX(DockRight, m_scene_panel.width, PanelMargin);
		m_scene_panel.yPos = m_diagnostics_panel.yPos + m_diagnostics_panel.height + PanelMargin;

		m_template_wizard.width = TemplateWizardGuiWidth;
		m_template_wizard.height = TemplateWizardGuiHeight;
		m_template_wizard.xPos = (m_window_width / 2) - (m_template_wizard.width / 2);
		m_template_wizard.yPos = (m_window_height / 2) - (m_template_wizard.height / 2);

		m_entity_wizard = m_template_wizard;

		m_console_panel.width = m_game_window_panel->GetGuiDimensions().x;
		m_console_panel.height = m_window_height -  (m_game_window_panel->GetLastPosition().y + m_game_window_panel->GetGuiDimensions().y + (PanelMargin * 2));
		m_console_panel.xPos = (float)m_game_window_panel->GetLastPosition().x;
		m_console_panel.yPos = ((float)m_game_window_panel->GetLastPosition().y) + m_game_window_panel->GetGuiDimensions().y + PanelMargin;*/
	}

	void Editor::AddFont(EditorFont name, std::string& ttfFile, const std::vector<unsigned int>& sizes)
	{
		if (Utils::PathExists(ttfFile)) {
			
			if (!GuiManager::FontExists(name)) {
				GuiManager::LoadFont(name, ttfFile);
			}

			for (auto size : sizes) {
				m_fonts[name][size] = GuiManager::GetFont(name, size);
			}
		}
		else {
			RDT_CORE_WARN("Could not find file '{}'", ttfFile);
			return;
		}
	}

	void Editor::SetScenePtr(Scene* ptr)
	{
		m_scene = ptr;
	}

	void Editor::AddCenteredText(const std::string& text)
	{
		ImVec2 size = ImGui::CalcTextSize(text.c_str());

		ImVec2 pos = { (ImGui::GetWindowSize().x / 2) - (size.x / 2), ImGui::GetCursorPosY() };
		ImGui::SetCursorPos(pos);
		ImGui::Text(text.c_str());
	}

	void Editor::InactiveButtonBegin()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4 color;

		color = style.Colors[ImGuiCol_Button];
		color.w = 0.1f;
		ImGui::PushStyleColor(ImGuiCol_Button, color);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, color);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, color);

		color = style.Colors[ImGuiCol_Text];
		color.w = 0.1f;
		ImGui::PushStyleColor(ImGuiCol_Text, color);
	}

	void Editor::InactiveButtonEnd()
	{
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
	}

	void Editor::InactiveTextBoxBegin()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4 color;

		color = style.Colors[ImGuiCol_FrameBg];
		color.w = 0.3f;
		ImGui::PushStyleColor(ImGuiCol_FrameBg, color);
		ImGui::BeginDisabled();
	}

	void Editor::InactiveTextBoxEnd()
	{
		ImGui::PopStyleColor();
		ImGui::EndDisabled();
	}

	float Editor::GetButtonWidth(const char* label)
	{
		ImGuiStyle& style = ImGui::GetStyle();
		return ImGui::CalcTextSize(label).x + (style.FramePadding.x * 2);
	}

	float Editor::GetButtonHeight(const char* label)
	{
		ImGuiStyle& style = ImGui::GetStyle();
		return ImGui::CalcTextSize(label).y + (style.FramePadding.y * 2);
	}

	int Editor::MyTextCallback(ImGuiInputTextCallbackData* data)
	{
		if (data->EventFlag == ImGuiInputTextFlags_CallbackEdit)
		{
			bool* p_bool = (bool*)data->UserData;
			*p_bool = true;
		}

		return 0;
	}
	
	void Editor::ApplyGuiConfig(PanelConfig& config)
	{
		if (config.update) {
			ImGui::SetNextWindowSize(ImVec2(config.width, config.height), ImGuiCond_Always);
			ImGui::SetNextWindowPos(ImVec2(config.xPos, config.yPos), ImGuiCond_Always);
			config.update = false;
		}
		else {
			//ImGui::SetNextWindowSize(ImVec2(config.width, config.height), ImGuiCond_Appearing);
			//ImGui::SetNextWindowPos(ImVec2(config.xPos, config.yPos), ImGuiCond_Appearing);
		}
	}
}
