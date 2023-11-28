#include <Radiant/Application/Application.hpp>

#include <Radiant/Window/Window.hpp>
#include <Radiant/Window/WindowConfig.hpp>
#include <Radiant/Graphics/Renderer.hpp>
#include <Radiant/Scene/Scene.hpp>

namespace rdt {

	struct Application::Impl {
		size_t refCount = 0; // used to avoid more than one application instance
		float deltaTime = 0.0f;

		Impl()
		{
			Window::Create(); // create but do not launch window
		}

		~Impl()
		{
		}

		void RunGameLoop() {

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
		
		// Initializes the rest of the Radiant modules
		void OnStart() {
			Window::GetInstance().LaunchWindow();
			Renderer::Initialize();
		}

		bool IsRunning() {
			return Window::GetInstance().IsOpen();
		}

		void BeginFrame() {
			Renderer::OnBeginFrame();
		}

		void ProcessInput() {
			Scene::GetCurrentScene().ProcessInput(deltaTime);
		}

		void WorldUpdate() {
			Scene::GetCurrentScene().WorldUpdate(deltaTime);
		}

		void FinalUpdate() {
			Scene::GetCurrentScene().FinalUpdate();
		}

		void RenderUpdate() {
			Scene::GetCurrentScene().RenderUpdate();
			Renderer::Render();
		}

		void EndFrame() {
			Renderer::OnEndFrame();
		}

		void TearDown() {
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
	m_impl->OnStart();
	OnGameBegin();
	m_impl->RunGameLoop();
}

rdt::WindowConfig& rdt::Application::GetWindowConfig()
{
	return Window::GetInstance().GetWindowConfig();
}

void rdt::Application::BeginScene(const char* sceneName)
{
	Scene::SetCurrentScene(sceneName);
}
