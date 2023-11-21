#include "pch.h"
#include "Application.h"
#include "Internal/State.h"
#include "Internal/Window.h"
#include "Texture/TextureManager.h"
#include "Utils/ErrorHandling.h"

#include <Radiant/Logger.h>

rdt::glCore::Window& GetWindow(rdt::glCore::WindowImpl* ptr) {
    return *((rdt::glCore::Window*)ptr);
}

#define APP_WINDOW GetWindow(m_windowImpl)
#define TEXTURE_MANAGER GetWindow(m_windowImpl).GetTextureManager()

rdt::glCore::glApplication::glApplication(const char* name)
{
    size_t length = strlen(name) + 1;
    m_name = new char[length];
    strcpy_s(m_name, length, name);

    m_windowImpl = nullptr;
}

rdt::glCore::glApplication::~glApplication()
{
    delete m_name;

    if (m_windowImpl != nullptr) {
        delete (Window*)m_windowImpl;
    }
}

const char* rdt::glCore::glApplication::GetName()
{
	return m_name;
}

bool rdt::glCore::glApplication::LaunchWindow()
{
    if (m_windowImpl == nullptr) {
        m_windowImpl = new Window;
    }

    return APP_WINDOW.LaunchWindow(m_name);
}

bool rdt::glCore::glApplication::AppShouldClose()
{
    return APP_WINDOW.WindowShouldClose();
}

void rdt::glCore::glApplication::BeginFrame()
{
    APP_WINDOW.BeginFrame();
}

void rdt::glCore::glApplication::Clear()
{
    APP_WINDOW.ClearViewport();
}

void rdt::glCore::glApplication::SetClearColor(float r, float g, float b, float a)
{
    APP_WINDOW.SetClearColor(r, g, b, a);
}

void rdt::glCore::glApplication::EndFrame()
{
    APP_WINDOW.EndFrame();
}

rdt::glCore::ViewportID rdt::glCore::glApplication::CreateViewport(int xPos, int yPos, int width, int height)
{
    return APP_WINDOW.CreateViewport(xPos, yPos, width, height);
}

void rdt::glCore::glApplication::SetViewport(ViewportID vID)
{
    APP_WINDOW.SetViewport(vID);
}

rdt::glCore::TextureID rdt::glCore::glApplication::CreateTextureFromPNG(const char* filepath)
{
    return TEXTURE_MANAGER.LoadTextureFromPNG(filepath);
}

const rdt::glCore::Texture& rdt::glCore::glApplication::GetTexture(TextureID tID)
{
    return TEXTURE_MANAGER.GetTexture(tID);
}

void rdt::glCore::glApplication::UseTexture(TextureID tID)
{
    if (!TEXTURE_MANAGER.TextureExists(tID)) {
        RDT_CORE_WARN("UseTexture - Texture [id:{}] does not exist! Request ignored.", tID);
        return;
    }

    APP_WINDOW.AddToTextureList(tID);
}

rdt::glCore::CameraID rdt::glCore::glApplication::CreateCamera()
{
    return APP_WINDOW.CreateCamera();
}

rdt::glCore::Camera& rdt::glCore::glApplication::GetCamera(CameraID cID)
{
    return APP_WINDOW.GetCamera(cID);
}

void rdt::glCore::glApplication::SetCamera(CameraID cID)
{
    APP_WINDOW.SetCamera(cID);
}

void rdt::glCore::glApplication::SubmitVertices(Vertex* vertices, size_t vertexCount)
{
    APP_WINDOW.SubmitVertices(vertices, vertexCount);
}

void rdt::glCore::glApplication::SubmitIndices(unsigned int* indices, size_t indexCount, size_t vertexCount)
{
    APP_WINDOW.SubmitIndices(indices, indexCount, vertexCount);
}

void rdt::glCore::glApplication::Draw()
{
    APP_WINDOW.DrawContext();
}

// ========================================================================================

rdt::glCore::glApplication& rdt::glCore::CreateApplication(const char* application_name)
{
	if (strlen(application_name) == 0) {
        application_name = "OpenGL Window";
	}

	return glState::Get()->RegisterApplication(application_name);
}

rdt::glCore::glApplication& rdt::glCore::GetApplication(const char* application_name)
{
	return glState::Get()->GetApplication(application_name);
}

bool rdt::glCore::DestroyApplication(glApplication& application)
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
