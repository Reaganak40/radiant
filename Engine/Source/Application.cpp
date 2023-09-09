#include "pch.h"
#include "Application.h"
#include "Renderer/Renderer.h"
#include "Utils/Input.h"
#include "Utils/Utils.h"
#include "Physics/Physics.h"
#include "Gui/GuiManager.h"
#include "Scene/SceneManager.h"

namespace Radiant
{
	Application::Application()
		: m_current_scene(nullptr)
	{
		Renderer::Initialize();
	}

	Application::~Application()
	{

		SceneManager::Destroy();
		GuiManager::Destroy();
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
		GuiManager::Initialize();
		SceneManager::Initialize();
	}

	bool Application::IsRunning()
	{
		return !Renderer::ShouldWindowClose();
	}

	void Application::BeginFrame()
	{
		// Get the new deltaTime for this frame.
		m_timestep.Update();

		// Get the currently bounded scene for next game loop procedures.
		m_current_scene = SceneManager::GetCurrentScene();

		// Update renderer if the window size has changed.
		if (Input::CheckWindowResize()) {
			Renderer::OnWindowResize();
		}

		// Clear the Screen
		Renderer::Clear();

		// Run renderer's begin frame procedures.
		Renderer::OnBeginFrame();

	}

	void Application::ProcessInput()
	{
		// All game objects in the bounded scene run their OnProcessInput()
		if (m_current_scene != nullptr) {
			m_current_scene->OnProcessInput(m_timestep.deltaTime);
		}
	}

	void Application::UpdateWorld()
	{
		// Updates all active physical objects.
		Physics::OnUpdate(m_timestep.deltaTime);
	}

	void Application::FinalUpdate()
	{
		// Runs the procedures for the final update of the currently bounded scene.
		if (m_current_scene != nullptr) {
			m_current_scene->OnFinalUpdate();
		}

		// Renderer does its update procedure.
		Renderer::OnUpdate(m_timestep.deltaTime);
	}

	void Application::Render()
	{
		// Creates the render command queue for the currently bounded scene.
		if (m_current_scene != nullptr) {
			m_current_scene->OnRender();
		}

		// RUn the render queue and display the new frame.
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

	const UniqueID Application::AddScene(const std::string& sceneName, Scene* nScene)
	{
		SceneManager::RegisterScene(sceneName, nScene);
		return nScene->GetID();
	}

	void Application::SetScene(const std::string& sceneName)
	{
		SceneManager::SetScene(sceneName);
	}
}
