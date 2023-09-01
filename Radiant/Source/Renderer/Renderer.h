#pragma once
#include "pch.h"

// For Render API
#include "Polygon/Polygon.h"
#include "Mesh.h"
#include "Utils/Color.h"

// For Opengl rendering
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

namespace Radiant {

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

		// Renderer API dependables
		unsigned int m_current_layer;
		MeshCache m_mesh_cache;
		std::vector<std::queue<UniqueID>> m_mesh_queue;

		Color m_polygon_color;

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

		// Use only one VAO
		VertexArray* m_vertex_array;
		std::vector<glRenderUnit> m_render_units;

		// Keep shaders independent from render units.
		std::vector<Shader*> m_shaders;

		// Track currently bounded gl objects.
		VBO_ID m_current_vbo;
		IBO_ID m_current_ibo;
		ShaderID m_current_shader;

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
		static void CreateWindow(const std::string& windowName, unsigned int windowWidth, unsigned int windowHeight) {
			m_instance->CreateWindowImpl(windowName, windowWidth, windowHeight);
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
			Clears the screen with the defined background color.
		*/
		static void Clear() { glClear(GL_COLOR_BUFFER_BIT); }

		/*
			Runs the draw command queue, drawing all objects to the screen.
		*/
		static void Draw() { m_instance->DrawImpl(); }

		/*
			Starts a new render command session context.
		*/
		static void Begin(unsigned int layer = 0) { m_instance->BeginImpl(layer); }

		/*
			Ends the current render command session context.
		*/
		static void End() { m_instance->EndImpl(); }

		/*
			Adds a polygon to the render queue to be drawn next frame.
		*/
		static void AddPolygon(const Polygon& polygon) { m_instance->AddPolygonImpl(polygon); }

		static void SetPolygonColor(const Color& color) { m_instance->SetPolygonColorImpl(color); }
		static void SetPolygonColor(ColorType color) { m_instance->SetPolygonColorImpl(Color(color)); }

	private:
		void CreateWindowImpl(const std::string& windowName, unsigned int windowWidth, unsigned int windowHeight);

		void DrawImpl();

		void BeginImpl(unsigned int layer);
		void EndImpl();
		void AddPolygonImpl(const Polygon& polygon);
		void SetPolygonColorImpl(const Color& color);


		void SetVBO(VBO_ID vbo);
		void SetIBO(IBO_ID ibo);
		void SetShader(ShaderID shader);

		void AddDefaultShader();
		void AddRenderUnit();

	};

}