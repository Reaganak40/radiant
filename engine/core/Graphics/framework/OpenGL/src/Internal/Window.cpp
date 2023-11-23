#include "pch.h"
#include "Window.h"
#include "Utils/ErrorHandling.h"
#include "State.h"
#include "Texture/TextureManager.h"

#include <Radiant/Logger.h>

rdt::glCore::Window::Window()
{
    m_clear_color[0] = 0.0f;
    m_clear_color[1] = 0.0f;
    m_clear_color[2] = 0.0f;
    m_clear_color[3] = 0.0f;
}

rdt::glCore::Window::~Window()
{
}

bool rdt::glCore::Window::LaunchWindow(const char* windowName)
{
    RDT_CORE_TRACE("Launching new window instance...");

    // Launch the glfw window with the given config data
    m_window_data.SetWindowName(windowName);
    if (!m_window_data.Launch()) {
        return false;
    }

    // set default VAO
    m_VAO_component.Init();
    m_VAO_component.GetVertexArray().Bind();

    // Set default viewport
    m_viewport_component.Init(m_window_data.m_window_width, m_window_data.m_window_height);

    // Set default shader
    m_shader_component.Init();

    // Set default camera
    m_camera_component.Init();
    SetCamera(GL_CORE_NULL_CAMERA_ID);

    // Initialize texture resources
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    m_texture_component.Init();

    // Set to 60 FPS
    glfwSwapInterval(1);

    return true;
}

bool rdt::glCore::Window::WindowShouldClose()
{
    return glfwWindowShouldClose(m_window_data.m_window);
}

void rdt::glCore::Window::BeginFrame()
{
    m_VBO_component.Reset();
    m_IBO_component.Reset();
}

void rdt::glCore::Window::EndFrame()
{
    /* Swap front and back buffers */
    glfwSwapBuffers(m_window_data.m_window);

    /* Poll for and process events */
    glfwPollEvents();
}

void rdt::glCore::Window::ClearViewport()
{
    auto& viewport = m_viewport_component.GetCurrentViewport();
    glScissor(viewport.posX, viewport.posY, viewport.width, viewport.height);
    glEnable(GL_SCISSOR_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
}

void rdt::glCore::Window::SetClearColor(float r, float g, float b, float a)
{
    m_clear_color[0] = r;
    m_clear_color[1] = g;
    m_clear_color[2] = b;
    m_clear_color[3] = a;

    glClearColor(r, g, b, a);
}

rdt::glCore::ViewportID rdt::glCore::Window::CreateViewport(int xPos, int yPos, int width, int height)
{
    return m_viewport_component.CreateViewport(xPos, yPos, width, height);
}

rdt::glCore::ViewportID rdt::glCore::Window::GetCurrentViewport()
{
    return m_viewport_component.m_current_viewport;
}

void rdt::glCore::Window::SubmitVertices(Vertex* vertices, size_t vertexCount)
{
    m_VBO_component.GetVertexBuffer().PushToBatch(vertices, vertexCount);
}

void rdt::glCore::Window::SubmitIndices(unsigned int* indices, size_t indexCount, size_t vertexCount)
{
    m_IBO_component.GetIndexBuffer().PushToBatch(indices, indexCount, vertexCount);
}


rdt::glCore::CameraID rdt::glCore::Window::CreateCamera()
{
    return m_camera_component.CreateCamera();
}

rdt::glCore::Camera& rdt::glCore::Window::GetCamera(CameraID cID)
{
    if (!m_camera_component.CameraExists(cID)) {
        cID = GL_CORE_NULL_CAMERA_ID;
    }

    return m_camera_component.GetAnyCamera(cID);
}


void rdt::glCore::Window::SetCamera(CameraID cID)
{
    if (m_camera_component.BindCamera(cID)) {
        auto& current_camera = m_camera_component.GetCurrentCamera();
        m_viewport_component.OnCameraChange(current_camera);
    }
}

rdt::glCore::TextureManager& rdt::glCore::Window::GetTextureManager()
{
    return *m_texture_component.m_texture_manager;
}

bool rdt::glCore::Window::AddToTextureList(TextureID tID)
{
    if (m_texture_component.m_texture_requests.size() == MAX_TEXTURES) {
        RDT_CORE_WARN("AddToTextureList - Maximum textures reached! Ignored request.");
        return false;
    }

    m_texture_component.m_texture_requests.insert(tID);
    return true;
}

void rdt::glCore::Window::SetViewport(ViewportID vID)
{
    m_viewport_component.BindViewport(vID);
}

void rdt::glCore::Window::DrawContext()
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

void rdt::glCore::Window::SetUpNextContext()
{
    m_VBO_component.OnFinishedDrawCall();
    m_IBO_component.OnFinishedDrawCall();
    m_texture_component.OnFinishedDrawCall();
}

void rdt::glCore::Window::PrepareShader()
{
    auto& shader = m_shader_component.GetCurrentShader();

    // Update texture map uniform
    if (m_texture_component.m_texture_manager->TextureSlotsChanged()) {
        shader.SetUniform<std::array<TextureID, MAX_TEXTURES>>("uTextures", m_texture_component.m_texture_manager->GetTextureSlotMap());
    }

    // Update MVP uniform
    glm::mat4 mvp = *(glm::mat4*)m_camera_component.GetCurrentCamera().GetMVP();
    shader.SetUniform<glm::mat4>("uMVP", mvp);
}