#include "pch.h"
#include "glWindow.h"

#include "Internal/DataComponents.h"
#include "Texture/Texture.h"
#include "Texture/TextureManager.h"
#include "Utils/ErrorHandling.h"

#include <Radiant/Utils.h>
#include <Radiant/Logger.h>

struct rdt::glCore::glWindow::Impl {
private:
    WindowDataComponent       m_window_data;
    VertexArrayDataComponent  m_VAO_component;
    ViewportDataComponent     m_viewport_component;
    VertexBufferDataComponent m_VBO_component;
    IndexBufferDataComponent  m_IBO_component;
    ShaderDataComponent       m_shader_component;
    TextureDataComponent      m_texture_component;
public:
    Impl()
    {
        RDT_CORE_TRACE("Initialized new OpenGL state...");

        /* Initialize the library */
        if (!glfwInit()) {
            RDT_CORE_ERROR("Could not initliaze glfw.");
        }
    }

    ~Impl()
    {
        glfwTerminate();
    }

    bool LaunchWindow(std::shared_ptr<WindowConfig> windowConfig)
    {
        RDT_CORE_TRACE("Launching new window instance...");

        if (!m_window_data.Launch(windowConfig)) {
            return false;
        }

        // set default VAO
        m_VAO_component.Init();
        m_VAO_component.GetVertexArray().Bind();

        // Set default viewport
        m_viewport_component.Init(m_window_data.m_window_width, m_window_data.m_window_height);

        // Set default shader
        m_shader_component.Init();

        // Initialize texture resources
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        m_texture_component.Init();

        // Set to 60 FPS
        glfwSwapInterval(1);

        return true;
    }

    const char* GetName()
    {
        return m_window_data.m_window_title.c_str();
    }

    bool WindowShouldClose()
    {
        return glfwWindowShouldClose(m_window_data.m_window);
    }

    void BeginFrame()
    {
        m_VBO_component.Reset();
        m_IBO_component.Reset();
        ClearWindow();
    }

