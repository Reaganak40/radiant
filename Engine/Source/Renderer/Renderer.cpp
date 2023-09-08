#include "pch.h"
#include "Renderer.h"
#include "ErrorHandling.h"

namespace Radiant {

    Renderer* Renderer::m_instance = nullptr;

    Renderer::Renderer()
        : m_window(nullptr), m_window_name(""), m_window_width(0), m_window_height(0), m_vertex_array(nullptr),
        m_current_vbo(0), m_current_ibo(0), m_current_shader(0), m_current_layer(0), m_current_render_cond(0),
        m_current_mode(FillMode)
    {
        /* Initialize the library */
        if (!glfwInit()) {
            ASSERT(false);
        }

        m_proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f , -1.0f, 1.0f);
        m_view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        m_model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));

        m_polygon_color = BLACK;
        m_line_color = BLACK;
    }

    Renderer::~Renderer()
    {
        // Delete all buffers from all layers
        for (const auto& layer : m_layers) {
            for (const auto& type : renderUnitOrder) {
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

    void Renderer::SetBackgroundColorImpl(const Vec4f& colorBits)
    {
        glClearColor(colorBits.x1, colorBits.x2, colorBits.x3, colorBits.x4);
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
            const Mesh& mesh = m_render_cache.GetMesh(command.meshIdentifier);

            if (mesh.vertices.size() == 2) {
                AddToRenderUnit(mesh, LineUnit);
            }
            else {
                AddToRenderUnit(mesh, DrawCommand::HasRendCond(command.renderCond, DrawOutline) ? OutlineUnit : FillUnit);
            }

            m_command_queue.pop();
        }

        for (const auto& layer : m_layers) {
            for (const auto& type : renderUnitOrder) {
                for (auto& unit : layer.renderUnits[type]) {
                    
                    // Draw Call Procedure
                    SetVBO(unit.vboID);
                    SetIBO(unit.iboID);
                    SetShader(unit.shaderID);
                    SetMode(type == OutlineUnit ? OutlineMode : FillMode);

                    unit.m_VBO->Update();
                    unit.m_IBO->Update();

                    m_vertex_array->DefineVertexBufferLayout();
                    
                    glDrawElements(
                        type == LineUnit ? GL_LINES : GL_TRIANGLES,
                        unit.m_IBO->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
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

        m_render_cache.OnEndFrame();

        // Reset the buffers of each layer.
        for (const auto& layer : m_layers) {
            for (const auto& type : renderUnitOrder) {
                for (auto& unit : layer.renderUnits[type]) {
                    unit.m_IBO->Flush();
                    unit.m_VBO->Flush();
                }
            }
        }

    }

    void Renderer::DrawRectImpl(const Vec2d& origin, const Vec2d& size, const Color& color,
        unsigned int layer, const unsigned int rendCond)
    {
        // Use the rect cache for efficiency
        std::shared_ptr<Rect> rect;

        if ((rect = m_render_cache.GetFreeRect()) == nullptr) {
            m_render_cache.AddRectToCache(std::shared_ptr<Rect>(new Rect(origin, size.x, size.y)));
            rect = m_render_cache.GetFreeRect();
        }
        else {
            rect->SetPosition(origin);
            rect->SetSize(size);
        }
        
        Color old_color = m_polygon_color;

        // Use draw API
        BeginImpl(layer);
        SetPolygonColorImpl(color);
        SetRenderCondImpl(rendCond);
        AddPolygonImpl(*rect);
        EndImpl();
           
        // Go back to saved settings
        m_polygon_color = old_color;
    }

    void Renderer::DrawLineImpl(const Vec2d& start, const Vec2d& end, const Color& color, unsigned int layer, const unsigned int rendCond)
    {
        // Use the line cache for efficiency
        std::shared_ptr<Line> line;

        if ((line = m_render_cache.GetFreeLine()) == nullptr) {
            m_render_cache.AddLineToCache(std::shared_ptr<Line>(new Line(start, end)));
            line = m_render_cache.GetFreeLine();
        }
        else {
            line->SetStart(start);
            line->SetEnd(end);
        }

        Color old_line_color = m_line_color;

        // Use draw API
        BeginImpl(layer);
        SetLineColorImpl(color);
        SetRenderCondImpl(rendCond);
        AddLineImpl(*line);
        EndImpl();

        m_line_color = old_line_color;
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
        m_current_render_cond = 0;
    }

    void Renderer::AddPolygonImpl(const Polygon& polygon)
    {
        Mesh& pMesh = m_render_cache.GetMesh(polygon.GetUUID());

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

    void Renderer::AddLineImpl(const Line& line)
    {
        Mesh& pMesh = m_render_cache.GetMesh(line.GetUUID());

        if (pMesh.indices.size() != line.GetIndices().size()) {
            pMesh.indices = line.GetIndices();
        }

        int index = 0;
        for (const auto& vertex : line.GetVertices()) {
            if (pMesh.vertices.size() == index) {

                pMesh.vertices.push_back(
                    Vertex(
                        Vec3f((float)vertex.x, (float)vertex.y), m_line_color.GetColor()
                    )
                );
            }
            else {
                pMesh.vertices[index].position = Vec3f((float)vertex.x, (float)vertex.y);
                pMesh.vertices[index].color = m_line_color.GetColor();
            }
            index++;
        }

        pMesh.layer = m_current_layer;

        m_command_queue.push(DrawCommand(line.GetUUID(), m_current_render_cond));

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

    void Renderer::SetLineColorImpl(const Color& color)
    {
        m_line_color = color;
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

    void Renderer::AddToRenderUnit(const Mesh& mesh, const glRenderUnitType type)
    {

        if (m_layers.at(mesh.layer).renderUnits[type].size() == 0) {
            m_layers.at(mesh.layer).renderUnits[type].push_back(glRenderUnit());
            glRenderUnit& unit = m_layers.at(mesh.layer).renderUnits[type].back();

            unit.m_VBO = new VertexBuffer;
            unit.m_IBO = new IndexBuffer;

            unit.vboID = unit.m_VBO->GetID();
            unit.iboID = unit.m_IBO->GetID();
            unit.shaderID = m_shaders.front()->GetID();
        }
        glRenderUnit& unit = m_layers.at(mesh.layer).renderUnits[type].back();

        unit.m_VBO->PushToBatch(mesh.vertices);
        unit.m_IBO->PushToBatch(mesh.indices, mesh.vertices.size());
    }

    void Renderer::FlushPolygonImpl(const UniqueID UUID)
    {
        m_render_cache.Flush(UUID);
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