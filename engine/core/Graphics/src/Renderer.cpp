#include "pch.h"
#include "Renderer.h"

#include <Radiant/glCore.h>

#define RENDERER_IMPL rdt::graphics::Renderer::Impl

// ======================================================
struct RENDERER_IMPL {
	
	std::string m_application_name;

	Impl()
	{
	}

	~Impl()
	{
	}

	virtual void LaunchWindow() = 0;
};

// ======================================================
//	OPENGL RENDERER
// ======================================================
struct Renderer_OpenGL : public RENDERER_IMPL {

	rdt::glCore::glApplication* glApp = nullptr;
	Renderer_OpenGL()
	{
	}

	~Renderer_OpenGL()
	{
	}

	void LaunchWindow() override final {
		glApp = rdt::glCore::CreateApplication(m_application_name.c_str());
		glApp->LaunchWindow();
	}
};

// ======================================================
//	DIRECTX RENDERER
// ======================================================
struct Renderer_DirectX : public RENDERER_IMPL {

	Renderer_DirectX()
	{
	}

	~Renderer_DirectX()
	{
	}

	void LaunchWindow() override final {

	}
};

// ======================================================
rdt::graphics::Renderer::Impl* rdt::graphics::Renderer::m_impl = nullptr;
rdt::graphics::RenderAPI rdt::graphics::Renderer::m_framework = rdt::graphics::RenderAPI_OpenGL;

void rdt::graphics::Renderer::SetFramework(RenderAPI framework)
{
	if (m_impl == nullptr) {
		m_framework = framework;
	}
}

void rdt::graphics::Renderer::Initialize()
{
	Destroy();

	switch (m_framework)
	{
	case RenderAPI_OpenGL:
		m_impl = new Renderer_OpenGL;
		break;
	case RenderAPI_DirectX:
		// throw error
		break;
	}
}

void rdt::graphics::Renderer::Destroy()
{
	if (m_impl != nullptr) {
		delete m_impl;
		m_impl = nullptr;
	}
}

void rdt::graphics::Renderer::LaunchWindow()
{
	m_impl->LaunchWindow();
}
