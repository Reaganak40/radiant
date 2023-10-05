#include "pch.h"
#include "ScenePanel.h"
#include "Messaging/MessageTypes.h"
#include "Utils/Utils.h"

namespace rdt::core {
	ScenePanel::ScenePanel()
		: m_scene(nullptr)
	{
		SetGuiDimensions(ScenePanelGuiWidth, ScenePanelGuiHeight);
		SetGuiPosition(
			GetDockPosX(DockRight, PanelMargin),
			GetDockPosY(DockTop, PanelMargin) + DiagnosticGuiHeight + PanelMargin
		);

		RegisterToMessageBus("ScenePanel");
	}

	ScenePanel::~ScenePanel()
	{
	}

	void ScenePanel::OnUpdate(const float deltaTime)
	{
		for (auto& msg : MessageBus::GetBroadcast("SceneManager")) {
			switch (msg.type) {
			case MT_SceneChanged:
				SetScenePtr(((SceneChangedData*)msg.data)->ptr);
				break;
			}
		}
	}

	void ScenePanel::OnRender()
	{
		PrepareRender();
		ThemeBegin();

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
		ThemeEnd();
	}
	void ScenePanel::OnMessage(Message msg)
	{
		switch (msg.type) {
		case MT_SendScenePtr:
			SetScenePtr(((ScenePtrData*)msg.data)->ptr);
			break;
		}
	}
	void ScenePanel::SetScenePtr(Scene* ptr)
	{
		m_scene = ptr;
	}

	void ScenePanel::AddLayerPanel(Layer* layer)
	{
		std::string name = typeid(*layer).name();
		std::string panel_header = "Layer: " + name.substr(6, name.size() - 6);

		if (ImGui::CollapsingHeader(panel_header.c_str())) {
			ImGui::Text("This is text about %s", name.c_str());
		}
	}
}