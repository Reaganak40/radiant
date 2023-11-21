/*******************************************************************
*	File:    Window.h
*	Module:  Internal
*
*	Author: Reagan Kelley
*
*	The window class holds all OpenGL context for the window of
*   an application. This includes viewports, buffers, and the
*   functionality to draw to the screen.
*******************************************************************/
#pragma once

// Forward Declarations
namespace rdt::glCore {
    struct Vertex;

    using TextureID = unsigned int;
	using CameraID = unsigned int;
}

// Required files for struct/classes
#include "DataComponents.h"

namespace rdt::glCore {
	class Window {
	private:

        WindowDataComponent m_window_data;
        VertexArrayDataComponent m_VAO_component;
        ViewportDataComponent m_viewport_component;
        VertexBufferDataComponent m_VBO_component;
        IndexBufferDataComponent m_IBO_component;
        ShaderDataComponent m_shader_component;
        TextureDataComponent m_texture_component;
        CameraDataComponent m_camera_component;

        float m_clear_color[4];

	public:
        Window();
        ~Window();

        /*
            Launches a new glfw window instance
        */
        bool LaunchWindow(const char* windowName);

        /*
            Returns true if the application should terminate (close window requested)
        */
        bool WindowShouldClose();

        /*
            Runs start-up procedures for the beginning of a new draw frame
        */
        void BeginFrame();

        /*
            Runs end of frame procedures for a draw frame (swapping buffers, polling input)
        */
        void EndFrame();

        /*
            Clears the viewport
        */
        void ClearViewport();

        /*
            Sets the color used for clearing viewports.
        */
        void SetClearColor(float r, float g, float b, float a);

        /*
            Creates a new viewport that can binded and drawn too. Use the
            returned viewport's ID to reference it later.
        */
        ViewportID CreateViewport(int xPos, int yPos, int width, int height);

        /*
            Returns the ID of the currently bound viewport.
        */
        ViewportID GetCurrentViewport();

        /*
            Creates a new camera object.
        */
        CameraID CreateCamera();

        /*
            Gets the camera represented by the given ID. Returns the default
            camera if not found.
        */
        Camera& GetCamera(CameraID cID);

        /*
            Sets the camera to be used for the current draw call.
        */
        void SetCamera(CameraID cID);

        /*
            Gets the texture manager for this window instance
        */
        TextureManager& GetTextureManager();

        /*
            Adds a texture to the texture list, which announces what textures
            will be used in the next draw call. Returns false if the maximum
            textures for a draw call has been reached.
        */
        bool AddToTextureList(TextureID tID);

        /*
            Binds the viewport for the current draw call.
        */
        void SetViewport(ViewportID vID);

        /*
            Submits an array of vertices for the current draw call. This
            can be called multiple times per draw context.
        */
        void SubmitVertices(Vertex* vertices, size_t vertexCount);

        /*
            Submits an array of indcies for the current draw call. This
            can be called multiple times per draw context.

            The indices should be relative to the last submitted vertices
        */
        void SubmitIndices(unsigned int* indices, size_t indexCount, size_t vertexCount);

        /*
            Uses the previously submission calls (draw context) to send
            an OpenGL draw call. Resets the context.
        */
        void DrawContext();


    private:
        /*
            Prepares the buffers for the next draw call.
        */
        void SetUpNextContext();

        /*
            Takes the bounded shader and updates it uniforms (if needed)
        */
        void PrepareShader();

        /*
            Updates the texture map uniform for this shader according to the current state
            of the Texture Manager
        */
        void UpdateTextureMapUniform(Shader& shader);

        /*
            Assigns the Model View Project matrix from the camera to the given shader.
        */
        void ApplyCameraToShader(Shader& shader);
	};
}