#include "pch.h"
#include "DevTools.h"
#include "Panels/Diagnostics.h"
#include "Panels/ScenePanel.h"
#include "Graphics/Renderer.h"
#include "Utils/Utils.h"

namespace rdt::core {
	DevLayer::DevLayer()
	{
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
	}
	DevLayer::~DevLayer()
	{
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
