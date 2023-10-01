#include "pch.h"
#include "Application.h"

#include "Graphics/Renderer.h"
#include "Utils/Utils.h"

namespace rdt {
	Application::Application()
	{
		Renderer::Initialize();
	}

	Application::~Application()
	{
		Renderer::Destroy();
	}

	void Application::Start(std::string appName, unsigned int windowWidth, unsigned int windowHeight, bool resizable)
	{
		Utils::SetRandomSeed();
		Renderer::CreateRadiantWindow(appName, windowWidth, windowHeight, resizable);
	}

	void Application::Run()
	{
		while (!ShouldWindowClose()) {
		}
	}
	bool Application::ShouldWindowClose()
	{
		return Renderer::ShouldWindowClose();
	}
}