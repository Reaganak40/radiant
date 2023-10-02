#pragma once
#include "Panel.h"

namespace rdt::core {

	class DiagnosticsGUI : public Panel {
	private:
	public:
		DiagnosticsGUI();

		void OnUpdate(const float deltaTime) override;
		void OnRender() override;

		void OnMessage(Message msg) override final;
	};
}