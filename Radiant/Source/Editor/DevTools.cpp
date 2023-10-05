#include "pch.h"
#include "DevTools.h"

#include "Messaging/MessageTypes.h"
#include "Graphics/Renderer.h"
#include "Utils/Utils.h"

#include "Gui/GuiManager.h"


namespace rdt::core {

	DevLayer* DevLayer::m_instance = nullptr;

	DevLayer::DevLayer()
	{
		RDT_CORE_WARN("Developer tools are enabled");

		RegisterToMessageBus("DevLayer");

		EditorLayout* layout;
		m_GUIs.push_back(layout = new EditorLayout);
		layout->SetTheme(Theme_Codz);

		m_showTools = true;

		m_base_directory = Utils::GetCWD();
		RDT_CORE_INFO("DevTools base directory: {}", m_base_directory);

		namespace fs = std::filesystem;
		fs::path configFile = fs::path(m_base_directory) / fs::path("radiant.ini");
		m_config.SetDefaultPath(configFile.generic_string());
		m_config.Read();

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


		Renderer::SetBackgroundColor(Color(0.1f, 0.1, 0.1f, 1.0f));

		// Set world camera to a shrunked window
		Camera* defaultCamera = Renderer::GetCamera();
		int viewportWidth = 1280;
		int viewportHeight = 720;
		int cameraPosX = (((int)Renderer::GetWindowWidth()) / 2) - (viewportWidth / 2);
		int cameraPosY = (((int)Renderer::GetWindowHeight())) - (viewportHeight) - 30;
		defaultCamera->SetViewport({ cameraPosX, cameraPosY }, { viewportWidth, viewportHeight });
		defaultCamera->SetBackgroundColor(Color(173, 216, 230, 255, false));
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
			for (auto& gui : m_GUIs) {
				Renderer::AttachGui(gui);
			}
		}
	}

	void DevLayer::OnDetach()
	{
		if (m_showTools) {
			for (auto& gui : m_GUIs) {
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
				for (auto& gui : m_GUIs) {
					Renderer::DetachGui(gui);
				}
			}
			else {
				for (auto& gui : m_GUIs) {
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
		for (auto& msg : MessageBus::GetBroadcast("SceneManager")) {
			switch (msg.type) {
			case MT_SceneChanged:
				SetScenePtr(((SceneChangedData*)msg.data)->ptr);
				break;
			}
		}
	}

	void EditorLayout::OnRender()
	{
		
		ImGui::PushFont(m_fonts[18]);

		RenderMenuBar();
		RenderDiagnosticsPanel();
		RenderScenePanel();
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

	void EditorLayout::InitResources(std::string& resourcePath)
	{
		namespace fs = std::filesystem;
		std::string ttfFile = (fs::path(resourcePath) / fs::path("NunitoSans_7pt_Condensed-Medium.ttf")).generic_string();

		if (Utils::PathExists(ttfFile)) {
			GuiManager::LoadFont(NunitoSans, ttfFile);

			m_fonts[18] = GuiManager::GetFont(NunitoSans, 18);
			m_fonts[24] = GuiManager::GetFont(NunitoSans, 24);
			m_fonts[36] = GuiManager::GetFont(NunitoSans, 36);

		}
		else {
			RDT_CORE_WARN("Could not find file '{}'", ttfFile);
		}

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

	int EditorLayout::GetDockPosX(Dock docking, int guiWidth, int margin)
	{
		switch (docking) {
		case DockLeft:
			return margin;
		case DockRight:
			return m_window_width - guiWidth - margin;
		case DockTop:
			return 0;
		case DockBottom:
			return 0;
		}

		return 0;
	}

	int EditorLayout::GetDockPosY(Dock docking, int guiHeight, int margin)
	{
		switch (docking) {
		case DockLeft:
			return 0;
		case DockRight:
			return 0;
		case DockTop:
			return margin;
		case DockBottom:
			return m_window_height - guiHeight - margin;
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
					m_templateWizardLaunched = true;
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

				unsigned int count;
				Layer** layers = m_scene->GetLayers(&count);
				for (int i = 0; i < count; i++) {
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

			unsigned int count;
			GameObject** objects = layer->GetGameObjects(&count);
			for (int i = 0; i < count; i++) {
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
	void EditorLayout::RenderTemplateWizard()
	{
		if (!m_templateWizardLaunched) {
			return;
		}

		ApplyGuiConfig(m_template_wizard);

		ImGuiWindowFlags windowConfig = 0;
		windowConfig |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize;


		if (ImGui::Begin("Template Wizard", &m_templateWizardLaunched, windowConfig)) {
			
			ImGui::PushFont(m_fonts[36]);
			AddCenteredText("Create New File");
			ImGui::PopFont();
		}
	
		ImGui::End();
	}
}
