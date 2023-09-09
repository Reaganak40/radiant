#include "MainMenu.h"
#include "UI/Title.h"
#include "UI/MenuSelection.h"
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

	Radiant::Renderer::SetBackgroundColor(Radiant::BLACK);
	for (auto& gui : m_GUIs) {
		Radiant::Renderer::AttachGui(gui);
	}
}

void MainMenu::OnRelease()
{
	using namespace Radiant;
	for (auto& gui : m_GUIs) {
		Renderer::DetachGui(gui);
	}

	((MenuSelection*)m_GUIs[1])->ResetSelection();
}

void MainMenu::OnRender()
{
	RunRenderQueue();

	if (((MenuSelection*)m_GUIs[1])->GetSelection() == PlayAgainstHuman) {
		ChangeScene("Arena");
	}
}

