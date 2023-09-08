#pragma once
#include "pch.h"

// For Render API
#include "Polygon/Polygon.h"
#include "Polygon/Rect.h"
#include "Polygon/Line.h"
#include "Mesh.h"
#include "Utils/Color.h"
#include "RenderCache.h"

// For Opengl rendering
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "GeoMode.h"
#include "Gui/Gui.h"

namespace Radiant {

	enum RenderCond {
		NoConditions  = 0,
		DrawOutline = 1, // fill by default
		DrawLines = 2, // draw triangles by default
	};


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
			unsigned int renderCond;

			DrawCommand(UniqueID nMeshIdentifier, unsigned int nRenderCond)
				: meshIdentifier(nMeshIdentifier), renderCond(nRenderCond) {}

			static bool HasRendCond(unsigned int src, unsigned int condQuery);
			static unsigned int AddRendCond(unsigned int src, const unsigned int cond);
		};

		unsigned int m_current_layer;
		unsigned int m_current_render_cond;
		RenderCache m_render_cache;
		std::queue<DrawCommand> m_command_queue;

		Color m_polygon_color;
		Color m_line_color;

		// *****************************************************
		// 
		//			  Opengl renderering dependables
		// 
		// *****************************************************

		// Opengl render unit contains all needed data for 1 draw call.
		struct glRenderUnit {
			VBO_ID vboID;
			IBO_ID iboID;
			ShaderID shaderID;

			VertexBuffer* m_VBO;
			IndexBuffer* m_IBO;
		};

		// Container for layers in a scene with their own unique specifications.

		enum glRenderUnitType {
			FillUnit = 0,
			OutlineUnit,
			LineUnit
		};

		const glRenderUnitType renderUnitOrder[3] = { FillUnit, OutlineUnit, LineUnit };
		struct glLayer {
			std::vector<glRenderUnit> renderUnits[3];

			glLayer() {}
		};

		// Use only one VAO
		VertexArray* m_vertex_array;

		// Each layer contains render units (specified draw calls).
		std::vector<glLayer> m_layers;

		// Keep shaders independent from render units.
		std::vector<Shader*> m_shaders;

		// Track currently bounded gl objects.
		VBO_ID m_current_vbo;
		IBO_ID m_current_ibo;
		ShaderID m_current_shader;
		GeoMode m_current_mode;

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
		static Vec2i CreateWindow(const std::string& windowName, unsigned int windowWidth, unsigned int windowHeight) {
			return m_instance->CreateWindowImpl(windowName, windowWidth, windowHeight);
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
			Runs the end of frame procedures for the renderer.
		*/
		static void OnEndFrame() { m_instance->OnEndFrameImpl(); }

		/*
			Utility function for quickly drawing a Rect to the screen.
		*/
		static void DrawRect(const Vec2d& origin, const Vec2d& size, const Color& color, 
			unsigned int layer = 0, const unsigned int rendCond = NoConditions) {
			m_instance->DrawRectImpl(origin, size, color, layer, rendCond);
		}

		/*
			Utility function for quickly drawing a Line to the screen.
		*/
		static void DrawLine(const Vec2d& start, const Vec2d& end, const Color& color,
			unsigned int layer = 0, const unsigned int rendCond = NoConditions) {
			m_instance->DrawLineImpl(start, end, color, layer, rendCond);
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
		static void SetRenderCond(const unsigned int rendCond) { m_instance->SetRenderCondImpl(rendCond); }

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
			Attaches a Gui instance, which will be drawn at the end of the draw command queue.
		*/
		static void AttachGui(GuiTemplate* gui) { m_instance->m_GUIs.push_back(gui); }

		/*
			Removes a polygon from renderer mesh cache.
		*/
		static void _FlushPolygon(const UniqueID UUID) { m_instance->FlushPolygonImpl(UUID); }

		friend class Input;

	private:
		Vec2i CreateWindowImpl(const std::string& windowName, unsigned int windowWidth, unsigned int windowHeight);

		void SetBackgroundColorImpl(const Vec4f& colorBits);

		Vec2i OnWindowResizeImpl();

		void OnBeginFrameImpl();
		void OnUpdateImpl(const float deltaTime);
		void RenderImpl();
		void OnEndFrameImpl();

		void DrawRectImpl(const Vec2d& origin, const Vec2d& size, const Color& color, unsigned int layer, const unsigned int rendCond);
		void DrawLineImpl(const Vec2d& start, const Vec2d& end, const Color& color, unsigned int layer, const unsigned int rendCond);
		
		void BeginImpl(unsigned int layer);
		void EndImpl();
		void AddPolygonImpl(const Polygon& polygon);
		void AddLineImpl(const Line& line);
		void SetRenderCondImpl(const unsigned int rendCond);
		void SetPolygonColorImpl(const Color& color);
		void SetLineColorImpl(const Color& color);


		void SetVBO(VBO_ID vbo);
		void SetIBO(IBO_ID ibo);
		void SetShader(ShaderID shader);
		void SetMode(GeoMode mode);

		void AddDefaultShader();
		void AddLayer();

		void AddToRenderUnit(const Mesh& mesh, const glRenderUnitType type);

		void FlushPolygonImpl(const UniqueID UUID);
	};

}