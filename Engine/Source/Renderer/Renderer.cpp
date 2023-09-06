#include "pch.h"
#include "Renderer.h"
#include "ErrorHandling.h"
#include "Polygon/Rect.h"

namespace Radiant {

    Renderer* Renderer::m_instance = nullptr;

    Renderer::Renderer()
        : m_window(nullptr), m_window_name(""), m_window_width(0), m_window_height(0), m_vertex_array(nullptr),
        m_current_vbo(0), m_current_ibo(0), m_current_shader(0), m_current_layer(0), m_rect_index(0)
    {
        /* Initialize the library */
        if (!glfwInit()) {
            ASSERT(false);
        }

        m_proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f , -1.0f, 1.0f);
        m_view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        m_model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
    }


    Renderer::~Renderer()
    {
        // Delete all buffers from all layers
        unsigned int unitTypes[2] = { glLayerFillUnits, glLayerOutlineUnits };
        for (const auto& layer : m_layers) {
            for (const auto& type : unitTypes) {
                for (auto& unit : layer.renderUnits[type]) {
                    delete unit.m_IBO;
                    delete unit.m_VBO;
                }
            }
        }

        for (auto& shader : m_shaders) {
            delete shader;
        }

        delete m_vertex_array;

        for (const auto& gui : m_GUIs) {
            delete gui;
        }

        glfwTerminate();
    }

    Renderer* Renderer::GetInstance()
    {
        if (m_instance == nullptr) {
            m_instance = new Renderer;
        }

        return m_instance;
    }

    void Renderer::Initialize()
    {
        if (m_instance != nullptr) {
            Destroy();
        }
        GetInstance();
    }

    void Renderer::Destroy()
    {
        if (m_instance != nullptr) {
            delete m_instance;
            m_instance = nullptr;
        }
    }

    void Renderer::CreateWindowImpl(const std::string& windowName, unsigned int windowWidth, unsigned int windowHeight)
    {
        m_window_name = windowName;
        m_window_width = windowWidth;
        m_window_height = windowHeight;

        /* Create a windowed mode window and its OpenGL context */
        m_window = glfwCreateWindow(m_window_width, m_window_height, m_window_name.c_str(), NULL, NULL);
        if (!m_window)
        {
            glfwTerminate();
            ASSERT(false);
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(m_window);

        // Load all OpenGL functions using the glfw loader function
        // If you use SDL you can use: https://wiki.libsdl.org/SDL_GL_GetProcAddress
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            printf("Failed to initialize OpenGL context\n");
            ASSERT(false);
        }

        // glad populates global constants after loading to indicate,
        // if a certain extension/version is available.
        printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);

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

        // set default shader
        AddDefaultShader();

        // Setup Camera (model view project matrix)
        m_proj = glm::ortho(0.0f, (float)m_window_width, 0.0f, (float)m_window_height, -1.0f, 1.0f);
        m_screen_origin = Vec3f(0.0f, 0.0f, 0.0f);

        SetShader(m_shaders[0]->GetID());
        glm::mat4 mvp = m_proj * m_view * m_model;
        m_shaders[0]->SetUniform<glm::mat4>("uMVP", mvp);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Set to 60 FPS
        glfwSwapInterval(1);

    }

    void Renderer::OnBeginFrameImpl()
    {
        if (m_GUIs.size() > 0) {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }
    }

    void Renderer::OnUpdateImpl(const float deltaTime)
    {
        for (const auto& gui : m_GUIs) {
            gui->OnUpdate(deltaTime);
        }
    }

    void Renderer::RenderImpl()
    {
        while (!m_command_queue.empty()) {
            DrawCommand command = m_command_queue.front();
            const Mesh& mesh = m_mesh_cache.GetMesh(command.meshIdentifier);

            AddToRenderUnit(mesh, command.renderCond);
            m_command_queue.pop();
        }

        unsigned int unitTypes[2] = { glLayerFillUnits, glLayerOutlineUnits };
        for (const auto& layer : m_layers) {
            for (const auto& type : unitTypes) {
                for (auto& unit : layer.renderUnits[type]) {
                    
                    // Draw Call Procedure
                    SetVBO(unit.vboID);
                    SetIBO(unit.iboID);
                    SetShader(unit.shaderID);
                    SetMode(type == glLayerFillUnits ? FillMode : OutlineMode);

                    unit.m_VBO->Update();
                    unit.m_IBO->Update();

                    m_vertex_array->DefineVertexBufferLayout();
                    glDrawElements(GL_TRIANGLES, unit.m_IBO->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
                }
            }
        }

        for (const auto& gui : m_GUIs) {
            gui->OnRender();
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(m_window);
    }

    void Renderer::OnEndFrameImpl()
    {

        /* Poll for and process events */
        glfwPollEvents();


        if (m_rect_cache.size() < m_rect_index) {
            m_rect_cache.resize(m_rect_index);
        }
        m_rect_index = 0;

        // Reset the buffers of each layer.
        unsigned int unitTypes[2] = { glLayerFillUnits, glLayerOutlineUnits };
        for (const auto& layer : m_layers) {
            for (const auto& type : unitTypes) {
                for (auto& unit : layer.renderUnits[type]) {
                    unit.m_IBO->Flush();
                    unit.m_VBO->Flush();
                }
            }
        }

    }

    void Renderer::DrawRectImpl(const Vec2d& origin, const Vec2d& size, const Color& color, unsigned int layer)
    {
        // Use the rect cache for efficiency
        if (m_rect_index == m_rect_cache.size()) {
            m_rect_cache.push_back(Rect(origin, size.x, size.y));
        }
        else {
            m_rect_cache.at(m_rect_index).SetPosition(origin);
            m_rect_cache.at(m_rect_index).SetSize(size);
        }

        Color old_color = m_polygon_color;

        // Use draw API
        BeginImpl(layer);
        SetPolygonColorImpl(color);
        AddPolygonImpl(m_rect_cache.at(m_rect_index));
        EndImpl();
           
        // Go back to saved settings
        m_polygon_color = old_color;

        m_rect_index++;
    }

    void Renderer::BeginImpl(unsigned int layer)
    {
        while (m_layers.size() <= layer) {
            m_layers.push_back(glLayer());
        }

        m_current_layer = layer;
    }

    void Renderer::EndImpl()
    {
    }

    void Renderer::AddPolygonImpl(const Polygon& polygon)
    {
        Mesh& pMesh = m_mesh_cache.GetMesh(polygon.GetUUID());

        if (pMesh.indices.size() != polygon.GetIndices().size()) {
            pMesh.indices = polygon.GetIndices();
        }

        int index = 0;
        for (const auto& vertex : polygon.GetVertices()) {
            if (pMesh.vertices.size() == index) {

                pMesh.vertices.push_back(
                    Vertex(
                        Vec3f((float)vertex.x, (float)vertex.y), m_polygon_color.GetColor()
                    )
                );
            }
            else {
                pMesh.vertices[index].position = Vec3f((float)vertex.x, (float)vertex.y);
                pMesh.vertices[index].color = m_polygon_color.GetColor();
            }
            index++;
        }

        pMesh.layer = m_current_layer;

        m_command_queue.push(DrawCommand(polygon.GetUUID(), m_current_render_cond));
    }

    void Renderer::SetRenderCondImpl(const unsigned int rendCond)
    {
        m_current_render_cond = 0;
        m_current_render_cond = DrawCommand::AddRendCond(0, rendCond);
    }

    void Renderer::SetPolygonColorImpl(const Color& color)
    {
        m_polygon_color = color;
    }

    void Renderer::SetVBO(VBO_ID vbo)
    {
        if (vbo != m_current_vbo) {
            VertexBuffer::Bind(vbo);
            m_current_vbo = vbo;
        }
    }

    void Renderer::SetIBO(IBO_ID ibo)
    {
        if (ibo != m_current_ibo) {
            IndexBuffer::Bind(ibo);
            m_current_ibo = ibo;
        }
    }

    void Renderer::SetShader(ShaderID shader)
    {
        if (shader != m_current_shader) {
            Shader::Bind(shader);
            m_current_shader = shader;
        }
    }

    void Renderer::SetMode(GeoMode mode)
    {
        if (m_current_mode != mode) {
            ActivateGeoMode(mode);
            m_current_mode = mode;
        }
    }

    void Renderer::AddDefaultShader()
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

    void Renderer::AddLayer()
    {
        m_layers.push_back(glLayer());
    }

    void Renderer::AddToRenderUnit(const Mesh& mesh, unsigned int rendCond)
    {
        unsigned int targetArray = DrawCommand::HasRendCond(rendCond, DrawOutline) ? glLayerOutlineUnits : glLayerFillUnits;

        if (m_layers.at(mesh.layer).renderUnits[targetArray].size() == 0) {
            m_layers.at(mesh.layer).renderUnits[targetArray].push_back(glRenderUnit());
            glRenderUnit& unit = m_layers.at(mesh.layer).renderUnits[targetArray].back();

            unit.m_VBO = new VertexBuffer;
            unit.m_IBO = new IndexBuffer;

            unit.vboID = unit.m_VBO->GetID();
            unit.iboID = unit.m_IBO->GetID();
            unit.shaderID = m_shaders.front()->GetID();
        }
        glRenderUnit& unit = m_layers.at(mesh.layer).renderUnits[targetArray].back();

        unit.m_VBO->PushToBatch(mesh.vertices);
        unit.m_IBO->PushToBatch(mesh.indices, mesh.vertices.size());
    }

    void Renderer::FlushPolygonImpl(const UniqueID UUID)
    {
        m_mesh_cache.Flush(UUID);
    }

    bool Renderer::DrawCommand::HasRendCond(unsigned int src, unsigned int condQuery)
    {
        return (bool)(src & condQuery);
    }

    unsigned int Renderer::DrawCommand::AddRendCond(unsigned int src, const unsigned int cond) 
    {
        return src | cond;
    }
}