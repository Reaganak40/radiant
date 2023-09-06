#include "pch.h"
#include "Application.h"
#include "Renderer/Renderer.h"
#include "Utils/Input.h"
#include "Utils/Utils.h"
#include "Physics/Physics.h"


namespace Radiant
{
	Application::Application()
	{
		Renderer::Initialize();
	}

	Application::~Application()
	{

		for (const auto& gameObject : m_game_objects) {
			delete gameObject;
		}

		Physics::Destroy();
		Input::Destroy();
		Renderer::Destroy();
	}

	void Application::Start(std::string appName, unsigned int windowWidth, unsigned int windowHeight)
	{
		Utils::SetRandomSeed();

		Renderer::CreateWindow(appName, windowWidth, windowHeight);
		Input::Initialize();
		Physics::Initialize();
	}

	bool Application::IsRunning()
	{
		return !Renderer::ShouldWindowClose();
	}

	void Application::BeginFrame()
	{
		m_timestep.Update();

		Renderer::Clear();
		Renderer::OnBeginFrame();
	}

	void Application::ProcessInput()
	{
		for (const auto& gameObject : m_game_objects) {
			gameObject->OnProcessInput(m_timestep.deltaTime);
		}
	}

	void Application::UpdateWorld()
	{
		Physics::OnUpdate(m_timestep.deltaTime);
	}

	void Application::FinalUpdate()
	{
		for (const auto& gameObject : m_game_objects) {
			gameObject->OnFinalUpdate();
		}

		Renderer::OnUpdate(m_timestep.deltaTime);
	}

	void Application::Render()
	{
		for (const auto& gameObject : m_game_objects) {
			gameObject->OnRender();
		}

		Renderer::DrawRect(Vec2d(300.0, 300.0), Vec2d(150.0, 150.0), BLUE);

		Renderer::Render();
	}

	void Application::EndFrame()
	{
		Renderer::OnEndFrame();
		Input::PollInputs();
	}

	const int Application::WindowWidth() {
		return Renderer::GetWindowWidth();
	}

	const int Application::WindowHeight() {
		return Renderer::GetWindowHeight();
	}

	const UniqueID Application::AddGameObject(GameObject* nGameObject)
	{
		nGameObject->OnRegister();
		m_game_objects.push_back(nGameObject);

		return nGameObject->GetUUID();
	}
}
