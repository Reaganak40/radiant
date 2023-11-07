/*******************************************************************
*	Module:  Graphics (core)
*	File:    RendererGL.h
*
*	Author: Reagan Kelley
*
*	OpenGL implementation of the renderer.
*******************************************************************/
#pragma once
#include "Core.h"
#include "Graphics/Renderer.h"

// Forward Declarations
namespace rdt {
}

// For Opengl rendering
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "RenderLayer.h"
#include "GeoMode.h"
#include "RenderCache.h"

#include "Graphics/Mesh.h"
#include "Utils/Color.h"

namespace rdt::core {

	class RendererGL : public Renderer {
	private:

		// Window management
		GLFWwindow* m_window;
		GLFWmonitor* m_monitor;
		
		std::string m_window_name;

		int m_window_width;
		int m_window_height;

		Color m_clear_color;
		
		// *****************************************************
		// 
		//			  Renderer API dependables
		// 
		// *****************************************************
		core::RenderType m_current_render_type;
		core::RenderCache m_render_cache;

		// *****************************************************
		// 
		//			  Opengl renderering dependables
		// 
		// *****************************************************
		struct glViewportData {
			int posX;
			int posY;
			int width;
			int height;

			glViewportData(int nPosX = 0, int nPosY = 0, int nWidth = 0, int nHeight = 0)
				: posX(nPosX), posY(nPosY), width(nWidth), height(nHeight) {}
		};

		glViewportData m_default_viewport;

		// Use only one VAO
		core::VertexArray* m_vertex_array;

		// Each layer contains render units (specified draw calls).
		std::map<unsigned int, RenderLayer> m_layers;

		// Keep shaders independent from render units.
		std::vector<Shader*> m_shaders;

		// Framebuffers
		std::unordered_map<int, FrameBuffer> m_frame_buffers;

		// Camera depenables
		Vec3f m_screen_origin;

		// Track currently bounded gl objects.
		FBO_ID m_current_fbo;
		VBO_ID m_current_vbo;
		IBO_ID m_current_ibo;
		ShaderID m_current_shader;
		GeoMode m_current_mode;
		glViewportData m_current_viewport;

		std::set<Camera*> m_selected_cameras;

		// For ImGui instances.
		std::vector<GuiTemplate*> m_GUIs;
		bool m_imgui_newFrameCalled;

	public:
		RendererGL();
		~RendererGL();
	protected:

		/* ***********************************************
			Renderer Singleton Implementation Functions
		** ***********************************************/
		bool ShouldWindowCloseImpl() override final;
		unsigned int GetWindowWidthImpl() override final;
		unsigned int GetWindowHeightImpl() override final;
		bool CreateWindowImpl(const std::string& windowName) override final;
		void* GetWindowInstanceImpl() override final;
		void SetBackgroundColorImpl(const Color& color) override final;
		Vec2i OnWindowResizeImpl() override final;
		void OnNewRenderWindow(int id, RenderWindow* nRenderWindow) override final;
		void EnableFullscreenImpl() override final;
		void DisableFullscreenImpl() override final;


		void ClearImpl() override final;
		void OnBeginFrameImpl() override final;
		void OnUpdateImpl(const float deltaTime) override final;
		
		/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
		void RenderImpl() override final;
		/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

		void OnEndFrameImpl() override final;

		void SetRenderTypeImpl(core::RenderType type) override final;

		void AttachGuiImpl(GuiTemplate* gui) override final;
		void DetachGuiImpl(const GuiTemplate* gui) override final;

		Vec2d ScreenToWorldCoordinatesImpl(const Vec2d& ScreenCoords, int renderWindowIndex) override final;

		void _FlushPolygonImpl(const UniqueID UUID) override final;
		Vec2d _TranslateMouseCoordsToViewportImpl(const Vec2d& mouseCoords, int renderWindowIndex) override final;

		/* ***********************************************
			    OpenGL Rendering Helper Functions
		** ***********************************************/
		void DrawContext();

		void SetFBO(FBO_ID fbo);
		void SetVBO(VBO_ID vbo);
		void SetIBO(IBO_ID ibo);
		void SetShader(ShaderID shader);
		void SetMode(GeoMode mode);
		void SetViewport(const glViewportData& nViewport);

		void AddRenderLayer(unsigned int layer);
		void AddDefaultShader();
		void UpdateTextureUniforms();
		void StartImGuiFrame();
		void ClearViewportSpace(glViewportData& viewport);

		void Clear(glViewportData& viewport, const Color& color);
	};
}
