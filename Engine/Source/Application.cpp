#include "pch.h"
#include "Application.h"
#include "Renderer/Renderer.h"
#include "Utils/Input.h"
#include "Utils/Utils.h"

namespace Radiant
{
	Application::Application()
	{
		Renderer::Initialize();
	}

	Application::~Application()
	{
		Input::Destroy();
		Renderer::Destroy();
	}

	void Application::Start(std::string appName, unsigned int windowWidth, unsigned int windowHeight)
	{
		Utils::SetRandomSeed();

		Renderer::CreateWindow(appName, windowWidth, windowHeight);
		Input::Initialize();
	}

	bool Application::IsRunning()
	{
		return !Renderer::ShouldWindowClose();
	}

	void Application::Update()
	{
		m_timestep.Update();

		Renderer::Clear();
		Renderer::Update(m_timestep.deltaTime);
	}

	void Application::Render()
	{
		Renderer::Render();
	}

	void Application::EndFrame()
	{
		Input::PollInputs();
	}

	const int Application::WindowWidth() {
		return Renderer::GetWindowWidth();
	}

	const int Application::WindowHeight() {
		return Renderer::GetWindowHeight();
	}
}
