#include "pch.h"
#include "RendererGL.h"
#include "Gui/GuiManager.h"
#include "Graphics/Texture/TextureManager.h"

#include "Graphics/Camera.h"
#include "Graphics/RenderWindow.h"
#include "Utils/Utils.h"

#include "Logging/Log.h"
#include "ErrorHandling.h"

#include "Graphics/RenderTypes.h"

namespace rdt::core {
	RendererGL::RendererGL()
        : m_window(nullptr), m_window_name(""), m_window_width(0), m_window_height(0), m_vertex_array(nullptr),
        m_current_vbo(0), m_current_ibo(0), m_current_fbo(0), m_current_shader(0), m_current_render_type(DrawFilled),
        m_current_mode(FillMode)
	{
		RDT_CORE_INFO("Renderer is using OpenGL");

        /* Initialize the library */
        if (!glfwInit()) {
            RDT_CORE_FATAL("Could not initliaze glfw.");
            ASSERT(false);
        }
        
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
        m_monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(m_monitor);
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

        m_default_viewport.width = m_window_width;
        m_default_viewport.height = m_window_height;

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

    void RendererGL::EnableFullscreenImpl()
    {
        const GLFWvidmode* mode = glfwGetVideoMode(m_monitor);
        glfwSetWindowMonitor(m_window, m_monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        SetFullscreenFlag(true);
    }

    void RendererGL::DisableFullscreenImpl()
    {
        const GLFWvidmode* mode = glfwGetVideoMode(m_monitor);
        glfwSetWindowMonitor(m_window, NULL, 0, 0, mode->width, mode->height, mode->refreshRate);
        glfwMaximizeWindow(m_window);
        SetFullscreenFlag(false);
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
        /*
            Step 1: Sort meshes into draw calls.
        */
        MeshToDrawCalls();

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

                if (!window->ShouldShow()) {
                    continue;
                }

                auto& fbo = m_frame_buffers.at(id);
                window->OnBegin();
            
                /* Update window dimensions and setup viewport */
                Vec2d windowDimensions = window->UpdateAndGetWindowSize();
                Vec2f cameraDimensions = GetCamera().GetCameraDimensionsFromViewport((float)windowDimensions.x, (float)windowDimensions.y);
                fbo.Rescale((int)windowDimensions.x, (int)windowDimensions.y);
                int midX = (int)((windowDimensions.x / 2) - (cameraDimensions.x / 2));
                int midY = (int)((windowDimensions.y / 2) - (cameraDimensions.y / 2));
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


        // Clear draw call buffer.
        m_drawCallAllocator.Flush();
    }

    void RendererGL::MeshToDrawCalls()
    {
        for (auto& mesh : GetBackBuffer()) {
            m_drawCallAllocator.AddMesh(mesh);
        }
    }


    void RendererGL::DrawContext()
    {
        SetShader(m_shaders[0]->GetID());
        glm::mat4 mvp = GetCamera().GetMVP();
        m_shaders[0]->SetUniform<glm::mat4>("uMVP", mvp);
        Clear(m_current_viewport, GetCamera().GetBackgroundColor());


        for (auto drawCall : m_drawCallAllocator.GetDrawCalls()) {
            Draw(drawCall);
        }
    }

    void RendererGL::Draw(glDrawCall* drawCall)
    {
        // Updates the texture slot map (if texture is not already bounded)
        TextureID targetTexture = drawCall->GetAssignedTexture();
        if (TextureManager::BindTexture(targetTexture)) {
            drawCall->UpdateTextureSlotIndex(TextureManager::GetTextureSlot(targetTexture));
            UpdateTextureUniforms();
        }

        // Draw Call Procedure
        SetVBO(drawCall->vboID);
        SetIBO(drawCall->iboID);
        SetShader(drawCall->shaderID);

        // Update buffers and notify the GPU
        drawCall->UpdatBuffers();
        m_vertex_array->DefineVertexBufferLayout();

        // TODO: Allow other render types
        SetMode(FillMode);
        RenderType type = RenderType::DrawFilled;

        // Draw using the binded data
        glDrawElements(
            type == RenderType::DrawLine ? GL_LINES : GL_TRIANGLES,
            drawCall->m_IBO->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
    }


    void RendererGL::OnEndFrameImpl()
    {
        /* Poll for and process events */
        glfwPollEvents();

        m_render_cache.OnEndFrame();

        m_selected_cameras.clear();
    }


    void RendererGL::SetRenderTypeImpl(core::RenderType type)
    {
        m_current_render_type = type;
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

    Vec2d RendererGL::ScreenToWorldCoordinatesImpl(const Vec2d& ScreenCoords, int renderWindowIndex)
    {
        auto& camera = GetCamera();
        Vec2d res = Vec2d::Zero();

        if (UsingDefaultViewport() || renderWindowIndex < 0) {
            res.x = camera.GetCameraDimensions().x * ScreenCoords.x / m_current_viewport.width;
            res.y = camera.GetCameraDimensions().y * ScreenCoords.y / m_current_viewport.height;
        }
        else {

            auto& renderWindows = GetRenderWindows();
            if (renderWindows.find(renderWindowIndex) == renderWindows.end()) {
                return Vec2d::Zero();
            }
            Vec2d size = renderWindows.at(renderWindowIndex)->GetLastSize();
            res.x = camera.GetCameraDimensions().x * ScreenCoords.x / size.x;
            res.y = camera.GetCameraDimensions().y * ScreenCoords.y / size.y;
        };

        res += camera.GetPosition();
        return res;
    }

    void RendererGL::_FlushPolygonImpl(const UniqueID UUID)
    {
        m_render_cache.Flush(UUID);
    }

    Vec2d RendererGL::_TranslateMouseCoordsToViewportImpl(const Vec2d& mouseCoords, int renderWindowIndex)
    {
        if (UsingDefaultViewport() || renderWindowIndex < 0) {
            return { mouseCoords.x - m_current_viewport.posX, mouseCoords.y - m_current_viewport.posY };
        }
        
        auto& renderWindows = GetRenderWindows();
        if (renderWindows.find(renderWindowIndex) == renderWindows.end()) {
            return Vec2d::Zero();
        }
        Vec2d offset = renderWindows.at(renderWindowIndex)->GetLastPosition();
        offset.y = m_window_height - offset.y;
        offset.y -= renderWindows.at(renderWindowIndex)->GetLastSize().y;
        return { mouseCoords.x - offset.x, mouseCoords.y - offset.y };
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

        // Add default shader to draw call allocator
        m_drawCallAllocator.SetDefaultShader(m_shaders[0]->GetID());
    }

    void RendererGL::UpdateTextureUniforms()
    {
        SetShader(m_shaders[0]->GetID());
        m_shaders[0]->SetUniform<std::array<TextureID, MAX_TEXTURES>>("uTextures", TextureManager::GetTextureSlotMap());
    }
    void RendererGL::StartImGuiFrame()
    {
        if (m_GUIs.size() > 0 && !m_imgui_newFrameCalled) {

            GuiManager::BeforeGuiNewFrame();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            GuiManager::OnGuiNewFrame();
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