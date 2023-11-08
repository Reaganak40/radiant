#include "pch.h"
#include "Application.h"

// Engine
#include "Graphics/Renderer.h"
#include "Audio/SoundEngine.h"
#include "Utils/Input.h"
#include "Utils/Utils.h"
#include "Messaging/MessageBus.h"
#include "Physics/Physics.h"
#include "Gui/GuiManager.h"
#include "OOComponents/Scene/SceneManager.h"
#include "Graphics/Model.h"
#include "Physics/Ptag.h"
#include "Utils/Timestep.h"
#include "ResourceManager/ResourceManager.h"

// ECS
#include "Engine/ECS/ECS.h"
#include "Engine/Physics/PhysicsSystem.h"
#include "Engine/Graphics/RenderSystem.h"
#include "Engine/Graphics/AnimationSystem.h"

// DevTools
#include "Editor/DevTools.h"

namespace rdt {

	struct Application::Impl {
		Timestep m_timestep;
		Scene* m_current_scene;
		ApplicationConfig m_config;

		Impl()
			: m_current_scene(nullptr)
		{
		}

		~Impl()
		{
		}
	};

	Application::Application()
		: m_impl(new Application::Impl)
	{
		Renderer::Initialize();
		MessageBus::Initialize();
		SoundEngine::Initialize();
		Physics::Initialize();
		SceneManager::Initialize();
		ModelManager::Initialize();
		ResourceManager::Initialize();

		AddECS();
		ResourceManager::LoadDefaultResources();
	}

	Application::~Application()
	{
		delete m_impl;
  
#ifdef RDT_USE_DEV_TOOLS
		core::DevLayer::Destroy();
#endif
		ResourceManager::Destroy();
		SceneManager::Destroy();
		Physics::Destroy();
		core::PtagManager::Destroy();

		RemoveECS();

		ModelManager::Destroy();

		Input::Destroy();
		SoundEngine::Destroy();
		MessageBus::Destroy();
		Renderer::Destroy();
	}

	void Application::OnStart()
	{
		Utils::SetRandomSeed();
		Renderer::CreateRadiantWindow(m_impl->m_config.appName);
		Input::Initialize();

		Renderer::GetCamera().SetAspectRatio(m_impl->m_config.cameraAspectRatio);
		Renderer::GetCamera().SetBackgroundColor(m_impl->m_config.backgroundColor);
	}

	void Application::Run()
	{
		OnGameBegin();

		std::thread audioThread(SoundEngine::StartAudioLoop);

		/* Loop until the user closes the window */
		while (IsRunning())
		{
			/* Clears window and prepares for next game loop.*/
			BeginFrame();

			/* Get start of frame messages. */
			PollMessages1();

			/* Process input from user and update game objects. */
			ProcessInput();

			/* Update the physical world. Detect and resolve collisions. */
			UpdateWorld();

			/* Get late update messages. */
			PollMessages2();

			/* Final update of game objects before render. */
			FinalUpdate();

			/* Render the current frame. */
			Render();

			/* End the Frame, poll inputs, swap buffers. */
			EndFrame();
		}

		SoundEngine::StopAudioLoop();
		audioThread.join();
	}

	const float Application::GetDeltaTime()
	{
		return m_impl->m_timestep.deltaTime;
	}

	void Application::SetApplicationConfig(const ApplicationConfig& config)
	{
		m_impl->m_config = config;
	}

	void Application::AddECS()
	{
		// Initialize ECS Singletons
		EntityManager::Initialize();
		ComponentManager::Initialize();
		SystemManager::Initialize();

		// Add Common Components
		ComponentManager::RegisterComponent<EntityConfig>();
		ComponentManager::RegisterComponent<Sprite>();
		ComponentManager::RegisterComponent<Transform>();
		ComponentManager::RegisterComponent<RigidBody2D>();
		ComponentManager::RegisterComponent<Renderable>();
		ComponentManager::RegisterComponent<Animator>();

		// Add Common Systems
		SystemManager::RegisterSystem<PhysicsSystem>();
		SystemManager::RegisterSystem<AnimationSystem>();
		SystemManager::RegisterSystem<RenderSystem>();

	}

	void Application::RemoveECS()
	{
		SystemManager::Destroy();
		ComponentManager::Destroy();
		EntityManager::Destroy();
	}

	bool Application::IsRunning()
	{
		return !(Renderer::ShouldWindowClose());
	}

	void Application::BeginFrame()
	{
		// Get the new deltaTime for this frame.
		m_impl->m_timestep.Update(true);

		// Get the currently bounded scene for next game loop procedures.
		m_impl->m_current_scene = SceneManager::GetCurrentScene();

		// Update renderer if the window size has changed.
		if (Input::CheckWindowResize()) {
			Renderer::OnWindowResize();
		}

		// Clear the Screen
		Renderer::Clear();

		// Run renderer's begin frame procedures.
		Renderer::OnBeginFrame();

		// Update log buffer
		Log::Update();
	}

	void Application::PollMessages1()
	{
		MessageBus::SendMessages();
	}

	void Application::ProcessInput()
	{
		// All game objects in the bounded scene run their OnProcessInput()
		if (m_impl->m_current_scene != nullptr) {
			m_impl->m_current_scene->OnProcessInput(m_impl->m_timestep.deltaTime);
		}

		SystemManager::OnProcessInput(m_impl->m_timestep.deltaTime);
	}

	void Application::UpdateWorld()
	{
		SystemManager::OnWorldUpdate(m_impl->m_timestep.deltaTime);
	}

	void Application::PollMessages2()
	{
		MessageBus::SendMessages();
	}

	void Application::FinalUpdate()
	{
		// Runs the procedures for the final update of the currently bounded scene.
		if (m_impl->m_current_scene != nullptr) {
			m_impl->m_current_scene->OnFinalUpdate();
		}
		SystemManager::OnFinalUpdate();
		
		// Renderer does its update procedure.
		Renderer::OnUpdate(m_impl->m_timestep.deltaTime);
	}

	void Application::Render()
	{
		// Creates the render command queue for the currently bounded scene.
		if (m_impl->m_current_scene != nullptr) {
			m_impl->m_current_scene->OnRender();
		}

		SystemManager::OnRender();

		// Run the render queue and display the new frame.
		Renderer::Render();
	}

	void Application::EndFrame()
	{
		Renderer::OnEndFrame();
		Input::UpdateTime(m_impl->m_timestep.deltaTime);
		Input::PollInputs();
		MessageBus::ResetBroadcasts();
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