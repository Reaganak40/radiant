#include "pch.h"
#include "Renderer.h"

#ifdef RDT_USE_OPENGL

#include "OpenGL/RendererGL.h"
#define INIT_RENDERER() m_instance = new rdt::core::RendererGL

#else

#define	INIT_RENDERER()

#endif

namespace rdt {

	Renderer* Renderer::m_instance = nullptr;

	Renderer::Renderer()
	{
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::Initialize()
	{
		if (m_instance != nullptr) {
			Destroy();
		}

		INIT_RENDERER();
	}

	void Renderer::Destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}
}
