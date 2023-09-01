#pragma once
#include <stdio.h>

namespace Radiant {

	class GuiTemplate {
	private:
	public:
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() { printf("Warning: Using GuiTemplate::OnRender()!\n"); }
	protected:
		void RenderImGui();
	};

}