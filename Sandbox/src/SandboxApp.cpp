#include <Radiant.h>
#include <Engine/EntryPoint.h>

#include <Radiant/Utils.h>

using namespace rdt;

class SandboxApp : public Application
{
public:
	SandboxApp()
	{
		/* Configure Game Window */
		auto& windowConfig = GetWindowConfig();
		windowConfig.SetWindowName("Sandbox");
		windowConfig.SetWindowSize(1280, 720);
		windowConfig.SetBackgroundColor(GREEN);
		windowConfig.SetAspectRatio(AR_1_1);
	}

	void OnGameBegin() override final
	{
	}

	~SandboxApp()
	{
	}

};

rdt::Application* rdt::CreateApplication()
{
	return new SandboxApp;
}