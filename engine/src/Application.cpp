#include "pch.h"
#include "Application.h"

#include <Radiant/Utils.h>
#include <Radiant/Graphics.h>
#include <Radiant/Logger.h>
#include <Radiant/Editor.h>

namespace rdt {

	struct Application::Impl {
		Timestep m_timestep;
		std::shared_ptr<WindowConfig> m_config;

		Impl()
		{
			
			logger::Log::Initialize();
#ifdef RDT_DEBUG
			logger::Log::log_to_stdout();
#endif // RDT_DEBUG

			m_config = CreateWindowConfig();
		}

		~Impl()
		{
			TearDown();
		}

		void RunGameLoop() {

#ifdef RDT_USE_EDITOR
			editor::Editor::Test();
#endif

			while (IsRunning()) {
				
				BeginFrame();

				RenderUpdate();

				EndFrame();
			}
		}

		WindowConfig& GetWindowConfig() {
			return *m_config;
		}

		/*
			Starts a new Radiant application, launching the window instance. Aspect ratio is 16:9 by default.
			Window is resizable by default.
		*/
		void OnStart() {
			utils::SetRandomSeed();

			graphics::Renderer::Initialize();
			graphics::Renderer::SetWindowConfig(m_config);
			graphics::Renderer::LaunchWindow();
		}

		bool IsRunning() {
			return !graphics::Renderer::WindowShouldClose();
		}

		void BeginFrame() {
			graphics::Renderer::OnBeginFrame();
		}

		void RenderUpdate() {
			graphics::Renderer::OnRenderUpdate();
		}

		void EndFrame() {
			graphics::Renderer::OnEndFrame();
		}

		void TearDown() {
			graphics::Renderer::Destroy();
			logger::Log::Destroy();
		}
	};
}

// =============================================================
rdt::Application::Application()
	: m_impl(new Application::Impl)
{
}

rdt::Application::~Application()
{
	delete m_impl;
}

void rdt::Application::Run()
{
	m_impl->OnStart();
	OnGameBegin();
	m_impl->RunGameLoop();
}

rdt::WindowConfig& rdt::Application::GetWindowConfig()
{
	return m_impl->GetWindowConfig();
}
