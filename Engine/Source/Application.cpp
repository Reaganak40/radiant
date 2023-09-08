#include "pch.h"
#include "Application.h"
#include "Renderer/Renderer.h"
#include "Utils/Input.h"
#include "Utils/Utils.h"
#include "Physics/Physics.h"


namespace Radiant
{
	Application::Application()
		: m_current_scene(nullptr)
	{
		Renderer::Initialize();
	}

	Application::~Application()
	{
		for (auto& [id, scene] : m_scenes) {
			delete scene;
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

		if (Input::CheckWindowResize()) {
			Renderer::OnWindowResize();
		}

		Renderer::Clear();
		Renderer::OnBeginFrame();
	}

	void Application::ProcessInput()
	{
		if (m_current_scene != nullptr) {
			m_current_scene->OnProcessInput(m_timestep.deltaTime);
		}
	}

	void Application::UpdateWorld()
	{
		Physics::OnUpdate(m_timestep.deltaTime);
	}

	void Application::FinalUpdate()
	{
		if (m_current_scene != nullptr) {
			m_current_scene->OnFinalUpdate();
		}

		Renderer::OnUpdate(m_timestep.deltaTime);
	}

	void Application::Render()
	{		
		if (m_current_scene != nullptr) {
			m_current_scene->OnRender();
		}

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

	const UniqueID Application::AddScene(Scene* nScene)
	{
		nScene->OnRegister();
		m_scenes[nScene->GetID()] = nScene;

		return nScene->GetID();
	}

	void Application::SetScene(UniqueID sceneUUID)
	{
		if (m_scenes.find(sceneUUID) == m_scenes.end()) {
			return;
		}

		if (m_current_scene != nullptr) {
			m_current_scene->OnRelease();
		}

		m_current_scene = m_scenes.at(sceneUUID);
		m_current_scene->OnBind();
	}
}
