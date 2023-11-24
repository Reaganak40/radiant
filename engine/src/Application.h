#pragma once
#include "radiant_api.h"

// Forward Declarations
namespace rdt {
	using UniqueID = unsigned int;
	class Scene;

	class WindowConfig;
}

namespace rdt {

	enum LoopPhase {
		LoopPhase_Begin,
		LoopPhase_ProcessInput,
		LoopPhase_WorldUpdate,
		LoopPhase_FinalUpdate,
		LoopPhase_RenderUpdate,
		LoopPhase_End,
	};

	class RADIANT_API Application
	{
	private:
		struct Impl;
		Impl* m_impl;

	public:
		Application();
		virtual ~Application();

		/*
			Runs and manages the game loop, and will continue to run until the application is closed.
		*/
		void Run();

	protected:
		/*
			Returns the application's window configuration struct, which can be
			edited before OnStart to configurate the game window.
		*/
		WindowConfig& GetWindowConfig();

		/*
			Called right before the game loop begins to define and setup scenes.
		*/
		virtual void OnGameBegin() {}
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}