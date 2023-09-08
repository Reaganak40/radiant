#include "MainMenu.h"
#include "UI/Title.h"

MainMenu::MainMenu()
{

}

MainMenu::~MainMenu()
{
}

void MainMenu::OnRegister()
{
	m_GUIs.push_back(new Title);
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
}
