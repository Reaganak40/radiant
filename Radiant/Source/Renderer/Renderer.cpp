#include "Renderer.h"
#include "ErrorHandling.h"

Renderer* Renderer::m_instance = nullptr;

Renderer::Renderer()
    : m_window(nullptr), m_window_name(""), m_window_width(0), m_window_height(0)
{
    /* Initialize the library */
    if (!glfwInit()) {
        ASSERT(false);
    }
}


Renderer::~Renderer()
{
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
    delete m_instance;
    m_instance = nullptr;
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

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void Renderer::DrawImpl()
{
    /* Swap front and back buffers */
    glfwSwapBuffers(m_window);

    /* Poll for and process events */
    glfwPollEvents();
}
