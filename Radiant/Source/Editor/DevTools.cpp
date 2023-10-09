#include "pch.h"
#include "DevTools.h"
#include "IconsForkAwesome.h"

#include "Messaging/MessageTypes.h"
#include "Graphics/Renderer.h"
#include "Utils/Utils.h"

#include "Gui/GuiManager.h"

namespace fs = std::filesystem;

namespace rdt::core {

	DevLayer* DevLayer::m_instance = nullptr;

	DevLayer::DevLayer()
	{
		RDT_CORE_WARN("Developer tools are enabled");

		RegisterToMessageBus("DevLayer");

		EditorLayout* layout;
		RegisterGUI(layout = new EditorLayout);
		layout->SetTheme(Theme_Codz);

		m_showTools = true;

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


		Renderer::SetBackgroundColor(Color(0.1f, 0.1f, 0.1f, 1.0f));

		// Set world camera to a shrunked window
		Camera* defaultCamera = Renderer::GetCamera();
		int viewportWidth = 1280;
		int viewportHeight = 720;
		int cameraPosX = (((int)Renderer::GetWindowWidth()) / 2) - (viewportWidth / 2);
		int cameraPosY = (((int)Renderer::GetWindowHeight())) - (viewportHeight)- 120;
		defaultCamera->SetViewport({ cameraPosX, cameraPosY }, { viewportWidth, viewportHeight });
	}
	DevLayer::~DevLayer()
	{
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
		if (m_showTools) {
			for (auto& gui : GetGUIs()) {
				Renderer::AttachGui(gui);
			}
		}
	}

	void DevLayer::OnDetach()
	{
		if (m_showTools) {
			for (auto& gui : GetGUIs()) {
				Renderer::DetachGui(gui);
			}
		}
	}
	void DevLayer::OnMessage(Message msg)
	{
	
	}
	void DevLayer::OnProcessInput(const float deltaTime)
	{
		if (Input::CheckKeyboardState(controls_ShowTools1) && Input::CheckKeyboardState(controls_ShowTools2)) {
			
			if (m_showTools) {
				for (auto& gui : GetGUIs()) {
					Renderer::DetachGui(gui);
				}
			}
			else {
				for (auto& gui : GetGUIs()) {
					Renderer::AttachGui(gui);
				}
			}
			m_showTools = !m_showTools;
		}

		Layer::OnProcessInput(deltaTime);
	}

	void DevLayer::OnRender()
	{
		if (!m_showTools) {
			return;
		}

		Layer::OnRender();
	}

	// ==============================================================================

	/*
		Editor Gui Implementation
	*/

	/*
		Panel Layout Macros
	*/
	constexpr int PanelMargin = 10;

	constexpr float DiagnosticGuiWidth = 300.0f;
	constexpr float DiagnosticGuiHeight = 105.0f;

	constexpr float ScenePanelGuiWidth = 300.0f;
	constexpr float ScenePanelGuiHeight = 400.0f;

	constexpr float TemplateWizardGuiWidth = 500.0f;
	constexpr float TemplateWizardGuiHeight = 575.0f;

	constexpr float GameWindowPanelWidth = 200.0f;

	EditorLayout::EditorLayout()
		: m_scene(nullptr), first_render(true), m_templateWizardLaunched(false)
	{
		RegisterToMessageBus("EditorLayout");
		SetTheme(Theme_Codz);
	}
	EditorLayout::~EditorLayout()
	{
	}
	void EditorLayout::OnMessage(Message msg)
	{
	}

