#pragma once
#include "pch.h"
#include "Core.h"

namespace rdt {

	class RADIANT_API Application
	{
	public:
		Application();
		virtual ~Application();

		/*
			Starts a new rdt application, launching the window instance.
			Aspect ratio is 16:9 by default.
			Window is resizable by default.
		*/
		void Start(std::string appName, unsigned int windowWidth = 1280, unsigned int windowHeight = 720, bool resizable = true);

		/*
			Runs and manages the game loop, and will continue to run until
			the application is closed.
		*/
		void Run();

	private:
		bool ShouldWindowClose();
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}