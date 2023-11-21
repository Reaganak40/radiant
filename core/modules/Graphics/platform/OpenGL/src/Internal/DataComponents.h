#pragma once

// Required files for struct/classes
#include "Buffers/VertexArray.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/IndexBuffer.h"
#include "Shader/Shader.h"
#include "View/Viewport.h"
#include "View/Camera.h"

namespace rdt::glCore {

	struct WindowDataComponent {
        // Window management
        GLFWwindow* m_window = nullptr;
        GLFWmonitor* m_monitor = nullptr;

        // Window Config
        std::string m_window_name = "OpenGL Window";
        int m_window_width = 0;
        int m_window_height = 0;
        
        /*
            Sets the name of the window
        */
        void SetWindowName(const std::string& name);

        /*
            Sets the window dimensions (pixels)
        */
        void SetWindowSize(int windowWidth, int windowHeight);

        /*
            Initializes and launches a new glfw Context. Returns false
            on failure.
        */
        bool Launch();

	};

    struct VertexArrayDataComponent {
        std::shared_ptr<VertexArray> m_vertex_array = nullptr;
      
        /*
            Creates a new vertex array object
        */
        void Init();

        /*
            Gets the single Vertex Array in this component
        */
        VertexArray& GetVertexArray();
    };

    struct ViewportDataComponent {
        ViewportID viewportIDCounter = 0;
        std::unordered_map<ViewportID, Viewport> m_viewports;
        ViewportID m_current_viewport = GL_CORE_DEFAULT_VIEWPORT_ID;

        /*
            Initializes the default viewport
        */
        void Init(int windowWidth, int windowHeight);

        /*
            Generates the next available viewportID
        */
        ViewportID NextViewportID();

        /*
            Creates a new viewport that can binded and drawn too. Use the
            returned viewport's ID to reference it later.
        */
        ViewportID CreateViewport(int xPos, int yPos, int width, int height);

        /*
            Sets the new current viewport
        */
        void BindViewport(ViewportID vID);

        /*
            Returns true if the given viewport is registered
        */
        bool ViewportExists(ViewportID vID);
        
        /*
            Returns a reference to the currently bound viewport
        */
        Viewport& GetCurrentViewport();

        /*
            Procedure called when the camera changes which demands new
            viewport configurations.
        */
        void OnCameraChange(Camera& bindedCamera);
    };

    struct VertexBufferDataComponent {
        std::vector<VertexBuffer> m_VBOs;
        size_t m_VBO_index = 0;
        VBO_ID m_binded_VBO;

        /*
            Resets the vertex buffers for a new draw frame
        */
        void Reset();

        /*
            Gets the currently in-use vertex buffer
        */
        VertexBuffer& GetVertexBuffer();

        /*
           Runs procedure for component when a draw call finishes.
        */
        void OnFinishedDrawCall();

        /*
            Binds the given vertex buffer. This does not have to be
            the considered in-use vertex buffer.
        */
        void BindVertexBuffer(const VertexBuffer& vbo);
    };

    struct IndexBufferDataComponent {
        std::vector<IndexBuffer> m_IBOs;
        size_t m_IBO_index = 0;
        IBO_ID m_binded_IBO;

        /*
            Resets the index buffers for a new draw frame
        */
        void Reset();

        /*
           Gets the currently in-use index buffer
       */
        IndexBuffer& GetIndexBuffer();

        /*
            Runs procedure for component when a draw call finishes.
        */
        void OnFinishedDrawCall();

        /*
           Binds the given index buffer. This does not have to be
           the considered in-use index buffer.
       */
        void BindIndexBuffer(const IndexBuffer& ibo);
    };

    struct ShaderDataComponent {
        std::unordered_map<ShaderID, Shader> m_shaders;
        ShaderID m_default_shader = GL_CORE_NULL_SHADER_ID;
        ShaderID m_current_shader = GL_CORE_NULL_SHADER_ID;

        /*
            Initializes the default shader and binds it.
        */
        void Init();

        /*
            Binds a shader according to its ID
        */
        void BindShader(ShaderID shader);

        /*
            Returns true if a shader is registered.
        */
        bool ShaderExists(ShaderID shader);

        /*
            Gets the currently in-use shader
        */
        Shader& GetCurrentShader();
    };

    struct TextureDataComponent {
        std::shared_ptr<TextureManager> m_texture_manager = nullptr;
        std::unordered_set<TextureID> m_texture_requests;
        
        /*
            Initializes the TextureManager 
        */
        void Init();

        /*
            Takes the texture requests and binds them to the texture map.
        */
        void BindTextureRequests();

        /*
           Runs procedure for component when a draw call finishes.
        */
        void OnFinishedDrawCall();
    };

    struct CameraDataComponent {
        std::unordered_map<CameraID, Camera> m_cameras;
        CameraID m_current_camera;
        CameraID cameraIDCounter;

        /*
            Creates default camera BUT does NOT bind it.
        */
        void Init();

        /*
            Generates the next cameraID
        */
        CameraID NextCameraID();

        /*
            Binds the given camera. Returns true if the camera
            did in fact change.
        */
        bool BindCamera(CameraID camera);

        /*
            Creates a new camera and returns its ID
        */
        CameraID CreateCamera();

        /*
            Returns true if the camera is registered
        */
        bool CameraExists(CameraID camera);

        /*
            Gets a reference to the current camera
        */
        Camera& GetCurrentCamera();

        /*
            Gets a camera from the camera map, does not check if it exists.
        */
        Camera& GetAnyCamera(CameraID camera);
    };
}