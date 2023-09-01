#include "pch.h"
#include "Gui.h"

namespace Radiant {

	void GuiTemplate::RenderImGui()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}