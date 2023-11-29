#include <Radiant/Application/Application.hpp>

#include <Radiant/Window/Window.hpp>
#include <Radiant/Window/WindowConfig.hpp>
#include <Radiant/Graphics/Renderer.hpp>

#include <Radiant/Scene/SceneManager.hpp>
#include <Radiant/Scene/Scene.hpp>

#include <Radiant/ECS/EntityManager.hpp>

#include <string>

namespace rdt {

	using namespace scene;
	using namespace ecs;

	struct Application::Impl {
		size_t refCount = 0; // used to avoid more than one application instance
		float deltaTime = 0.0f;
		Scene* m_current_scene = nullptr;
		std::string m_start_scene = "";

		Impl()
		{
			Window::Create(); // create but do not launch window
		}

		~Impl()
		{
		}

		void RunGameLoop() {

			SetStartScene();

			while (IsRunning()) {
				
				BeginFrame();

				ProcessInput();

				WorldUpdate();

				FinalUpdate();

				RenderUpdate();

				EndFrame();
			}

			TearDown();
		}
		
		void InitModules() {
			
			// Initializes the rest of the Radiant modules
			Window::GetInstance().LaunchWindow();
			Renderer::Initialize();
			SceneManager::Initialize();

			InitECS();
		}

		void InitECS() {
			EntityManager::Initialize();
		}

		void SetStartScene() {
			// Set the starting scene
			SceneManager::SetCurrentScene(m_start_scene.c_str());
		}

		bool IsRunning() {
			// window is open and scene is not nullptr
			return Window::GetInstance().IsOpen() && (SceneManager::GetCurrentScene());
		}

		void BeginFrame() {
			Renderer::OnBeginFrame();
			m_current_scene = SceneManager::GetCurrentScene();
		}

		void ProcessInput() {
			m_current_scene->ProcessInput(deltaTime);
		}

		void WorldUpdate() {
			m_current_scene->WorldUpdate(deltaTime);
		}

		void FinalUpdate() {
			m_current_scene->FinalUpdate();
		}

		void RenderUpdate() {
			m_current_scene->RenderUpdate();
			Renderer::Render();
		}

		void EndFrame() {
			Renderer::OnEndFrame();
			SceneManager::OnEndFrame();
		}

		void TearDown() {

			// ECS
			EntityManager::Destroy();


			SceneManager::Destroy();
			Renderer::Destroy();
			Window::Destroy();
		}
	};
}

// =============================================================
rdt::Application::Impl* rdt::Application::m_impl = nullptr;

rdt::Application::Application()
{
	if (m_impl == nullptr) {
		m_impl = new Application::Impl;
	}
	m_impl->refCount++;
}

rdt::Application::~Application()
{
	if (m_impl != nullptr) {
		if ((--m_impl->refCount) == 0) {
			delete m_impl;
			m_impl = nullptr;
		}
	}
}

void rdt::Application::Run()
{
	m_impl->InitModules();
	OnGameBegin();
	m_impl->RunGameLoop();
}

rdt::WindowConfig& rdt::Application::GetWindowConfig()
{
	return Window::GetInstance().GetWindowConfig();
}

void rdt::Application::BeginScene(const char* sceneName)
{
	m_impl->m_start_scene = sceneName;
}
