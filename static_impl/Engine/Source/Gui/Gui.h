#pragma once
#include <stdio.h>

namespace rdt {
	
	

	class GuiTemplate {
	private:
	public:
		virtual void OnUpdate(const float deltaTime) {}
		virtual void OnRender() { printf("Warning: Using GuiTemplate::OnRender()!\n"); }
		static void RenderImGui();
	protected:
	};

}