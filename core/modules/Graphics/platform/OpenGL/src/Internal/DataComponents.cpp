#include "pch.h"
#include "DataComponents.h"
#include "Logging/Log.h"
#include "Utils/ErrorHandling.h"

// ======================================================
void glCore::WindowDataComponent::SetWindowName(const std::string& name)
{
	m_window_name = name;
}
void glCore::WindowDataComponent::SetWindowSize(int windowWidth, int windowHeight)
{
	m_window_width = windowWidth;
	m_window_height = windowHeight;
}
bool glCore::WindowDataComponent::Launch()
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
	m_window = glfwCreateWindow(mode->width, mode->height, m_window_name.c_str(), NULL, NULL);
	if (!m_window)
	{
		GL_CORE_FATAL("Failed to create glfw window");
		return false;
	}

	/* Start with maximized window */
	glfwMaximizeWindow(m_window);
	glfwGetWindowSize(m_window, &m_window_width, &m_window_height);

	/* Make the window's context current */
	glfwMakeContextCurrent(m_window);

	// Load all OpenGL functions using the glfw loader function
	// If you use SDL you can use: https://wiki.libsdl.org/SDL_GL_GetProcAddress
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		GL_CORE_FATAL("Failed to initialize OpenGL context");
		glfwMakeContextCurrent(NULL);
		return false;
	}

	// glad populates global constants after loading to indicate,
	// if a certain extension/version is available.
	GL_CORE_INFO("OpenGL {}.{}", GLVersion.major, GLVersion.minor);

	// For opengl error handling
	EnableErrorCallback();
}
// ======================================================
void glCore::VertexArrayDataComponent::Init()
{
	m_vertex_array = std::make_shared<VertexArray>();
}
glCore::VertexArray& glCore::VertexArrayDataComponent::GetVertexArray()
{
	return* m_vertex_array;
}
// ======================================================
void glCore::ViewportDataComponent::Init(int windowWidth, int windowHeight)
{
	m_viewports[GL_CORE_DEFAULT_VIEWPORT_ID] = Viewport(0, 0, windowWidth, windowHeight);
	m_viewports.at(GL_CORE_DEFAULT_VIEWPORT_ID).Bind();
}
glCore::ViewportID glCore::ViewportDataComponent::NextViewportID()
{
	return ++viewportIDCounter;
}
glCore::ViewportID glCore::ViewportDataComponent::CreateViewport(int xPos, int yPos, int width, int height)
{
	ViewportID nID = NextViewportID();
	m_viewports[nID] = Viewport(xPos, yPos, width, height);
	return nID;
}
void glCore::ViewportDataComponent::BindViewport(ViewportID vID)
{
	if (m_current_viewport == vID || !ViewportExists(vID)) {
		return;
	}

	m_viewports.at(vID).Bind();
	m_current_viewport = vID;
}
bool glCore::ViewportDataComponent::ViewportExists(ViewportID vID)
{
	return m_viewports.find(vID) != m_viewports.end();
}
glCore::Viewport& glCore::ViewportDataComponent::GetCurrentViewport()
{
	return m_viewports.at(m_current_viewport);
}
void glCore::ViewportDataComponent::OnCameraChange(Camera& bindedCamera)
{
	if (!bindedCamera.ShouldMaintainAspectRatio()) {
		return;
	}
	Viewport& viewport = GetCurrentViewport();
	int nWidth = 0;
	int nHeight = 0;
	int nX = 0;
	int nY = 0;
	bindedCamera.GetCameraDimensionsFromViewport(viewport.width, viewport.height, &nWidth, &nHeight);

	nX = (viewport.width / 2) - (nWidth / 2);
	nY = (viewport.height / 2) - (nHeight / 2);
	
	Viewport::Bind(nX, nY, nWidth, nHeight);
}
// ======================================================
void glCore::VertexBufferDataComponent::Reset()
{
	for (size_t i = 0; i < m_VBO_index; i++) {
		m_VBOs[i].Flush();
	}

	m_VBO_index = 0;
}
glCore::VertexBuffer& glCore::VertexBufferDataComponent::GetVertexBuffer()
{
	if (m_VBO_index < m_VBOs.size()) {
		return m_VBOs[m_VBO_index];
	}

	m_VBOs.push_back(VertexBuffer());
	return m_VBOs.back();
}
void glCore::VertexBufferDataComponent::OnFinishedDrawCall()
{
	m_VBO_index++;
}
void glCore::VertexBufferDataComponent::BindVertexBuffer(const VertexBuffer& vbo)
{
	if (m_binded_VBO == vbo.GetID()) {
		return;
	}

	vbo.Bind();
	m_binded_VBO = vbo.GetID();
}
// ======================================================
void glCore::IndexBufferDataComponent::Reset()
{
	for (size_t i = 0; i < m_IBO_index; i++) {
		m_IBOs[i].Flush();
	}

	m_IBO_index = 0;
}
glCore::IndexBuffer& glCore::IndexBufferDataComponent::GetIndexBuffer()
{
	if (m_IBO_index < m_IBOs.size()) {
		return m_IBOs[m_IBO_index];
	}

	m_IBOs.push_back(IndexBuffer());
	return m_IBOs.back();
}
void glCore::IndexBufferDataComponent::OnFinishedDrawCall()
{
	m_IBO_index++;
}
void glCore::IndexBufferDataComponent::BindIndexBuffer(const IndexBuffer& ibo)
{
	if (m_binded_IBO == ibo.GetID()) {
		return;
	}

	ibo.Bind();
	m_binded_IBO = ibo.GetID();
}
// ======================================================
void glCore::ShaderDataComponent::Init()
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
void glCore::ShaderDataComponent::BindShader(ShaderID shader)
{
	if (m_current_shader == shader || !ShaderExists(shader)) {
		return;
	}

	m_shaders.at(shader).Bind();
	m_current_shader = shader;
}
bool glCore::ShaderDataComponent::ShaderExists(ShaderID shader)
{
	return m_shaders.find(shader) != m_shaders.end();
}
glCore::Shader& glCore::ShaderDataComponent::GetCurrentShader()
{
	return m_shaders.at(m_current_shader);
}
// ======================================================
void glCore::TextureDataComponent::Init()
{
	m_texture_manager = std::make_shared<TextureManager>();
}
void glCore::TextureDataComponent::BindTextureRequests()
{
	m_texture_manager->BindTextures(m_texture_requests);
}
void glCore::TextureDataComponent::OnFinishedDrawCall()
{
	m_texture_requests.clear();
}
// ======================================================
void glCore::CameraDataComponent::Init()
{
	m_cameras[GL_CORE_NULL_CAMERA_ID];
}
glCore::CameraID glCore::CameraDataComponent::NextCameraID()
{
	return ++cameraIDCounter;
}

bool glCore::CameraDataComponent::BindCamera(CameraID camera)
{
	if (m_current_camera == camera || !CameraExists(camera)) {
		return false;
	}

	m_current_camera = camera;
	return true;
}
glCore::CameraID glCore::CameraDataComponent::CreateCamera()
{
	CameraID nID = NextCameraID();
	m_cameras[nID];
	return nID;
}
bool glCore::CameraDataComponent::CameraExists(CameraID camera)
{
	return m_cameras.find(camera) != m_cameras.end();
}
glCore::Camera& glCore::CameraDataComponent::GetCurrentCamera()
{
	return m_cameras.at(m_current_camera);
}
glCore::Camera& glCore::CameraDataComponent::GetAnyCamera(CameraID camera)
{
	return m_cameras.at(camera);
}
