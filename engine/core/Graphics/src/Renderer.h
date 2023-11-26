#pragma once
#include "graphics_api.h"

// Forward Delcarations
namespace rdt {
	class WindowConfig;
	class Color;
}

namespace rdt::graphics {

	class GRAPHICS_API Renderer {
	public:
		struct Impl;
	private:
		static Impl* m_impl;
	public:

		/*
			Creates a new instance of the Renderer
		*/
		static void Initialize();

		/*
			Destroys the Renderer's singleton instance
		*/
		static void Destroy();

		/*
			Assigns a window configuration to the Renderer, which will be
			used in the creation of a new window.
		*/
		static void SetWindowConfig(std::shared_ptr<WindowConfig> windowConfig);

		/*
			Launches the Application's window under the previously
			described configurations
		*/
		static void LaunchWindow();

		static bool WindowShouldClose();

		static void OnBeginFrame();

		static void Render();

		static void OnEndFrame();

		static void SetClearColor(const Color& nColor);

		static float GetDeltaTime();
	};

}