	void EditorLayout::OnUpdate(const float deltaTime)
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
	}

	void EditorLayout::OnRender()
	{
		
		ImGui::PushFont(m_fonts[NunitoSans][18]);

		RenderMenuBar();
		RenderDiagnosticsPanel();
		RenderScenePanel();
		RenderGameWindowPanel();
		RenderTemplateWizard();

		ImGui::PopFont();
	}

	void EditorLayout::SetTheme(EditorTheme nTheme)
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
			break;
		}
	}

	void EditorLayout::SetSourcePath(const std::string& path)
	{
		if (Utils::PathExists(path)) {
			sourcePath = path;
		}
		else {
			RDT_CORE_WARN("EditorLayout - Source path does not exist: {}", path);
		}
	}

	void EditorLayout::InitResources(std::string& resourcePath)
	{
		fs::path fontFolder = fs::path(resourcePath) / fs::path("fonts");
		std::string ttfFile;
		
		ttfFile = (fontFolder / fs::path("NunitoSans_7pt_Condensed-Medium.ttf")).generic_string();
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

	void EditorLayout::OnFirstRender()
	{
		m_diagnostics_panel.width = DiagnosticGuiWidth;
		m_diagnostics_panel.height = DiagnosticGuiHeight;
		m_diagnostics_panel.xPos = GetDockPosX(DockRight, m_diagnostics_panel.width, PanelMargin);
		m_diagnostics_panel.yPos = GetDockPosY(DockTop, m_diagnostics_panel.height, PanelMargin) + m_menu_bar_height;

		m_scene_panel.width = ScenePanelGuiWidth;
		m_scene_panel.height = ScenePanelGuiHeight;
		m_scene_panel.xPos = GetDockPosX(DockRight, m_scene_panel.width, PanelMargin);
		m_scene_panel.yPos = m_diagnostics_panel.yPos + m_diagnostics_panel.height + PanelMargin;

		m_template_wizard.width = TemplateWizardGuiWidth;
		m_template_wizard.height = TemplateWizardGuiHeight;
		m_template_wizard.xPos = (m_window_width / 2) - (m_template_wizard.width / 2);
		m_template_wizard.yPos = (m_window_height / 2) - (m_template_wizard.height / 2);

		ImGui::PushFont(m_fonts[ForkAwesome][18]);
		m_game_window_panel.width = GameWindowPanelWidth;
		m_game_window_panel.height = GetButtonHeight(ICON_FK_PAUSE) + 20;
		m_game_window_panel.xPos = GetDockPosX(DockRight, m_game_window_panel.width + m_diagnostics_panel.width + PanelMargin, PanelMargin);
		m_game_window_panel.yPos = GetDockPosY(DockTop, m_game_window_panel.height, PanelMargin) + m_menu_bar_height;
		ImGui::PopFont();
	}

	void EditorLayout::AddFont(EditorFont name, std::string& ttfFile, const std::vector<unsigned int>& sizes)
	{
		if (Utils::PathExists(ttfFile)) {
			GuiManager::LoadFont(name, ttfFile);

			for (auto size : sizes) {
				m_fonts[name][size] = GuiManager::GetFont(name, size);
			}
		}
		else {
			RDT_CORE_WARN("Could not find file '{}'", ttfFile);
			return;
		}
	}

	void EditorLayout::ApplyGuiConfig(const GuiConfig& config)
	{
		// First render config
		ImGui::SetNextWindowSize(ImVec2(config.width, config.height), ImGuiCond_Appearing);
		ImGui::SetNextWindowPos(ImVec2(config.xPos, config.yPos), ImGuiCond_Appearing);
	}

	void EditorLayout::SetScenePtr(Scene* ptr)
	{
		m_scene = ptr;
	}

	void EditorLayout::AddCenteredText(const std::string& text)
	{
		ImVec2 size = ImGui::CalcTextSize(text.c_str());

		ImVec2 pos = { (ImGui::GetWindowSize().x / 2) - (size.x / 2), ImGui::GetCursorPosY() };
		ImGui::SetCursorPos(pos);
		ImGui::Text(text.c_str());
	}

	void EditorLayout::InactiveButtonBegin()
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

	void EditorLayout::InactiveButtonEnd()
	{
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
	}

	void EditorLayout::InactiveTextBoxBegin()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4 color;

		color = style.Colors[ImGuiCol_FrameBg];
		color.w = 0.3f;
		ImGui::PushStyleColor(ImGuiCol_FrameBg, color);
		ImGui::BeginDisabled();
	}

	void EditorLayout::InactiveTextBoxEnd()
	{
		ImGui::PopStyleColor();
		ImGui::EndDisabled();
	}

	void EditorLayout::CreateFileFromTemplate(TemplateType type, const std::string& name)
	{

		fs::path filepath = fs::path(sourcePath);
		fs::path template_h = fs::path(templatePath);
		fs::path template_cpp = fs::path(templatePath);


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

	float EditorLayout::GetButtonWidth(const char* label)
	{
		ImGuiStyle& style = ImGui::GetStyle();
		return ImGui::CalcTextSize(label).x + (style.FramePadding.x * 2);
	}

	float EditorLayout::GetButtonHeight(const char* label)
	{
		ImGuiStyle& style = ImGui::GetStyle();
		return ImGui::CalcTextSize(label).y + (style.FramePadding.y * 2);
	}

	bool EditorLayout::ValidTemplateName(const std::string& name, std::string& errorMsg)
	{
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

	int EditorLayout::MyTextCallback(ImGuiInputTextCallbackData* data)
	{
		if (data->EventFlag == ImGuiInputTextFlags_CallbackEdit)
		{
			bool* p_bool = (bool*)data->UserData;
			*p_bool = true;
		}

		return 0;
	}

	float EditorLayout::GetDockPosX(Dock docking, float guiWidth, float margin)
	{
		switch (docking) {
		case DockLeft:
			return margin;
		case DockRight:
			return (float)m_window_width - guiWidth - margin;
		case DockTop:
			return 0;
		case DockBottom:
			return 0;
		}

		return 0;
	}

	float EditorLayout::GetDockPosY(Dock docking, float guiHeight, float margin)
	{
		switch (docking) {
		case DockLeft:
			return 0;
		case DockRight:
			return 0;
		case DockTop:
			return margin;
		case DockBottom:
			return (float)m_window_height - guiHeight - margin;
		}

		return 0;
	}

	void EditorLayout::RenderMenuBar()
	{
		if (ImGui::BeginMainMenuBar()) {

			if (first_render) {
				m_menu_bar_height = ImGui::GetWindowSize().y;
				OnFirstRender();
				first_render = false;
			}

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
					if (!m_templateWizardLaunched) {
						m_template_selection_index = -1;
						m_templateWizardLaunched = true;
						strcpy_s(m_template_name, 60, "");
						m_template_name_edited = false;
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}

	void EditorLayout::RenderDiagnosticsPanel()
	{
		ApplyGuiConfig(m_diagnostics_panel);

		// Start drawing window
		ImGui::Begin("Diagnostic Tools");

		// Show framerate
		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::Text("Performance: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::End();
	}
	void EditorLayout::RenderScenePanel()
	{
		ApplyGuiConfig(m_scene_panel);

		// Start drawing window
		if (ImGui::Begin("Scene Panel")) {

			if (m_scene == nullptr) {
				ImGui::Text("No Scene Selected!");
			}
			else {
				ImGui::Text("Scene: %s", m_scene->GetName().c_str());

				size_t count;
				Layer** layers = m_scene->GetLayers(&count);
				for (size_t i = 0; i < count; i++) {
					AddLayerPanel(layers[i]);
				}
			}
		}

		ImGui::End();

	}
	void EditorLayout::AddLayerPanel(Layer* layer)
	{
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
	void EditorLayout::AddGameObjectPanel(GameObject* gobject)
	{
		std::string name = typeid(*gobject).name();
		std::string panel_header = name.substr(6, name.size() - 6) + ": " + gobject->GetName();

		ImGui::Indent(10);
		if (ImGui::CollapsingHeader(panel_header.c_str())) {

			ImGui::Text("This is info about this game object!");
		}
		ImGui::Unindent(10);
	}
	void EditorLayout::RenderGameWindowPanel()
	{
		ApplyGuiConfig(m_game_window_panel);

		ImGuiWindowFlags windowConfig = 0;
		windowConfig |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar;
		
		ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.7f, 0.7f, 0.7f, 0.9f });
		ImGui::Begin("##GameWindowPanel", (bool*)0, windowConfig);

		ImGui::PushFont(m_fonts[ForkAwesome][18]);
		ImGui::PushStyleColor(ImGuiCol_Button, { 1.0f, 1.0f, 1.0f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.8f, 0.8f, 0.8f, 1.0f });

		/* Play Button */
		ImGui::PushStyleColor(ImGuiCol_Text, { 0.2, 0.8, 0.2, 1.0f });
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
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - GetButtonWidth(ICON_FK_PAUSE) - 10);
		if (ImGui::Button(ICON_FK_EXPAND)) {

		}
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();


		ImGui::PopStyleColor();
		ImGui::PopStyleColor();

		ImGui::PopFont();
		
		ImGui::End();
		ImGui::PopStyleColor();
	}
	void EditorLayout::RenderTemplateWizard()
	{
		if (!m_templateWizardLaunched) {
			return;
		}

		ApplyGuiConfig(m_template_wizard);

		ImGuiWindowFlags windowConfig = 0;
		windowConfig |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize;


		if (ImGui::Begin("##Template Wizard", &m_templateWizardLaunched, windowConfig)) {
			
			bool createRequested = false;

			ImGui::PushFont(m_fonts[NunitoSans][36]);
			AddCenteredText("Template Wizard");
			ImGui::PopFont();
			ImGui::PushFont(m_fonts[NunitoSans][24]);
			AddCenteredText("-- Create New File -- ");
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
				InactiveTextBoxBegin();
			}
			
			if (ImGui::InputText("##Filename", m_template_name, 60, textFlags, MyTextCallback, (void*)&m_template_name_edited)) {
				createRequested = true;
			}

			if (m_template_selection_index < 0) {
				InactiveTextBoxEnd();
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

			if (!validName) { InactiveButtonBegin(); }
			if (ImGui::Button("Create File") && validName) {
				createRequested = true;
			}
			if (!validName) { InactiveButtonEnd(); }

			ImGui::Unindent(160);
			ImGui::PopFont();

			if (validName && createRequested) {
				CreateFileFromTemplate((TemplateType)m_template_selection_index, m_template_name);
				m_templateWizardLaunched = false;
			}
		}
	
		ImGui::End();
	}
}
