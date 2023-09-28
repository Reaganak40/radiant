#pragma once
#include "pch.h"

// For Render API
#include "Polygon/Polygon.h"
#include "Polygon/Rect.h"
#include "Polygon/Line.h"
#include "Mesh.h"
#include "Utils/Color.h"
#include "Texture/TextureManager.h"

// For Opengl rendering
#include "VertexArray.h"
#include "Layer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "GeoMode.h"
#include "Gui/Gui.h"
#include "RenderCache.h"

namespace rdt {

	class Renderer {
	private:
		Renderer();
		~Renderer();
		static Renderer* m_instance;

		// Window management
		GLFWwindow* m_window;
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
		std::vector<core::Layer> m_layers;

		// Keep shaders independent from render units.
		std::vector<core::Shader*> m_shaders;

		// Track currently bounded gl objects.
		core::VBO_ID m_current_vbo;
		core::IBO_ID m_current_ibo;
		core::ShaderID m_current_shader;
		core::GeoMode m_current_mode;

		// For ImGui instances.
		std::vector<GuiTemplate*> m_GUIs;

	public:

		// *****************************************************
		// 
		//		Renderer Build-Up and Tear-Down functions
		// 
		// *****************************************************

		/*
			Returns the Renderer signleton instance.
		*/
		static Renderer* GetInstance();

		/*
			Intiializes a new renderer singleton context.
		*/
		static void Initialize();

		/*
			Destroys the current renderer singleton context.
		*/
		static void Destroy();

		/*
			Creates a new GLFW window with the provided specifications.
		*/
		static Vec2i CreateWindow(const std::string& windowName, unsigned int windowWidth, unsigned int windowHeight, bool resizable) {
			return m_instance->CreateWindowImpl(windowName, windowWidth, windowHeight, resizable);
		}

		// *****************************************************
		// 
		//			        Renderer API Calls
		// 
		// *****************************************************

		/*
			Returns true if the window has closed and program should terminate.
		*/
		static bool ShouldWindowClose() { return glfwWindowShouldClose(m_instance->m_window); }

		/*
			Gets the current window instance's width
		*/
		static unsigned int GetWindowWidth() { return m_instance->m_window_width; }

		/*
			Gets the current window instance's height
		*/
		static unsigned int GetWindowHeight() { return m_instance->m_window_height; }

		/*
			Update renderer internals if encountered a window resize event. Returns
			the aspect ratio.
		*/
		static Vec2i OnWindowResize() { return m_instance->OnWindowResizeImpl(); }

		/*
			Gets the 2D camera coordinates based in the bottom left corner.
		*/
		static Vec2d GetCameraCoordinates2D() { return Vec2d(m_instance->m_screen_origin.x, m_instance->m_screen_origin.y); }

		/*
			Sets the background color used on clear.
		*/
		static void SetBackgroundColor(const Color& color) { m_instance->SetBackgroundColorImpl(color.GetColor()); }


		/*
			Clears the screen with the defined background color.
		*/
		static void Clear() { glClear(GL_COLOR_BUFFER_BIT); }

		/*
			Runs start of frame procedures for the renderer.
		*/
		static void OnBeginFrame() { m_instance->OnBeginFrameImpl(); }

		/*
			Runs the renderer OnUpdate step.
		*/
		static void OnUpdate(const float deltaTime) { m_instance->OnUpdateImpl(deltaTime); }

		/*
			Runs the draw command queue, drawing all objects to the screen.
		*/
		static void Render() { m_instance->RenderImpl(); }

		/*
			Polls render events and flushes the command queue and buffers.
		*/
		static void OnEndFrame() { m_instance->OnEndFrameImpl(); }

		/*
			Utility function for quickly drawing a Rect to the screen.
		*/
		static void DrawRect(const Vec2d& origin, const Vec2d& size, const Color& color, 
			unsigned int layer = 0) {
			m_instance->DrawRectImpl(origin, size, color, layer);
		}

		/*
			Utility function for quickly drawing a Line to the screen.
		*/
		static void DrawLine(const Vec2d& start, const Vec2d& end, const Color& color,
			unsigned int layer = 0) {
			m_instance->DrawLineImpl(start, end, color, layer);
		}

