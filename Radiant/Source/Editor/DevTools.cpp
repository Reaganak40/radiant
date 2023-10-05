#include "pch.h"
#include "DevTools.h"

#include "Messaging/MessageTypes.h"
#include "Graphics/Renderer.h"
#include "Utils/Utils.h"

namespace rdt::core {

	DevLayer* DevLayer::m_instance = nullptr;

	DevLayer::DevLayer()
	{
		RDT_CORE_WARN("Developer tools are enabled");

		EditorLayout* layout;
		m_GUIs.push_back(layout = new EditorLayout);
		layout->SetTheme(Theme_Gray);

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

		Renderer::SetBackgroundColor(Color(0.1f, 0.1, 0.1f, 1.0f));

		// Set world camera to a shrunked window
		Camera* defaultCamera = Renderer::GetCamera();
		int viewportWidth = 1280;
		int viewportHeight = 720;
		int cameraPosX = (((int)Renderer::GetWindowWidth()) / 2) - (viewportWidth / 2);
		int cameraPosY = (((int)Renderer::GetWindowHeight())) - (viewportHeight) - 10;
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
		: m_scene(nullptr)
	{
		RegisterToMessageBus("EditorLayout");
		SetTheme(Theme_Gray);

		m_diagnostics_panel.width = DiagnosticGuiWidth;
		m_diagnostics_panel.height = DiagnosticGuiHeight;
		m_diagnostics_panel.xPos = GetDockPosX(DockRight, m_diagnostics_panel.width, PanelMargin);
		m_diagnostics_panel.yPos = GetDockPosY(DockTop, m_diagnostics_panel.height, PanelMargin);

		m_scene_panel.width = ScenePanelGuiWidth;
		m_scene_panel.height = ScenePanelGuiHeight;
		m_scene_panel.xPos = GetDockPosX(DockRight, m_scene_panel.width, PanelMargin);
		m_scene_panel.yPos = (DockTop, m_scene_panel.height, PanelMargin) + DiagnosticGuiHeight + PanelMargin;

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
		ThemeBegin();

		RenderDiagnosticsPanel();
		RenderScenePanel();
		RenderMenuBar();

		ThemeEnd();
	}

	void EditorLayout::SetTheme(EditorTheme nTheme)
	{
		switch (nTheme) {
		case Theme_Gray:
			theme_data.Titlebackground = { 0.75f, 0.75f, 0.75f, 1.0f };
			theme_data.TitlebackgroundActive = { 0.75f, 0.75f, 0.75f, 1.0f };
			theme_data.TitleBackgroundCollapsed = { 0.75f, 0.75f, 0.75f, 1.0f };
			theme_data.HeaderColor = { 0.75f, 0.75f, 0.75f, 1.0f };
			theme_data.WindowBackground = { 0.96f, 0.96f,0.96f, 0.95f };
			theme_data.TextColor = { 0.0f, 0.0f, 0.0f, 1.0f };
			theme_data.MenuBarBackground = { 0.75f, 0.75f, 0.75f, 1.0f };

			break;
		}
	}
	void EditorLayout::ThemeBegin()
	{
		ImGui::PushStyleColor(ImGuiCol_TitleBg, theme_data.Titlebackground);
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, theme_data.TitlebackgroundActive);
		ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, theme_data.TitleBackgroundCollapsed);
		ImGui::PushStyleColor(ImGuiCol_Header, theme_data.HeaderColor);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, theme_data.WindowBackground);
		ImGui::PushStyleColor(ImGuiCol_Text, theme_data.TextColor);
		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, theme_data.MenuBarBackground);
	}
	void EditorLayout::ThemeEnd()
	{
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
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

	void EditorLayout::RenderMenuBar()
	{
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
			ImGui::EndMainMenuBar();

		}
	}
}
