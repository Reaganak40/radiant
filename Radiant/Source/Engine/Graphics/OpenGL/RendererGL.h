#pragma once
#include "Core.h"
#include "Graphics/Renderer.h"
#include "Graphics/Camera.h"

// For Opengl rendering
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "RenderLayer.h"
#include "GeoMode.h"
#include "RenderCache.h"

namespace rdt::core {

	class RADIANT_API RendererGL : public Renderer {
	private:

		// Window management
		GLFWwindow* m_window;
		
#pragma warning(push)
#pragma warning(disable: 4251)
		std::string m_window_name;

		int m_window_width;
		int m_window_height;

		// *****************************************************
		// 
		//			  Renderer API dependables
		// 
		// *****************************************************
		struct DrawCommand {
			UniqueID meshIdentifier;
			core::RenderType renderType;


			DrawCommand(UniqueID nMeshIdentifier, core::RenderType nRenderType)
				: meshIdentifier(nMeshIdentifier), renderType(nRenderType) {}
		};

		unsigned int m_current_layer;
		core::RenderType m_current_render_type;
		core::RenderCache m_render_cache;
		std::queue<DrawCommand> m_command_queue;

		Color m_polygon_color;
		Texture* m_polygon_texture;
		Vec2i m_polygon_texture_coords;

		Color m_line_color;

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
		std::vector<core::RenderLayer> m_layers;

		// Keep shaders independent from render units.
		std::vector<core::Shader*> m_shaders;

		// Camera depenables
		Camera m_camera;
		Vec3f m_screen_origin;

		// Track currently bounded gl objects.
		core::VBO_ID m_current_vbo;
		core::IBO_ID m_current_ibo;
		core::ShaderID m_current_shader;
		core::GeoMode m_current_mode;
		glViewportData m_current_viewport;

		// For ImGui instances.
		std::vector<GuiTemplate*> m_GUIs;
		bool m_imgui_newFrameCalled;

#pragma warning(pop)

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
		Vec2d GetCameraCoordinates2DImpl() override final;
		void SetBackgroundColorImpl(const Color& color) override final;
		Vec2i OnWindowResizeImpl() override final;

		void ClearImpl() override final;
		void OnBeginFrameImpl() override final;
		void OnUpdateImpl(const float deltaTime) override final;
		void RenderImpl() override final;
		void OnEndFrameImpl() override final;
		void DrawRectImpl(const Vec2d& origin, const Vec2d& size, const Color& color, unsigned int layer = 0) override final;
		void DrawLineImpl(const Vec2d& start, const Vec2d& end, const Color& color, unsigned int layer = 0) override final;
		void BeginImpl(unsigned int layer) override final;
		void EndImpl() override final;

		void SetRenderTypeImpl(core::RenderType type) override final;
		void AddPolygonImpl(const Polygon& polygon) override final;
		void AddLineImpl(const Line& line) override final;
		void SetLineColorImpl(const Color& color) override final;
		void SetPolygonColorImpl(const Color& color) override final;
		void SetPolygonTextureImpl(const std::string& texName, unsigned int atlasX = 0, unsigned int atlasY = 0) override final;
		void AttachGuiImpl(GuiTemplate* gui) override final;
		void DetachGuiImpl(const GuiTemplate* gui) override final;

		void _FlushPolygonImpl(const UniqueID UUID) override final;

		/* ***********************************************
			    OpenGL Rendering Helper Functions
		** ***********************************************/

		void SetVBO(core::VBO_ID vbo);
		void SetIBO(core::IBO_ID ibo);
		void SetShader(core::ShaderID shader);
		void SetMode(core::GeoMode mode);
		void SetViewport(glViewportData nViewport);

		void AddDefaultShader();
		void UpdateTextureUniforms();
		void StartImGuiFrame();
	};
}
