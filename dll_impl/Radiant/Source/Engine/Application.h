#pragma once

#include "Core.h"

namespace rdt {

	class RADIANT_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}