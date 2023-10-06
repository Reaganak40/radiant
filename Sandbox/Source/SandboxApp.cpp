#include <Radiant.h>
#include <Engine/EntryPoint.h>

#include "Scenes/Sandbox.h"

using namespace rdt;

class SandboxApp : public Application
{
public:
	SandboxApp()
	{
		Start("Sandbox", 1280, 720);

		AddScene("Sandbox Scene", new Sandbox);
		SetScene("Sandbox Scene");
	}

	~SandboxApp()
	{

	}

};

rdt::Application* rdt::CreateApplication()
{
	return new SandboxApp;
}