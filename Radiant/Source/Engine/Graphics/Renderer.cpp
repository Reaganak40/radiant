#include "pch.h"
#include "Renderer.h"
#include "Polygon/Rect.h"
#include "Physics/Collision.h"
#include "Camera.h"
#include "Mesh.h"

#include "Model.h"
#include "ECS/CommonComponents.h"

#include "Logging/Log.h"

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

		MeshBuffer m_mesh_buffers[2];
		unsigned int current_buffer = 0;

		bool begin_called = false;
		core::Mesh m_working_mesh; // Used to create meshes in the render processes

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

		void SwapMeshBuffers() {
			current_buffer = (current_buffer + 1) % 2;
			m_mesh_buffers[current_buffer].clear();
		}

		MeshBuffer& GetBackBuffer() {
			return m_mesh_buffers[current_buffer == 1 ? 0 : 1];
		}

		void Begin(unsigned int layer) {
			
			if (begin_called) {
				RDT_CORE_ERROR("Renderer - Called Begin() twice in the same context. Did you forget to call Renderer::End()?");
				return;
			}

			m_working_mesh.Reset();
			m_working_mesh.layer = layer;
			begin_called = true;
		}

		void End() {
			if (!begin_called) {
				RDT_CORE_ERROR("Renderer - Unmatched End(), did you forget to called Renderer::Begin()?");
				return;
			}

			begin_called = false;
			m_mesh_buffers[current_buffer].push_back(m_working_mesh);
		}
	};

	// =============================================================
	Renderer::Impl* Renderer::m_impl = nullptr;
	Renderer* Renderer::m_instance = nullptr;
	int Renderer::Impl::RenderWindowID = 0;

	Renderer::Renderer()
	{
	}

	Renderer::~Renderer()
	{
	}

	std::unordered_map<int, RenderWindow*>& Renderer::GetRenderWindows()
	{
		return m_impl->m_render_windows;
	}

	void Renderer::SetFullscreenFlag(bool isFullscreen)
	{
		m_impl->m_is_fullscreen = isFullscreen;
	}

	MeshBuffer& Renderer::GetBackBuffer()
	{
		return m_impl->GetBackBuffer();
	}

	void Renderer::Initialize()
	{
		if (m_instance != nullptr) {
			Destroy();
		}

		m_impl = new Renderer::Impl;
		INIT_RENDERER();
	}

	void Renderer::Destroy()
	{
		delete m_instance;
		m_instance = nullptr;

		delete m_impl;
		m_impl = nullptr;
	}
	Vec2i Renderer::OnWindowResize()
	{
		return m_instance->OnWindowResizeImpl();
	}

	int Renderer::AddRenderWindow(RenderWindow* nRenderWindow)
	{
		int id = m_instance->m_impl->GetNextRenderWindowID();
		m_instance->m_impl->m_render_windows[id] = nRenderWindow;
		m_instance->OnNewRenderWindow(id, nRenderWindow);

		return id;
	}

	void Renderer::Render()
	{
		m_impl->SwapMeshBuffers();
		m_instance->RenderImpl();
	}

	void Renderer::Begin(unsigned int layer)
	{
		m_impl->Begin(layer);
	}

	void Renderer::End()
	{
		m_impl->End();
	}

	void Renderer::SetModel(ModelID modelID)
	{
		if (!ModelManager::ModelExists(modelID)) {
			RDT_CORE_WARN("Renderer - Tried to use unregistered model [{}]", modelID);
			return;
		}

		m_impl->m_working_mesh.modelID = modelID;
	}

	void Renderer::SetTransform(const Transform& transform)
	{
		m_impl->m_working_mesh.transform = transform;
	}

	void Renderer::SetTexture(TextureID texture)
	{
		m_impl->m_working_mesh.textureID = texture;
	}

	void Renderer::SetAtlasProfile(const AtlasProfile& profile)
	{
		m_impl->m_working_mesh.atlasProfile = profile;
	}

	void Renderer::FlipTextureHorizontal()
	{
		m_impl->m_working_mesh.flipTexture = true;
	}

	void Renderer::SetFillColor(const Color& color)
	{
		m_impl->m_working_mesh.fillColor = color.GetColor();
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

	Vec2d Renderer::ScreenToWorldCoordinates(const Vec2d& ScreenCoords, int renderWindowIndex)
	{
		return m_instance->ScreenToWorldCoordinatesImpl(ScreenCoords, renderWindowIndex);
	}

	void Renderer::_FlushPolygon(const UniqueID UUID)
	{
		if (m_instance == nullptr) {
			return;
		}
		m_instance->_FlushPolygonImpl(UUID);
	}
	Vec2d Renderer::_TranslateMouseCoordsToViewport(const Vec2d& mouseCoords, int renderWindowIndex)
	{
		return m_instance->_TranslateMouseCoordsToViewportImpl(mouseCoords, renderWindowIndex);
	}
}
