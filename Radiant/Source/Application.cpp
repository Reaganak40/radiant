#include "pch.h"
#include "Application.h"
#include "Renderer/Renderer.h"

namespace Radiant
{
	Application::Application()
	{
		Renderer::Initialize();
	}

	Application::~Application()
	{
		Renderer::Destroy();
	}

	void Application::Start(std::string appName, unsigned int windowWidth, unsigned int windowHeight)
	{
		Renderer::CreateWindow(appName, windowWidth, windowHeight);
	}

	bool Application::IsRunning()
	{
		return !Renderer::ShouldWindowClose();
	}

	void Application::Update()
	{
		Renderer::Clear();
		Renderer::Update(0);
	}

	void Application::Render()
	{
		Renderer::Draw();
	}


}
