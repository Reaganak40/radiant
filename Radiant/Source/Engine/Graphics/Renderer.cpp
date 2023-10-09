#include "pch.h"
#include "Renderer.h"

#ifdef RDT_USE_OPENGL

#include "OpenGL/RendererGL.h"
#define INIT_RENDERER() m_instance = new rdt::core::RendererGL

#else

#define	INIT_RENDERER()

#endif

namespace rdt {

	struct Renderer::Impl {
		std::unordered_map <std::string, Camera*> m_cameras;
		Camera* m_default_camera;

		std::unordered_map<int, RenderWindow*> m_render_windows;
		static int RenderWindowID;

		Impl()
		{
			m_default_camera = nullptr;
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

	void Renderer::SetDefaultCamera(Camera* defaultCamera)
	{
		m_impl->m_default_camera = defaultCamera;
	}

	std::unordered_map<int, RenderWindow*>& Renderer::GetRenderWindows()
	{
		return m_impl->m_render_windows;
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

	void Renderer::AddCamera(const std::string& alias, Camera* nCamera)
	{
		if (m_instance->m_impl->m_cameras.find(alias) != m_instance->m_impl->m_cameras.end()) {
			RDT_CORE_WARN("Renderer: Duplicate Camera '{}', AddCamera() ignored...", alias);
			return;
		}

		if (alias.empty()) {
			RDT_CORE_WARN("Renderer: Empty camera names are not allowed!");
			return;
		}

		m_instance->m_impl->m_cameras[alias] = nCamera;
	}
	Camera* Renderer::GetCamera(const std::string& cameraName)
	{
		if (cameraName.empty()) {
			return m_instance->m_impl->m_default_camera;
		}

		if (m_instance->m_impl->m_cameras.find(cameraName) == m_instance->m_impl->m_cameras.end()) {
			RDT_CORE_WARN("Renderer - Camera '{}' does not exist.", cameraName);
			return nullptr;
		}
		return m_instance->m_impl->m_cameras.at(cameraName);
	}
}
