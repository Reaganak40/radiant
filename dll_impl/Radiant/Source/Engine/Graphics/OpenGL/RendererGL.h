#pragma once
#include "Core.h"
#include "Graphics/Renderer.h"

// For Render API
#include "Polygon/Polygon.h"
#include "Polygon/Rect.h"
#include "Polygon/Line.h"
#include "Mesh.h"
#include "Utils/Color.h"
#include "Texture/TextureManager.h"

// For Opengl rendering
#include "VertexArray.h"
#include "RenderLayer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "GeoMode.h"
#include "Gui/Gui.h"
#include "RenderCache.h"

namespace rdt::core {

	class RADIANT_API RendererGL : public Renderer {
	private:

		// Window management
		GLFWwindow* m_window;
		
#pragma warning(push)
#pragma warning(disable: 4251)
		std::string m_window_name;

		unsigned int m_window_width;
		unsigned int m_window_height;

		// Camera depenables
		glm::mat4 m_proj;
		glm::mat4 m_view;
		glm::mat4 m_model;
		Vec3f m_screen_origin;

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

		// Use only one VAO
		core::VertexArray* m_vertex_array;

		// Each layer contains render units (specified draw calls).
		std::vector<core::RenderLayer> m_layers;

		// Keep shaders independent from render units.
		std::vector<core::Shader*> m_shaders;

		// Track currently bounded gl objects.
		core::VBO_ID m_current_vbo;
		core::IBO_ID m_current_ibo;
		core::ShaderID m_current_shader;
		core::GeoMode m_current_mode;

		// For ImGui instances.
		std::vector<GuiTemplate*> m_GUIs;

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
		Vec2i CreateWindowImpl(const std::string& windowName, unsigned int windowWidth, unsigned int windowHeight, bool resizable) override final;
		Vec2d GetCameraCoordinates2DImpl() override final;
		void SetBackgroundColorImpl(const Color& color) override final;
		void _FlushPolygonImpl(const UniqueID UUID) override final;

		/* ***********************************************
			    OpenGL Rendering Helper Functions
		** ***********************************************/

		void SetVBO(core::VBO_ID vbo);
		void SetIBO(core::IBO_ID ibo);
		void SetShader(core::ShaderID shader);
		void SetMode(core::GeoMode mode);

		void AddDefaultShader();
		void UpdateTextureUniforms();
	};
}
