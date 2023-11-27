#include "pch.h"
#include <Radiant/Window/Window.hpp>
#include <Radiant/Window/WindowConfig.hpp>

static rdt::Window* windowInstance = nullptr;

#ifdef RDT_USE_OPENGL

#include <Core/OpenGL.hpp>

class WindowImplGL : public rdt::Window {
public:
	WindowImplGL() {}
	~WindowImplGL() {}

	rdt::WindowConfig& GetWindowConfig() override final {

	}

	void LaunchWindow() override final {

	}

	bool IsOpen() override final {

	}
};

#define NEW_WINDOW_INSTANCE() new WindowImplGL

#else
#define NEW_WINDOW_INSTANCE() nullptr
#endif

// =======================================================================
rdt::Window& rdt::Window::Create()
{
	Destroy(); // if already instantiated
	windowInstance = NEW_WINDOW_INSTANCE();
}

void rdt::Window::Destroy()
{
	if (windowInstance != nullptr) {
		delete windowInstance;
		windowInstance = nullptr;
	}
}

rdt::Window& rdt::Window::GetInstance()
{
	if (windowInstance == nullptr) {
		Create();
	}

	return *windowInstance;
}
