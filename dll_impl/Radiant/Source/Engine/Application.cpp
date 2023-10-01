#include "pch.h"
#include "Application.h"

#include "Graphics/Renderer.h"

namespace rdt {
	Application::Application()
	{
		Renderer::Initialize();
	}

	Application::~Application()
	{
		Renderer::Destroy();
	}

	void Application::Run()
	{
		while (true) {

		}
	}
}