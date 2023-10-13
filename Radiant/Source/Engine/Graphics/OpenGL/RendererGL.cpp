#include "pch.h"
#include "RendererGL.h"
#include "Gui/GuiManager.h"

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

        m_polygon_color = BLACK;
        m_line_color = BLACK;
        m_imgui_newFrameCalled = false;
	}

	RendererGL::~RendererGL()
	{
        for (auto& shader : m_shaders) {
            delete shader;
        }

        delete m_vertex_array;

        TextureManager::Destroy();
        GuiManager::Destroy();

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

    bool RendererGL::CreateWindowImpl(const std::string& windowName)
	{
        RDT_CORE_TRACE("Launching new window instance...");

        m_window_name = windowName;
        
        // Get monitor and mode for windowed fullscreen
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        /* Create a windowed mode window and its OpenGL context */
        m_window = glfwCreateWindow(mode->width, mode->height, m_window_name.c_str(), NULL, NULL);
        if (!m_window)
        {
            glfwTerminate();
            RDT_CORE_FATAL("Failed to create glfw window");
            ASSERT(false);
        }
        
        /* Start with maximized window */
        glfwMaximizeWindow(m_window);
        glfwGetWindowSize(m_window, &m_window_width, &m_window_height);
        m_default_viewport = glViewportData(0, 0, m_window_width, m_window_height);

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

        // For opengl error handling
        EnableErrorCallback();

        // Initialize ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(m_window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
        GuiManager::Initialize();

        // set default VAO
        m_vertex_array = new VertexArray;
        VertexArray::Bind(m_vertex_array->GetID());

        TextureManager::Initialize();

        // set default shader
        AddDefaultShader();
        UpdateTextureUniforms();

        // Setup Camera (model view project matrix)
        m_screen_origin = Vec3f(0.0f, 0.0f, 0.0f);
        
        glm::mat4 mvp = GetCamera().GetMVP();
        SetShader(m_shaders[0]->GetID());
        m_shaders[0]->SetUniform<glm::mat4>("uMVP", mvp);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Set to 60 FPS
        glfwSwapInterval(1);

        return true;
	}

    void* RendererGL::GetWindowInstanceImpl()
    {
        return m_window;
    }

    Vec2d RendererGL::GetCameraCoordinates2DImpl()
    {
        return Vec2d(m_screen_origin.x, m_screen_origin.y);
    }

    void RendererGL::SetBackgroundColorImpl(const Color& color)
    {
        m_clear_color = color;
    }

    Vec2i RendererGL::OnWindowResizeImpl()
    {
        glfwGetWindowSize(m_window, (int*)&m_window_width, (int*)&m_window_height);
        if (m_window_width == 0 || m_window_height == 0) {
            return Vec2i(0, 0);
        }
        return Utils::GetRatio(m_window_width, m_window_height);
    }

    void RendererGL::OnNewRenderWindow(int id, RenderWindow* nRenderWindow)
    {
        m_frame_buffers[id];
        auto& fbo = m_frame_buffers.at(id);
        fbo.Init();

#pragma warning( push )
#pragma warning( disable : 4312)
        nRenderWindow->AssignTexture((void*)fbo.GetTexture());
#pragma warning( pop ) 

    }

    void RendererGL::ClearImpl()
    {
        Clear(m_default_viewport, m_clear_color);
    }

    void RendererGL::OnBeginFrameImpl()
    {
        StartImGuiFrame();
    }

    void RendererGL::OnUpdateImpl(const float deltaTime)
    {
    }

    void RendererGL::RenderImpl()
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
            Step 2: DrawContext for default viewport or render windows.
        */

        if (UsingDefaultViewport()) {
            Clear(m_default_viewport, BLACK);

            Vec2d windowDimensions = { (double)m_default_viewport.width, (double)m_default_viewport.height };
            Vec2f cameraDimensions = GetCamera().GetCameraDimensionsFromViewport((float)windowDimensions.x, (float)windowDimensions.y);
            double midX = (windowDimensions.x / 2) - (cameraDimensions.x / 2);
            double midY = (windowDimensions.y / 2) - (cameraDimensions.y / 2);
            SetViewport({ (int)midX, (int)midY, (int)cameraDimensions.x, (int)cameraDimensions.y });
            DrawContext();
        }
        else {
            for (auto& [id, window] : GetRenderWindows()) {

                auto& fbo = m_frame_buffers.at(id);
                window->OnBegin();
            
                /* Update window dimensions and setup viewport */
                Vec2d windowDimensions = window->UpdateAndGetWindowSize();
                Vec2f cameraDimensions = GetCamera().GetCameraDimensionsFromViewport(windowDimensions.x, windowDimensions.y);
                fbo.Rescale((int)windowDimensions.x, (int)windowDimensions.y);
                int midX = (windowDimensions.x / 2) - (cameraDimensions.x / 2);
                int midY = (windowDimensions.y / 2) - (cameraDimensions.y / 2);
                SetViewport({ midX, midY, (int)cameraDimensions.x, (int)cameraDimensions.y });

                /* Attach framebuffer texture to ImGui window context. */
                window->OnRender();
                window->OnEnd();

                /* Use framebuffer to draw in window. */
                SetFBO(fbo.GetID());
                DrawContext();
            }
            SetFBO(0);
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
        if (m_imgui_newFrameCalled) {
            GuiTemplate::RenderImGui();
        }
        m_imgui_newFrameCalled = false;

        /* Swap front and back buffers */
        glfwSwapBuffers(m_window);
    }

    void RendererGL::OnEndFrameImpl()
    {
        /* Poll for and process events */
        glfwPollEvents();

        m_render_cache.OnEndFrame();

        // Reset the buffers of each layer.
        for (auto& layer : m_layers) {
            layer.Flush();
        }

        m_selected_cameras.clear();
    }

    void RendererGL::DrawRectImpl(const Vec2d& origin, const Vec2d& size, const Color& color, unsigned int layer)
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
        AddPolygonImpl(*rect, Vec2f::Zero());
        EndImpl();

        // Go back to saved settings
        m_polygon_color = old_color;
    }

    void RendererGL::DrawLineImpl(const Vec2d& start, const Vec2d& end, const Color& color, unsigned int layer)
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

    void RendererGL::BeginImpl(unsigned int layer)
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

    void RendererGL::EndImpl()
    {
        /* TODO:end of render context procedures... */
    }

    void RendererGL::SetRenderTypeImpl(core::RenderType type)
    {
        m_current_render_type = type;
    }

    void RendererGL::AddPolygonImpl(const Polygon& polygon, const Vec2f& offset)
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
                        { 0, 0 }, UNASSIGNED_TEXTURE
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

        if (offset.x != 0.0f || offset.y != 0.0f) {
            for (auto& vertex : pMesh.vertices) {
                vertex.position.x += offset.x;
                vertex.position.y += offset.y;
            }
        }

        m_command_queue.push(DrawCommand(polygon.GetUUID(), m_current_render_type));
    }

    void RendererGL::AddLineImpl(const Line& line)
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

    void RendererGL::SetLineColorImpl(const Color& color)
    {
        m_line_color = color;
    }

    void RendererGL::SetPolygonColorImpl(const Color& color)
    {
        m_polygon_color = color;
    }

    void RendererGL::SetPolygonTextureImpl(const std::string& texName, unsigned int atlasX, unsigned int atlasY)
    {
        m_polygon_texture = TextureManager::GetTexture(texName);
        m_polygon_texture_coords.x = atlasX;
        m_polygon_texture_coords.y = atlasY;
    }

    void RendererGL::AttachGuiImpl(GuiTemplate* gui)
    {
        m_GUIs.push_back(gui);
        StartImGuiFrame();
    }

    void RendererGL::DetachGuiImpl(const GuiTemplate* gui)
    {
        for (auto it = m_GUIs.begin(); it != m_GUIs.end(); ++it) {
            if (*it == gui) {
                m_GUIs.erase(it);
                return;
            }
        }
    }

    void RendererGL::_FlushPolygonImpl(const UniqueID UUID)
    {
        m_render_cache.Flush(UUID);
    }

    void RendererGL::DrawContext()
    {
        SetShader(m_shaders[0]->GetID());
        glm::mat4 mvp = GetCamera().GetMVP();
        m_shaders[0]->SetUniform<glm::mat4>("uMVP", mvp);
        Clear(m_current_viewport, GetCamera().GetBackgroundColor());
        
        for (auto& layer : m_layers) {
            layer.CompileBatches();

            if (layer.TextureSlotsChanged()) {
                UpdateTextureUniforms();
            }

            auto units = layer.GetRenderUnits();
            for (unsigned int i = 0; i < layer.GetBatchCount(); i++) {
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

    }

    void RendererGL::SetFBO(FBO_ID fbo)
    {
        if (fbo != m_current_fbo) {
            FrameBuffer::Bind(fbo);
            m_current_fbo = fbo;
        }
    }

    void RendererGL::SetVBO(VBO_ID vbo)
    {
        if (vbo != m_current_vbo) {
            VertexBuffer::Bind(vbo);
            m_current_vbo = vbo;
        }
    }

    void RendererGL::SetIBO(IBO_ID ibo)
    {
        if (ibo != m_current_ibo) {
            IndexBuffer::Bind(ibo);
            m_current_ibo = ibo;
        }
    }

    void RendererGL::SetShader(ShaderID shader)
    {
        if (shader != m_current_shader) {
            Shader::Bind(shader);
            m_current_shader = shader;
        }
    }

    void RendererGL::SetMode(GeoMode mode)
    {
        if (m_current_mode != mode) {
            ActivateGeoMode(mode);
            m_current_mode = mode;
        }
    }

    void RendererGL::SetViewport(const glViewportData& nViewport)
    {
        if (nViewport.posX != m_current_viewport.posX ||
            nViewport.posY != m_current_viewport.posY ||
            nViewport.width != m_current_viewport.width ||
            nViewport.height != m_current_viewport.height) {

            glViewport(nViewport.posX, nViewport.posY, nViewport.width, nViewport.height);
            m_current_viewport = nViewport;
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
    void RendererGL::StartImGuiFrame()
    {
        if (m_GUIs.size() > 0 && !m_imgui_newFrameCalled) {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            m_imgui_newFrameCalled = true;
        }
    }
    void RendererGL::ClearViewportSpace(glViewportData& viewport)
    {
        SetViewport(viewport);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void RendererGL::Clear(glViewportData& viewport, const Color& color)
    {
        glScissor(viewport.posX, viewport.posY, viewport.width, viewport.height);
        glEnable(GL_SCISSOR_TEST);

        const Vec4f& colorBits = color.GetColor();
        glClearColor(colorBits.x1, colorBits.x2, colorBits.x3, colorBits.x4);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}