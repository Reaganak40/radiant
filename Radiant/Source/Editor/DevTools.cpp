#include "pch.h"
#include "DevTools.h"
#include "Panels/Diagnostics.h"
#include "Panels/ScenePanel.h"
#include "Graphics/Renderer.h"
#include "Utils/Utils.h"

namespace rdt::core {

	DevLayer* DevLayer::m_instance = nullptr;

	DevLayer::DevLayer()
	{
		RDT_CORE_WARN("Developer tools are enabled");

		Panel::SetTheme(Theme_Gray);
		m_GUIs.push_back(new DiagnosticsGUI);
		m_GUIs.push_back(new ScenePanel);
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

		Camera* defaultCamera = Renderer::GetCamera();
		
		int viewportWidth = 1280;
		int viewportHeight = 720;
		int cameraPosX = (((int)Renderer::GetWindowWidth()) / 2) - (viewportWidth / 2);
		int cameraPosY = (((int)Renderer::GetWindowHeight()) / 2) - (viewportHeight / 2);

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
}
