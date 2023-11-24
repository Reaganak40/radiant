#pragma once
#include <Radiant/Utils.h>

// Forward Declarations
namespace rdt {
    struct Vec2i;
}

// Required files for struct/classes
#include "Buffers/VertexArray.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/IndexBuffer.h"
#include "Shader/Shader.h"
#include "View/Viewport.h"

namespace rdt::glCore {

	struct WindowDataComponent {
        // Window management
        GLFWwindow* m_window = nullptr;
        GLFWmonitor* m_monitor = nullptr;

        int m_window_width = 0;
        int m_window_height = 0;
        Color m_clear_color;

        std::string m_window_title = "No title";

        /*
            Initializes and launches a new glfw Context. Returns false
            on failure.
        */
        bool Launch(std::shared_ptr<WindowConfig> windowConfig);

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
        Viewport m_current_viewport;
        Viewport m_adjusted_viewport;
        AspectRatio m_aspect_ratio = NoAspectRatio;

        /*
            Initializes the default viewport
        */
        void Init(int windowWidth, int windowHeight);

        /*
            Sets and rebinds the viewport on the top of the stack
        */
        void SetViewport(int xPos, int yPos, int width, int height);

        /*
            Sets a new aspect ratio to the viewport. The viewport
            will be adjusted according to the aspect ratio.
        */
        void SetAspectRatio(AspectRatio aspect_ratio);

        /*
            Takes the current viewport and applies aspect ratio
            restrictions to it.
        */
        void ApplyAspectRatio();

        /*
            Returns a reference to the binded viewport
        */
        const Viewport& GetBindedViewport();
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
        
        glm::mat4 m_MVP;
        bool mvp_initialized = false;

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

        /*
            Sets the model view projection matrix
        */
        void SetMVP(const glm::mat4& mvp);

        /*
            Gets the model view projection matrix
        */
        glm::mat4& GetMVP();
    };

    struct TextureDataComponent {
        std::shared_ptr<TextureManager> m_texture_manager = nullptr;
        std::unordered_set<glTextureID> m_texture_requests;
        
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
}