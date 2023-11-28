#include <Radiant/Application.hpp>
#include <Radiant/EntryPoint.hpp>

#include <Radiant/Scene.hpp>
#include <Radiant/Logger.hpp>

#include <Radiant/Graphics.hpp>

using namespace rdt;

class TestLayer : public Layer {
private:
	float col[4];
	int dr, db;
public:

	TestLayer()
	{
		dr = 1;
		db = -1;

		col[0] = 0.2f;
		col[1] = 0.2f;
		col[2] = 0.8f;
		col[3] = 1.0f;
	}

	~TestLayer()
	{
	}

	void OnAttach() override final {
		RDT_TRACE("OnAttach called for TestLayer!");
	}

	void OnBind() override final {
		RDT_TRACE("OnBind called for TestLayer!");
	}

	void OnProcessInput(float deltaTime) override final {

		col[0] += (dr) * (0.4f) * deltaTime;
		if (col[0] >= 0.8f) {
			dr = -1;
			col[0] = 0.8f;
		}
		else if (col[0] <= 0.2f) {
			dr = 1;
			col[0] = 0.2f;
		}

		col[2] += (db) * (0.4f) * deltaTime;
		if (col[2] >= 0.8f) {
			db = -1;
			col[2] = 0.8f;
		}
		else if (col[2] <= 0.2f) {
			db = 1;
			col[2] = 0.2f;
		}
	}

	void OnRenderUpdate() override final {
		Renderer::SetBackgroundColor(Color(col[0], col[1], col[2], col[3]));
	}
};

class TestScene : public Scene {
private:
public:

	TestScene()
	{
	}

	~TestScene()
	{
	}

	void OnBind() override final {

		RDT_TRACE("Binding SandboxScene!");
		AttachLayer("testLayer");
	}

	void OnRelease() override final {
	}
};

class SandboxApp : public Application
{
public:
	SandboxApp()
	{
		/* Configure Game Window */
		auto& windowConfig = GetWindowConfig();
		windowConfig.SetWindowName("Sandbox");
		windowConfig.SetWindowSize(1280, 720);
		windowConfig.SetAspectRatio(AR_1_1);
	}

	void OnGameBegin() override final
	{
		/* Define scenes and layers */
		RegisterScene<TestScene>("testScene");
		RegisterLayer<TestLayer>("testLayer");

		/* Set the starting scene */
		BeginScene("testScene");
	}

	~SandboxApp()
	{
	}
};

rdt::Application* rdt::CreateApplication()
{
	return new SandboxApp;
}