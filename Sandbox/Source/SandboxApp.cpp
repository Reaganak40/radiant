#include <Radiant.h>
#include <Engine/EntryPoint.h>

#include "Scenes/Sandbox.h"

using namespace rdt;


class SandboxApp : public Application
{
public:
	SandboxApp()
	{
		// Snake Application Configurations
		ApplicationConfig appConfig;
		appConfig.appName = "Sandbox";
		appConfig.windowWidth = 1280;
		appConfig.windowHeight = 720;
		appConfig.backgroundColor = WHITE;
		appConfig.cameraAspectRatio = AR_16_9;
		SetApplicationConfig(appConfig);
	}

	void OnGameBegin() override final
	{
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