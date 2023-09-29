#include "MainMenu.h"
#include "UI/Title.h"
#include "UI/MenuSelection.h"
#include "Arena.h"

MainMenu::MainMenu()
{

}

MainMenu::~MainMenu()
{
}

void MainMenu::OnRegister()
{
	m_GUIs.push_back(new Title);
	m_GUIs.push_back(new MenuSelection);
}

void MainMenu::OnBind()
{

	rdt::Renderer::SetBackgroundColor(rdt::BLACK);
	for (auto& gui : m_GUIs) {
		rdt::Renderer::AttachGui(gui);
	}
}

void MainMenu::OnRelease()
{
	using namespace rdt;
	for (auto& gui : m_GUIs) {
		Renderer::DetachGui(gui);
	}

	((MenuSelection*)m_GUIs[1])->ResetSelection();
}

void MainMenu::OnRender()
{
	RunRenderQueue();

	switch (((MenuSelection*)m_GUIs[1])->GetSelection()) {
	case PlayAgainstHuman:
		ChangeScene("Arena");
		((Arena*)GetOtherScene("Arena"))->SetComputerActive(false);
		break;
	case PlayAgianstComputer:
		ChangeScene("Arena");
		((Arena*)GetOtherScene("Arena"))->SetComputerActive(true);
		break;
	case ExitGame:
		BeginExit();
		break;
	default:
		break;
	}
}

