#pragma once
#include "pch.h"

namespace Radiant {
	class Application {
	private:

	public:
		Application();
		~Application();

		/*
			Starts a new Radiant application, launching the window instance.
		*/
		void Start(std::string appName, unsigned int windowWidth = 1280, unsigned int windowHeight = 720);

		/*
			Returns true if the application is running and the window is launched.
		*/
		bool IsRunning();

		/*
			Start of game loop, clears the window, gets new deltaTime and prepares internals
			for following update procedures.
		*/
		void Update();

		/*
			Runs the renderer draw command queue, resulting in a new frame.
		*/
		void Render();

	};
}