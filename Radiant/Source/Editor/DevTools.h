#pragma once
#include "Scene/Layer.h"
#include "Utils/Input.h"
#include "Utils/ConfigReader.h"
namespace rdt::core {

	class DevLayer : public Layer {
	private:
		bool m_showTools;
		std::string m_base_directory;

		ConfigReader m_config;

		const std::vector<InputState> controls_ShowTools1{CTRL_KEY_DOWN};
		const std::vector<InputState> controls_ShowTools2{T_KEY_PRESS};

	public:

		DevLayer();
		~DevLayer();

		void OnAttach() override final;
		void OnDetach() override final;

		/*
			Function called when the layer is active and the host application
			calls ProcessInput.
		*/
		void OnProcessInput(const float deltaTime) override final;

		/*
			Function called when the layer is active and the host application
			calls Render()
		*/
		void OnRender() override final;
	};
}