		/*
			Starts a new render command session context.
		*/
		static void Begin(unsigned int layer = 0) { m_instance->BeginImpl(layer); }

		/*
			Ends the current render command session context.
		*/
		static void End() { m_instance->EndImpl(); }

		/*
			Sets the render condition for the current context.
		*/
		static void SetRenderType(core::RenderType type) { m_instance->SetRenderTypeImpl(type); }

		/*
			Adds a polygon to the render queue to be drawn next frame.
		*/
		static void AddPolygon(const Polygon& polygon) { m_instance->AddPolygonImpl(polygon); }

		/*
			Adds a line to the render queue to be drawn next frame.
		*/
		static void AddLine(const Line& line) { m_instance->AddLineImpl(line); }

		/*
			Sets the draw color for lines, which will be used on the next line.
		*/
		static void SetLineColor(const Color& color) { m_instance->SetLineColorImpl(color); }

		/*
			Sets the draw color for lines, which will be used on the next line.
		*/
		static void SetLineColor(ColorType color) { m_instance->SetLineColorImpl(color); }

		/*
			Sets the draw color for polygons, which will be used on the next polygon.
		*/
		static void SetPolygonColor(const Color& color) { m_instance->SetPolygonColorImpl(color); }

		/*
			Sets the draw color for polygons, which will be used on the next polygon.
		*/
		static void SetPolygonColor(ColorType color) { m_instance->SetPolygonColorImpl(color); }

		/*
			Sets the textureID for the next polygon. This ID should correspond with
			an already loaded texture in the TextureManager. If the texture has a
			defined atlas, provide the tile coordinate index.

			This currently only works for Rects.
		*/
		static void SetPolygonTexture(const std::string& texName, unsigned int atlasX = 0, unsigned int atlasY = 0) { m_instance->SetPolygonTextureImpl(texName, atlasX, atlasY); }

		/*
			Attaches a Gui instance, which will be drawn at the end of the draw command queue.
			The Renderer is not responsible for freeing the GUI instance. It should be detached
			and freed by its owner.
		*/
		static void AttachGui(GuiTemplate* gui) { m_instance->m_GUIs.push_back(gui); }

		/*
			Removes a GUI that is currently being rendered each frame.
		*/
		static void DetachGui(const GuiTemplate* gui) { m_instance->DetachGuiImpl(gui); }

		/*
			Removes a polygon from renderer mesh cache.
		*/
		static void _FlushPolygon(const UniqueID UUID) { m_instance->FlushPolygonImpl(UUID); }

		friend class Input;

	private:
		Vec2i CreateWindowImpl(const std::string& windowName, unsigned int windowWidth, unsigned int windowHeight, bool resizable);

		void SetBackgroundColorImpl(const Vec4f& colorBits);

		Vec2i OnWindowResizeImpl();

		void OnBeginFrameImpl();
		void OnUpdateImpl(const float deltaTime);
		void RenderImpl();
		void OnEndFrameImpl();

		void DrawRectImpl(const Vec2d& origin, const Vec2d& size, const Color& color, unsigned int layer);
		void DrawLineImpl(const Vec2d& start, const Vec2d& end, const Color& color, unsigned int layer);
		
		void BeginImpl(unsigned int layer);
		void EndImpl();
		void AddPolygonImpl(const Polygon& polygon);
		void AddLineImpl(const Line& line);
		void SetRenderTypeImpl(core::RenderType type);
		void SetPolygonColorImpl(const Color& color);
		void SetPolygonTextureImpl(const std::string& texName, unsigned int atlasX, unsigned int atlasY);
		void SetLineColorImpl(const Color& color);


		void SetVBO(core::VBO_ID vbo);
		void SetIBO(core::IBO_ID ibo);
		void SetShader(core::ShaderID shader);
		void SetMode(core::GeoMode mode);

		void AddDefaultShader();

		void DetachGuiImpl(const GuiTemplate* gui);

		void FlushPolygonImpl(const UniqueID UUID);

		void UpdateTextureUniforms();
	};

}