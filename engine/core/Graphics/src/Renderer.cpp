#include "pch.h"
#include "Renderer.h"

#include <Radiant/glCore.h>
#include <Radiant/Utils.h>

// ======================================================
struct rdt::graphics::Renderer::Impl {
	
	std::shared_ptr<WindowConfig> m_window_config = nullptr;

	Impl()
	{
	}

	virtual ~Impl()
	{
	}

	virtual void LaunchWindow() = 0;
};

#ifdef RDT_USE_DIRECTX
// ======================================================
//	DIRECTX RENDERER
// ======================================================
struct Renderer_DirectX : public rdt::graphics::Renderer::Impl {

	Renderer_DirectX()
	{
	}

	~Renderer_DirectX()
	{
	}

	void LaunchWindow() override final {

	}
};
#define RENDERER_IMPL Renderer_DirectX

#else
#ifndef RDT_USE_OPENGL
	#pragma message("rdt_platform not defined ... using OpenGL by default.") 
	#define RDT_USE_OPENGL
#endif
// ======================================================
//	OPENGL RENDERER
// ======================================================
namespace rdt {
	struct Renderer_OpenGL : public rdt::graphics::Renderer::Impl {

		rdt::glCore::glApplication* glApp = nullptr;
		Renderer_OpenGL()
		{
		}

		~Renderer_OpenGL()
		{
		}

		void LaunchWindow() override final {

			if (m_window_config == nullptr) {
				m_window_config = std::make_shared<WindowConfig>(CreateWindowConfig());
			}

			glApp = rdt::glCore::CreateApplication(m_window_config->GetWindowName());
			glApp->LaunchWindow();
		}
	};
}
#define RENDERER_IMPL Renderer_OpenGL
#endif
// ======================================================
rdt::graphics::Renderer::Impl* rdt::graphics::Renderer::m_impl = nullptr;

void rdt::graphics::Renderer::Initialize()
{
	Destroy();
	m_impl = new RENDERER_IMPL;
}

void rdt::graphics::Renderer::Destroy()
{
	if (m_impl != nullptr) {
		delete (RENDERER_IMPL*)m_impl;
		m_impl = nullptr;
	}
}

void rdt::graphics::Renderer::SetWindowConfig(std::shared_ptr<WindowConfig> windowConfig)
{
	m_impl->m_window_config = windowConfig;
}

void rdt::graphics::Renderer::LaunchWindow()
{
	m_impl->LaunchWindow();
}
