#include "pch.h"
#include "DataComponents.h"
#include "Utils/ErrorHandling.h"

#include <Radiant/Logger.h>
#include <Radiant/Utils.h>

// ======================================================
bool rdt::glCore::WindowDataComponent::Launch(std::shared_ptr<WindowConfig> windowConfig)
{
	if (m_window != nullptr) {
		return false;
	}

	// Get monitor and mode for windowed fullscreen
	m_monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(m_monitor);
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	/* Create a windowed mode window and its OpenGL context */
	m_window = glfwCreateWindow(windowConfig->GetWindowSize().x, windowConfig->GetWindowSize().y, windowConfig->GetWindowName(), NULL, NULL);
	if (!m_window)
	{
		RDT_CORE_FATAL("Failed to create glfw window");
		return false;
	}
	m_window_title = windowConfig->GetWindowName();

	/* Start with maximized window */
	if (windowConfig->ShouldMaximizeWindow()) {
		glfwMaximizeWindow(m_window);
	}

	glfwGetWindowSize(m_window, &m_window_width, &m_window_height);

	/* Make the window's context current */
	glfwMakeContextCurrent(m_window);

	// Load all OpenGL functions using the glfw loader function
	// If you use SDL you can use: https://wiki.libsdl.org/SDL_GL_GetProcAddress
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		RDT_CORE_FATAL("Failed to initialize OpenGL context");
		glfwMakeContextCurrent(NULL);
		return false;
	}

	// glad populates global constants after loading to indicate,
	// if a certain extension/version is available.
	RDT_CORE_INFO("OpenGL {}.{}", GLVersion.major, GLVersion.minor);

	// For opengl error handling
	EnableErrorCallback();
}
// ======================================================
void rdt::glCore::VertexArrayDataComponent::Init()
{
	m_vertex_array = std::make_shared<VertexArray>();
}
rdt::glCore::VertexArray& rdt::glCore::VertexArrayDataComponent::GetVertexArray()
{
	return* m_vertex_array;
}
// ======================================================
void rdt::glCore::ViewportDataComponent::Init(int windowWidth, int windowHeight)
{
	SetViewport(0, 0, windowWidth, windowHeight);
}
void rdt::glCore::ViewportDataComponent::SetViewport(int xPos, int yPos, int width, int height)
{
	m_current_viewport.posX   = xPos;
	m_current_viewport.posY   = yPos;
	m_current_viewport.width  = width;
	m_current_viewport.height = height;

	if (m_aspect_ratio == NoAspectRatio) {
		m_current_viewport.Bind();
	}
	else {
		ApplyAspectRatio();
		m_adjusted_viewport.Bind();
	}
}
void rdt::glCore::ViewportDataComponent::SetAspectRatio(AspectRatio aspect_ratio)
{
	if (m_aspect_ratio == aspect_ratio) {
		return;
	}

	if (aspect_ratio == NoAspectRatio) {
		m_current_viewport.Bind();
		return;
	}

	ApplyAspectRatio();
	m_adjusted_viewport.Bind();
}
void rdt::glCore::ViewportDataComponent::ApplyAspectRatio()
{
	Vec2d dimensions = Vec2d::Zero();
	dimensions.y = m_current_viewport.height;

	switch (m_aspect_ratio) {
	case AR_16_9:
		dimensions.x = (16.0f / 9.0f) * dimensions.y;
		break;
	case AR_1_1:
		dimensions.x = dimensions.y;
		break;
	default:
		RDT_CORE_ERROR("glCore - Tried to apply undefined aspect ratio: {}", m_aspect_ratio);
		return;
	}

	if (dimensions.x > m_current_viewport.width) {
		dimensions.x = m_current_viewport.width;
		switch (m_aspect_ratio) {
		case AR_16_9:
			dimensions.y = (9 / 16.0f) * dimensions.x;
			break;
		case AR_1_1:
			dimensions.y = dimensions.x;
			break;
		}
		dimensions.x = utils::ApplyEpsilon(dimensions.x);
		dimensions.y = utils::ApplyEpsilon(dimensions.y);
	}

	m_adjusted_viewport.width = (int)dimensions.x;
	m_adjusted_viewport.height = (int)dimensions.y;
	m_adjusted_viewport.posX = (m_current_viewport.width / 2) - (m_adjusted_viewport.width / 2);
	m_adjusted_viewport.posY = (m_current_viewport.height / 2) - (m_adjusted_viewport.height / 2);
}
const rdt::glCore::Viewport& rdt::glCore::ViewportDataComponent::GetBindedViewport()
{
	return (m_aspect_ratio == NoAspectRatio) ? m_current_viewport : m_adjusted_viewport;
}
// ======================================================
void rdt::glCore::VertexBufferDataComponent::Reset()
{
	for (size_t i = 0; i < m_VBO_index; i++) {
		m_VBOs[i].Flush();
	}

	m_VBO_index = 0;
}
rdt::glCore::VertexBuffer& rdt::glCore::VertexBufferDataComponent::GetVertexBuffer()
{
	if (m_VBO_index < m_VBOs.size()) {
		return m_VBOs[m_VBO_index];
	}

	m_VBOs.push_back(VertexBuffer());
	return m_VBOs.back();
}
void rdt::glCore::VertexBufferDataComponent::OnFinishedDrawCall()
{
	m_VBO_index++;
}
void rdt::glCore::VertexBufferDataComponent::BindVertexBuffer(const VertexBuffer& vbo)
{
	if (m_binded_VBO == vbo.GetID()) {
		return;
	}

	vbo.Bind();
	m_binded_VBO = vbo.GetID();
}
// ======================================================
void rdt::glCore::IndexBufferDataComponent::Reset()
{
	for (size_t i = 0; i < m_IBO_index; i++) {
		m_IBOs[i].Flush();
	}

	m_IBO_index = 0;
}
rdt::glCore::IndexBuffer& rdt::glCore::IndexBufferDataComponent::GetIndexBuffer()
{
	if (m_IBO_index < m_IBOs.size()) {
		return m_IBOs[m_IBO_index];
	}

	m_IBOs.push_back(IndexBuffer());
	return m_IBOs.back();
}
void rdt::glCore::IndexBufferDataComponent::OnFinishedDrawCall()
{
	m_IBO_index++;
}
void rdt::glCore::IndexBufferDataComponent::BindIndexBuffer(const IndexBuffer& ibo)
{
	if (m_binded_IBO == ibo.GetID()) {
		return;
	}

	ibo.Bind();
	m_binded_IBO = ibo.GetID();
}
// ======================================================
void rdt::glCore::ShaderDataComponent::Init()
{
	// Set default shader
	Shader nShader;
	nShader.LoadDefaultShader();
	m_default_shader = nShader.GetID();
	m_shaders[m_default_shader] = nShader;

	// Binds the default shader
	nShader.Bind();
	m_current_shader = nShader.GetID();
}
void rdt::glCore::ShaderDataComponent::BindShader(ShaderID shader)
{
	if (m_current_shader == shader || !ShaderExists(shader)) {
		return;
	}

	m_shaders.at(shader).Bind();
	m_current_shader = shader;
}
bool rdt::glCore::ShaderDataComponent::ShaderExists(ShaderID shader)
{
	return m_shaders.find(shader) != m_shaders.end();
}
rdt::glCore::Shader& rdt::glCore::ShaderDataComponent::GetCurrentShader()
{
	return m_shaders.at(m_current_shader);
}
void rdt::glCore::ShaderDataComponent::SetMVP(const glm::mat4& mvp)
{
	m_MVP = mvp;
	mvp_initialized = true;
}
glm::mat4& rdt::glCore::ShaderDataComponent::GetMVP()
{
	if (!mvp_initialized) {
		RDT_CORE_WARN("glCore - Using unitialized model view projection matrix!");
	}

	return m_MVP;
}
// ======================================================
void rdt::glCore::TextureDataComponent::Init()
{
	m_texture_manager = std::make_shared<TextureManager>();
}
void rdt::glCore::TextureDataComponent::BindTextureRequests()
{
	m_texture_manager->BindTextures(m_texture_requests);
}
void rdt::glCore::TextureDataComponent::OnFinishedDrawCall()
{
	m_texture_requests.clear();
}