#include "pch.h"
#include "Renderer.h"
#include "ErrorHandling.h"
#include "Utils/Utils.h"

namespace rdt {

    using namespace core;

    Renderer* Renderer::m_instance = nullptr;

    Renderer::Renderer()
        : m_window(nullptr), m_window_name(""), m_window_width(0), m_window_height(0), m_vertex_array(nullptr),
        m_current_vbo(0), m_current_ibo(0), m_current_shader(0), m_current_layer(0), m_current_render_type(DrawFilled),
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

        for (auto& shader : m_shaders) {
            delete shader;
        }

        delete m_vertex_array;

        TextureManager::Destroy();

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

    Vec2i Renderer::CreateWindowImpl(const std::string& windowName, unsigned int windowWidth, unsigned int windowHeight, bool resizable)
    {
        Vec2i aspect_ratio = Utils::GetRatio(windowWidth, windowHeight);

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

    void Renderer::SetBackgroundColorImpl(const Vec4f& colorBits)
    {
        glClearColor(colorBits.x1, colorBits.x2, colorBits.x3, colorBits.x4);
    }

    Vec2i Renderer::OnWindowResizeImpl()
    {
        glfwGetWindowSize(m_window, (int*)&m_window_width, (int*)&m_window_height);
        if (m_window_width == 0 || m_window_height == 0) {
            return Vec2i(0, 0);
        }
        return Utils::GetRatio(m_window_width, m_window_height);
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
        bool update_slots = false;

        /*
            Step 1: Run the command queue, sorting meshes into layers.
        */
        while (!m_command_queue.empty()) {
            DrawCommand command = m_command_queue.front();
            Mesh& mesh = m_render_cache.GetMesh(command.meshIdentifier);

            m_layers[mesh.layer].AddMesh(mesh, command.renderType);
            m_command_queue.pop();
        }
        
        /*
            Step 2: Go layer by layer, draw batch by batch.
        */
        for (auto& layer : m_layers) {
            layer.CompileBatches();

            if (layer.TextureSlotsChanged()) {
                UpdateTextureUniforms();
            }

            auto units = layer.GetRenderUnits();
            for (int i = 0; i < layer.GetBatchCount(); i++) {
                auto& unit = units.at(i);

                // Draw Call Procedure
                SetVBO(unit.vboID);
                SetIBO(unit.iboID);
                SetShader(unit.shaderID);
                SetMode(unit.type == DrawOutline ? OutlineMode : FillMode);

                // Update buffers and notify the GPU
                unit.m_VBO->Update();
                unit.m_IBO->Update();
                m_vertex_array->DefineVertexBufferLayout();

                glDrawElements(
                    unit.type == RenderType::DrawLine ? GL_LINES : GL_TRIANGLES,
                    unit.m_IBO->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
            }
        }

        /*
            Step 3: Run the GUI render queue.
        */
        for (const auto& gui : m_GUIs) {
            gui->OnRender();
        }

        /*
            Step 4: Render all the GUI objects.
        */
        if (m_GUIs.size() > 0) {
            GuiTemplate::RenderImGui();
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
        for (auto& layer : m_layers) {
            layer.Flush();
        }
    }

    void Renderer::DrawRectImpl(const Vec2d& origin, const Vec2d& size, const Color& color,
        unsigned int layer)
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
        SetRenderTypeImpl(DrawFilled);
        AddPolygonImpl(*rect);
        EndImpl();
           
        // Go back to saved settings
        m_polygon_color = old_color;
    }

    void Renderer::DrawLineImpl(const Vec2d& start, const Vec2d& end, const Color& color, unsigned int layer)
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
        SetRenderTypeImpl(RenderType::DrawLine);
        AddLineImpl(*line);
        EndImpl();

        m_line_color = old_line_color;
    }

    void Renderer::BeginImpl(unsigned int layer)
    {
        while (m_layers.size() <= layer) {
            m_layers.push_back(RenderLayer());
            m_layers.back().SetDefaultShader(m_shaders[0]->GetID());
        }

        m_current_layer = layer;
        m_current_render_type = DrawFilled;
        m_polygon_texture = TextureManager::GetTexture("None");
        m_polygon_color = WHITE;
    }

    void Renderer::EndImpl()
    {
       
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
                        Vec3f((float)vertex.x, (float)vertex.y), m_polygon_color.GetColor(),
                        {0, 0}, UNASSIGNED_TEXTURE
                    )
                );
            }
            else {
                pMesh.vertices[index].position = Vec3f((float)vertex.x, (float)vertex.y);
                pMesh.vertices[index].color = m_polygon_color.GetColor();
                pMesh.vertices[index].texCoords = { 0, 0 };
                pMesh.vertices[index].texIndex = UNASSIGNED_TEXTURE;
            }
            index++;
        }

        pMesh.layer = m_current_layer;
        pMesh.texture = m_polygon_texture;
        pMesh.texAtlasCoords = m_polygon_texture_coords;

        m_command_queue.push(DrawCommand(polygon.GetUUID(), m_current_render_type));
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
                        Vec3f((float)vertex.x, (float)vertex.y), m_line_color.GetColor(),
                        { 0, 0 }, UNASSIGNED_TEXTURE
                    )
                );
            }
            else {
                pMesh.vertices[index].position = Vec3f((float)vertex.x, (float)vertex.y);
                pMesh.vertices[index].color = m_line_color.GetColor();
                pMesh.vertices[index].texCoords = { 0, 0 };
                pMesh.vertices[index].texIndex = UNASSIGNED_TEXTURE;
            }
            index++;
        }

        pMesh.layer = m_current_layer;
        pMesh.texture = nullptr;
        pMesh.texAtlasCoords = Vec2i::Zero();

        m_command_queue.push(DrawCommand(line.GetUUID(), m_current_render_type));
    }

    void Renderer::SetRenderTypeImpl(RenderType type)
    {
        m_current_render_type = type;
    }

    void Renderer::SetPolygonColorImpl(const Color& color)
    {
        m_polygon_color = color;
    }

    void Renderer::SetPolygonTextureImpl(const std::string& texName, unsigned int atlasX, unsigned int atlasY)
    {
        m_polygon_texture = TextureManager::GetTexture(texName);
        m_polygon_texture_coords.x = atlasX;
        m_polygon_texture_coords.y = atlasY;
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

    void Renderer::DetachGuiImpl(const GuiTemplate* gui)
    {
        for (auto it = m_GUIs.begin(); it != m_GUIs.end(); ++it) {
            if (*it == gui) {
                m_GUIs.erase(it);
                return;
            }
        }
    }

    void Renderer::FlushPolygonImpl(const UniqueID UUID)
    {
        m_render_cache.Flush(UUID);
    }

    void Renderer::UpdateTextureUniforms()
    {
        SetShader(m_shaders[0]->GetID());
        m_shaders[0]->SetUniform<std::array<TextureID, MAX_TEXTURES>>("uTextures", TextureManager::GetTextureSlots());
    }
}