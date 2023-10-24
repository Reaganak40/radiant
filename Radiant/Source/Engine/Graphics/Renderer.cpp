#include "pch.h"
#include "Renderer.h"
#include "Polygon/Rect.h"
#include "Physics/Collision.h"

#ifdef RDT_USE_OPENGL

#include "OpenGL/RendererGL.h"
#define INIT_RENDERER() m_instance = new rdt::core::RendererGL

#else

#define	INIT_RENDERER()

#endif

namespace rdt {

	struct Renderer::Impl {
		Camera m_camera;

		std::unordered_map<int, RenderWindow*> m_render_windows;
		static int RenderWindowID;

		bool m_use_default_viewport;
		bool m_is_fullscreen;

		Impl()
			: m_use_default_viewport(true), m_is_fullscreen(false)
		{
		}

		~Impl()
		{
		}

		int GetNextRenderWindowID() {
			return ++RenderWindowID;
		}
	};

	int Renderer::Impl::RenderWindowID = 0;
	Renderer* Renderer::m_instance = nullptr;

	Renderer::Renderer()
		: m_impl(new Renderer::Impl)
	{
	}

	Renderer::~Renderer()
	{
		delete m_impl;
	}

	std::unordered_map<int, RenderWindow*>& Renderer::GetRenderWindows()
	{
		return m_impl->m_render_windows;
	}

	void Renderer::SetFullscreenFlag(bool isFullscreen)
	{
		m_impl->m_is_fullscreen = isFullscreen;
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
	int Renderer::AddRenderWindow(RenderWindow* nRenderWindow)
	{
		int id = m_instance->m_impl->GetNextRenderWindowID();
		m_instance->m_impl->m_render_windows[id] = nRenderWindow;
		m_instance->OnNewRenderWindow(id, nRenderWindow);

		return id;
	}

	Camera& Renderer::GetCamera()
	{
		return m_instance->m_impl->m_camera;
	}

	void Renderer::SetDefaultViewport(bool use)
	{
		m_instance->m_impl->m_use_default_viewport = use;
	}

	bool Renderer::IsFullscreen()
	{
		return m_instance->m_impl->m_is_fullscreen;
	}

	bool Renderer::UsingDefaultViewport()
	{
		return m_instance->m_impl->m_use_default_viewport;
	}

	bool Renderer::IsInView(const Polygon& polygon)
	{
		using namespace core;

		const Polygon& cameraRect = m_instance->m_impl->m_camera.GetCameraBoundaryBox();
		return Collision::CheckCollisionSAT(cameraRect, polygon);;
	}
	bool Renderer::IsInView(const Vec2d& rectOrigin, const Vec2d& rectSize)
	{
		using namespace core;

		Rect rect(rectOrigin, rectSize.x, rectSize.y);
		const Polygon& cameraRect = m_instance->m_impl->m_camera.GetCameraBoundaryBox();
		return Collision::CheckCollisionSAT(cameraRect, rect);;
	}
	void Renderer::_FlushPolygon(const UniqueID UUID)
	{
		if (m_instance == nullptr) {
			return;
		}
		m_instance->_FlushPolygonImpl(UUID);
	}
}
