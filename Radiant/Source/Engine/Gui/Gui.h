#pragma once
#include "Core.h"
#include "Logging/Log.h"

namespace rdt {
	class RADIANT_API GuiTemplate {
	private:
	public:
		virtual void OnUpdate(const float deltaTime) {}
		virtual void OnRender() { RDT_WARN("Using virtual GuiTemplate::OnRender()!"); }
		static void RenderImGui();
	protected:
	};

}