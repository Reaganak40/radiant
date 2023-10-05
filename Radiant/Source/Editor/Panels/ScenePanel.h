#pragma once
#include "Panel.h"
#include "Scene/Scene.h"

namespace rdt::core {

	class ScenePanel : public Panel {
	private:
		Scene* m_scene;
	public:
		ScenePanel();
		~ScenePanel();

		void OnUpdate(const float deltaTime) override;
		void OnRender() override;

		void OnMessage(Message msg) override final;

	private:
		void SetScenePtr(Scene* ptr);

		void AddLayerPanel(Layer* layer);
	};
}