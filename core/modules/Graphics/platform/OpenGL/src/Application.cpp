#include "pch.h"
#include "../include/glCore.h"

#include "Internal/State.h"
#include "Internal/Window.h"
#include "Texture/TextureManager.h"
#include "../include/glCore.h"
#include "Utils/ErrorHandling.h"
#include "Logging/Log.h"

glCore::Window& GetWindow(glCore::WindowImpl* ptr) {
    return *((glCore::Window*)ptr);
}

#define APP_WINDOW GetWindow(m_windowImpl)
#define TEXTURE_MANAGER GetWindow(m_windowImpl).GetTextureManager()

glCore::glApplication::glApplication(const char* name)
{
    size_t length = strlen(name) + 1;
    m_name = new char[length];
    strcpy_s(m_name, length, name);

    m_windowImpl = nullptr;
}

glCore::glApplication::~glApplication()
{
    delete m_name;

    if (m_windowImpl != nullptr) {
        delete (Window*)m_windowImpl;
    }
}

const char* glCore::glApplication::GetName()
{
	return m_name;
}

bool glCore::glApplication::LaunchWindow()
{
    if (m_windowImpl == nullptr) {
        m_windowImpl = new Window;
    }

    return APP_WINDOW.LaunchWindow(m_name);
}

bool glCore::glApplication::AppShouldClose()
{
    return APP_WINDOW.WindowShouldClose();
}

void glCore::glApplication::BeginFrame()
{
    APP_WINDOW.BeginFrame();
}

void glCore::glApplication::Clear()
{
    APP_WINDOW.ClearViewport();
}

void glCore::glApplication::SetClearColor(float r, float g, float b, float a)
{
    APP_WINDOW.SetClearColor(r, g, b, a);
}

void glCore::glApplication::EndFrame()
{
    APP_WINDOW.EndFrame();
}

glCore::ViewportID glCore::glApplication::CreateViewport(int xPos, int yPos, int width, int height)
{
    return APP_WINDOW.CreateViewport(xPos, yPos, width, height);
}

void glCore::glApplication::SetViewport(ViewportID vID)
{
    APP_WINDOW.SetViewport(vID);
}

glCore::TextureID glCore::glApplication::CreateTextureFromPNG(const char* filepath)
{
    return TEXTURE_MANAGER.LoadTextureFromPNG(filepath);
}

const glCore::Texture& glCore::glApplication::GetTexture(TextureID tID)
{
    return TEXTURE_MANAGER.GetTexture(tID);
}

void glCore::glApplication::UseTexture(TextureID tID)
{
    if (!TEXTURE_MANAGER.TextureExists(tID)) {
        GL_CORE_WARN("UseTexture - Texture [id:{}] does not exist! Request ignored.", tID);
        return;
    }

    APP_WINDOW.AddToTextureList(tID);
}

glCore::CameraID glCore::glApplication::CreateCamera()
{
    return APP_WINDOW.CreateCamera();
}

glCore::Camera& glCore::glApplication::GetCamera(CameraID cID)
{
    return APP_WINDOW.GetCamera(cID);
}

void glCore::glApplication::SetCamera(CameraID cID)
{
    APP_WINDOW.SetCamera(cID);
}

void glCore::glApplication::SubmitVertices(Vertex* vertices, size_t vertexCount)
{
    APP_WINDOW.SubmitVertices(vertices, vertexCount);
}

void glCore::glApplication::SubmitIndices(unsigned int* indices, size_t indexCount, size_t vertexCount)
{
    APP_WINDOW.SubmitIndices(indices, indexCount, vertexCount);
}

void glCore::glApplication::Draw()
{
    APP_WINDOW.DrawContext();
}

// ========================================================================================

glCore::glApplication& glCore::CreateApplication(const char* application_name)
{
	if (strlen(application_name) == 0) {
        application_name = "OpenGL Window";
	}

	return glState::Get()->RegisterApplication(application_name);
}

glCore::glApplication& glCore::GetApplication(const char* application_name)
{
	return glState::Get()->GetApplication(application_name);
}

bool glCore::DestroyApplication(glApplication& application)
{
	auto state = glState::Get();
	
    if (!state->RemoveApplication(application.GetName())) {
        return false;
    }

	if (state->ApplicationCount() == 0) {
		state->Reset();
	}

    return true;
}