    void EndFrame()
    {
        /* Swap front and back buffers */
        glfwSwapBuffers(m_window_data.m_window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    void ClearViewport()
    {
        auto& viewport = m_viewport_component.GetBindedViewport();
        glScissor(viewport.posX, viewport.posY, viewport.width, viewport.height);
        glEnable(GL_SCISSOR_TEST);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void SetClearColor(const Color& color)
    {
        m_window_data.m_clear_color = color;
        auto& colVals = color.GetColor();

        glClearColor(colVals.x1, colVals.x2, colVals.x3, colVals.x4);
    }

    void SetViewport(int xPos, int yPos, int width, int height)
    {
        m_viewport_component.SetViewport(xPos, yPos, width, height);
    }

    glTextureID CreateTextureFromPNG(const char* filepath, Vec2i& outSize)
    {
        glTextureID nID = m_texture_component.m_texture_manager->LoadTextureFromPNG(filepath);
        if (nID == GL_CORE_NULL_TEXTURE_ID) {
            return nID;
        }

        Texture& texture = m_texture_component.m_texture_manager->GetTexture(nID);
        outSize.x = texture.GetImageWidth();
        outSize.y = texture.GetImageHeight();
        return nID;
    }

    void UseTexture(glTextureID tID)
    {
        if (m_texture_component.m_texture_requests.size() == MAX_TEXTURES) {
            RDT_CORE_WARN("glCore - Maximum in-use textures reached!");
            return;
        }

        m_texture_component.m_texture_requests.insert(tID);
    }

    void SetMVPMatrix(const glm::mat4& mvp)
    {
        m_shader_component.SetMVP(mvp);
    }

    void SetAspectRatio(AspectRatio aspectRatio)
    {
        m_viewport_component.m_aspect_ratio = aspectRatio;
    }

    void SubmitVertices(glVertex* vertices, size_t vertexCount)
    {
        m_VBO_component.GetVertexBuffer().PushToBatch(vertices, vertexCount);
    }

    void SubmitIndices(unsigned int* indices, size_t indexCount, size_t vertexCount)
    {
        m_IBO_component.GetIndexBuffer().PushToBatch(indices, indexCount, vertexCount);
    }
    
    void DrawContext()
    {
        auto& vao = m_VAO_component.GetVertexArray();
        auto& vbo = m_VBO_component.GetVertexBuffer();
        auto& ibo = m_IBO_component.GetIndexBuffer();

        // Ensure the the vertex array for this window is binded
        vao.Bind();

        // Bind buffers for draw call
        m_VBO_component.BindVertexBuffer(vbo);
        m_IBO_component.BindIndexBuffer(ibo);

        // Send buffer data to GPU
        vbo.Update();
        ibo.Update();
        vao.DefineVertexBufferLayout();

        // Setup shader and textures
        m_texture_component.BindTextureRequests();
        PrepareShader();

        // Draw using the binded data
        glDrawElements(GL_TRIANGLES, ibo.GetIndexCount(), GL_UNSIGNED_INT, nullptr);

        // Prepare for next draw call
        SetUpNextContext();
    }

private:

    void ClearWindow() {

        // Clear entire window with black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glDisable(GL_SCISSOR_TEST);
        glClear(GL_COLOR_BUFFER_BIT);

        // Restore clear color
        auto& colVals = m_window_data.m_clear_color.GetColor();
        glClearColor(colVals.x1, colVals.x2, colVals.x3, colVals.x4);
    }
    /*
        Prepares the buffers for the next draw call.
    */
    void SetUpNextContext()
    {
        m_VBO_component.OnFinishedDrawCall();
        m_IBO_component.OnFinishedDrawCall();
        m_texture_component.OnFinishedDrawCall();
    }

    /*
        Takes the bounded shader and updates it uniforms (if needed)
    */
    void PrepareShader()
    {
        auto& shader = m_shader_component.GetCurrentShader();

        // Update texture map uniform
        if (m_texture_component.m_texture_manager->TextureSlotsChanged()) {
            shader.SetUniform<std::array<glTextureID, MAX_TEXTURES>>("uTextures", m_texture_component.m_texture_manager->GetTextureSlotMap());
        }

        // Update MVP uniform
        // We don't check for changes of MVP because it is expected
        // to be changed regularly.
        shader.SetUniform<glm::mat4>("uMVP", m_shader_component.GetMVP());
    }
};

rdt::glCore::glWindow::glWindow()
    : m_impl(new glWindow::Impl)
{
}

rdt::glCore::glWindow::~glWindow()
{
    delete m_impl;
}

const char* rdt::glCore::glWindow::GetName()
{
    return m_impl->GetName();
}

bool rdt::glCore::glWindow::LaunchWindow(std::shared_ptr<WindowConfig> windowConfig)
{
    return m_impl->LaunchWindow(windowConfig);
}

bool rdt::glCore::glWindow::AppShouldClose()
{
    return m_impl->WindowShouldClose();
}

void rdt::glCore::glWindow::BeginFrame()
{
    m_impl->BeginFrame();
}

void rdt::glCore::glWindow::EndFrame()
{
    m_impl->EndFrame();
}

void rdt::glCore::glWindow::SetViewport(int xPos, int yPos, int width, int height)
{
    m_impl->SetViewport(xPos, yPos, width, height);
}

rdt::glCore::glTextureID rdt::glCore::glWindow::CreateTextureFromPNG(const char* filepath, Vec2i& outSize)
{
    return m_impl->CreateTextureFromPNG(filepath, outSize);
}

void rdt::glCore::glWindow::UseTexture(glTextureID tID)
{
    m_impl->UseTexture(tID);
}

void rdt::glCore::glWindow::Clear()
{
    m_impl->ClearViewport();
}

void rdt::glCore::glWindow::SetClearColor(const Color& color)
{
    m_impl->SetClearColor(color);
}

void rdt::glCore::glWindow::SetMVPMatrix(const glm::mat4& mvp)
{
    m_impl->SetMVPMatrix(mvp);
}

void rdt::glCore::glWindow::SetAspectRatio(AspectRatio aspectRatio)
{
    m_impl->SetAspectRatio(aspectRatio);
}

void rdt::glCore::glWindow::SubmitVertices(glVertex* vertices, size_t vertexCount)
{
    m_impl->SubmitVertices(vertices, vertexCount);
}

void rdt::glCore::glWindow::SubmitIndices(unsigned int* indices, size_t indexCount, size_t vertexCount)
{
    m_impl->SubmitIndices(indices, indexCount, vertexCount);
}

void rdt::glCore::glWindow::Draw()
{
    m_impl->DrawContext();
}