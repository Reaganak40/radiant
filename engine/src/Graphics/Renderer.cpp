#include "pch.h"
#include <Radiant/Graphics/Renderer.hpp>

namespace rdt::graphics { struct RendererImpl; }

struct rdt::graphics::RendererImpl {

	RendererImpl()
	{
	}

	~RendererImpl()
	{
	}
};

// ======================================================================

static rdt::graphics::RendererImpl* m_impl = nullptr;

void rdt::Renderer::Initialize()
{
	Destroy();
	m_impl = new rdt::graphics::RendererImpl;
}

void rdt::Renderer::Destroy() 
{
	if (m_impl != nullptr) {
		delete m_impl;
		m_impl = nullptr;
	}
}

void rdt::Renderer::OnBeginFrame()
{
}

void rdt::Renderer::Render()
{
}

void rdt::Renderer::OnEndFrame()
{
}
