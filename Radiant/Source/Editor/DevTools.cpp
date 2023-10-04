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

		int version;
		if (m_config.GetAttribute("Core", "Version", version)) {
			RDT_CORE_TRACE("Config: Using version v.{}", version);
		}
		else {
			RDT_CORE_TRACE("Config: No version specified");
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
				RDT_CORE_TRACE("Hiding DevTools");
				for (auto& gui : m_GUIs) {
					Renderer::DetachGui(gui);
				}
			}
			else {
				RDT_CORE_TRACE("Showing DevTools");
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
