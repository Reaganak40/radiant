#include "pch.h"
#include "RendererGL.h"

#include "Logging/Log.h"
#include "Utils/Utils.h"

#include "ErrorHandling.h"

namespace rdt::core {
	RendererGL::RendererGL()
        : m_window(nullptr), m_window_name(""), m_window_width(0), m_window_height(0), m_vertex_array(nullptr),
        m_current_vbo(0), m_current_ibo(0), m_current_shader(0), m_current_layer(0), m_current_render_type(DrawFilled),
        m_current_mode(FillMode)
	{
		RDT_CORE_INFO("Renderer is using OpenGL");

        /* Initialize the library */
        if (!glfwInit()) {
            RDT_CORE_FATAL("Could not initliaze glfw.");
            ASSERT(false);
        }

        m_proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
        m_view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        m_model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        m_polygon_color = BLACK;
        m_line_color = BLACK;
	}

	RendererGL::~RendererGL()
	{
        for (auto& shader : m_shaders) {
            delete shader;
        }

        delete m_vertex_array;

        TextureManager::Destroy();

        glfwTerminate();
	}

    bool RendererGL::ShouldWindowCloseImpl()
    {
        return glfwWindowShouldClose(m_window);
    }

    unsigned int RendererGL::GetWindowWidthImpl()
    {
        return m_window_width;
    }

    unsigned int RendererGL::GetWindowHeightImpl()
    {
        return m_window_height;
    }

    Vec2i RendererGL::CreateWindowImpl(const std::string& windowName, unsigned int windowWidth, unsigned int windowHeight, bool resizable)
	{
        RDT_CORE_TRACE("Launching new window instance...");
        Vec2i aspect_ratio = Utils::GetRatio(windowWidth, windowHeight);

        m_window_name = windowName;
        m_window_width = windowWidth;
        m_window_height = windowHeight;

        /* Create a windowed mode window and its OpenGL context */
        m_window = glfwCreateWindow(m_window_width, m_window_height, m_window_name.c_str(), NULL, NULL);
        if (!m_window)
        {
            glfwTerminate();
            RDT_CORE_FATAL("Failed to create glfw window");
            ASSERT(false);
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(m_window);

        // Load all OpenGL functions using the glfw loader function
        // If you use SDL you can use: https://wiki.libsdl.org/SDL_GL_GetProcAddress
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            RDT_CORE_FATAL("Failed to initialize OpenGL context");
            ASSERT(false);
        }

        // glad populates global constants after loading to indicate,
        // if a certain extension/version is available.
        RDT_CORE_INFO("OpenGL {}.{}", GLVersion.major, GLVersion.minor);

        glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, resizable);

        // For opengl error handling
        EnableErrorCallback();

        // Initialize ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(m_window, true);
        ImGui_ImplOpenGL3_Init("#version 330");


        // set default background color
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        // set default VAO
        m_vertex_array = new VertexArray;
        VertexArray::Bind(m_vertex_array->GetID());

        TextureManager::Initialize();

        // set default shader
        AddDefaultShader();
        UpdateTextureUniforms();

        // Setup Camera (model view project matrix)
        m_screen_origin = Vec3f(0.0f, 0.0f, 0.0f);

        if (aspect_ratio.x == 16 && aspect_ratio.y == 9) {
            m_proj = glm::ortho(0.0f, (float)1920, 0.0f, (float)1080, -1.0f, 1.0f);
        }
        else {
            m_proj = glm::ortho(0.0f, (float)m_window_width, 0.0f, (float)m_window_height, -1.0f, 1.0f);
        }

        SetShader(m_shaders[0]->GetID());
        glm::mat4 mvp = m_proj * m_view * m_model;
        m_shaders[0]->SetUniform<glm::mat4>("uMVP", mvp);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Set to 60 FPS
        glfwSwapInterval(1);

        return aspect_ratio;
	}

    Vec2d RendererGL::GetCameraCoordinates2DImpl()
    {
        return Vec2d(m_screen_origin.x, m_screen_origin.y);
    }

    void RendererGL::SetBackgroundColorImpl(const Color& color)
    {
        const Vec4f& colorBits = color.GetColor();
        glClearColor(colorBits.x1, colorBits.x2, colorBits.x3, colorBits.x4);
    }

    void RendererGL::_FlushPolygonImpl(const UniqueID UUID)
    {
        m_render_cache.Flush(UUID);
    }

    void RendererGL::SetVBO(core::VBO_ID vbo)
    {
        if (vbo != m_current_vbo) {
            VertexBuffer::Bind(vbo);
            m_current_vbo = vbo;
        }
    }

    void RendererGL::SetIBO(core::IBO_ID ibo)
    {
        if (ibo != m_current_ibo) {
            IndexBuffer::Bind(ibo);
            m_current_ibo = ibo;
        }
    }

    void RendererGL::SetShader(core::ShaderID shader)
    {
        if (shader != m_current_shader) {
            Shader::Bind(shader);
            m_current_shader = shader;
        }
    }

    void RendererGL::SetMode(core::GeoMode mode)
    {
        if (m_current_mode != mode) {
            ActivateGeoMode(mode);
            m_current_mode = mode;
        }
    }

    void RendererGL::AddDefaultShader()
    {
        Shader* shader = new Shader;
        shader->LoadDefaultShader();

        if (m_shaders.size() != 0) {
            m_shaders.insert(m_shaders.begin(), shader);
        }
        else {
            m_shaders.push_back(shader);
        }
    }

    void RendererGL::UpdateTextureUniforms()
    {
        SetShader(m_shaders[0]->GetID());
        m_shaders[0]->SetUniform<std::array<TextureID, MAX_TEXTURES>>("uTextures", TextureManager::GetTextureSlots());
    }
}