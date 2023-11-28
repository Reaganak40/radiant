#include "pch.h"
#include <Radiant/Graphics/Renderer.hpp>

struct rdt::Renderer::Impl {

};

// ================================================
rdt::Renderer::Impl* rdt::Renderer::m_impl = nullptr;

void rdt::Renderer::Initialize()
{
	Destroy();
	m_impl = new rdt::Renderer::Impl;
}

void rdt::Renderer::Destroy() 
{
	if (m_impl != nullptr) {
		delete m_impl;
		m_impl = nullptr;
	}
}