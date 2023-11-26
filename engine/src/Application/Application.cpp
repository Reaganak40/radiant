#include "pch.h"
#include "Application.h"

#include <Radiant/Utils.h>
#include <Radiant/Graphics.h>
#include <Radiant/Logger.h>
#include <Radiant/Editor.h>
#include <Radiant/Scene.h>

namespace rdt {

	struct Application::Impl {
		std::shared_ptr<WindowConfig> m_config;
		LoopPhase m_current_phase = LoopPhase_Begin;
		float deltaTime = 0.0f;

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

				ProcessInput();

				WorldUpdate();

				FinalUpdate();

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
			m_current_phase = LoopPhase_Begin;
			graphics::Renderer::OnBeginFrame();
			deltaTime = graphics::Renderer::GetDeltaTime();

		}

		void ProcessInput() {
			m_current_phase = LoopPhase_ProcessInput;
			scene::CallUpdate(m_current_phase, deltaTime);
		}

		void WorldUpdate() {
			m_current_phase = LoopPhase_WorldUpdate;
			scene::CallUpdate(m_current_phase, deltaTime);
		}

		void FinalUpdate() {
			m_current_phase = LoopPhase_FinalUpdate;
			scene::CallUpdate(m_current_phase);
		}

		void RenderUpdate() {
			m_current_phase = LoopPhase_RenderUpdate;
			scene::CallUpdate(m_current_phase);
			graphics::Renderer::Render();
		}

		void EndFrame() {
			m_current_phase = LoopPhase_End;
			graphics::Renderer::OnEndFrame();
		}

		void TearDown() {
			scene::TearDown();
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
