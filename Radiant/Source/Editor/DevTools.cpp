#include "pch.h"
#include "DevTools.h"
#include "Panels/Diagnostics.h"
#include "Graphics/Renderer.h"

namespace rdt::core {
	DevLayer::DevLayer()
	{
	}
	DevLayer::~DevLayer()
	{
	}

	void DevLayer::OnAttach()
	{
		m_GUIs.push_back(new DiagnosticsGUI);
		
		for (auto& gui : m_GUIs) {
			Renderer::AttachGui(gui);
		}
	}

	void DevLayer::OnDetach()
	{
		for (auto& gui : m_GUIs) {
			Renderer::DetachGui(gui);
		}
	}
}
