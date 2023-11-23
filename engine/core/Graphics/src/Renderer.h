#pragma once
#include "graphics_api.h"

namespace rdt::graphics {

	enum RenderAPI {
		RenderAPI_OpenGL,
		RenderAPI_DirectX,
	};

	class GRAPHICS_API Renderer {
	public:
		struct Impl;
	private:
		static Impl* m_impl;
		static RenderAPI m_framework;
	public:

		/*
			Sets the graphical API to use for rendering. By defualt, the Renderer
			uses OpenGL, but can be changed to other RenderAPIs.

			NOTE: This needs to be called before Initialize()
			NOTE: OpenGL is the only framework supported at the moment.
		*/
		static void SetFramework(RenderAPI framework);

		/*
			Creates a new instance of the Renderer
		*/
		static void Initialize();

		/*
			Destroys the Renderer's singleton instance
		*/
		static void Destroy();

		/*
			Launches the Application's window under the previously
			described configurations
		*/
		static void LaunchWindow();
	};

}