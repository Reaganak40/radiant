#include "Sandbox.h"
#include "Layers/Sandbox2D.h"

using namespace rdt;

Sandbox::Sandbox()
{
	RegisterToMessageBus("Sandbox");
	AddLayer(new Sandbox2D);

	Camera* defaultCamera = Renderer::GetCamera();
	defaultCamera->SetViewport({ 100, 100 }, { 1280, 720 });
	defaultCamera->SetBackgroundColor(Color(0.1f, 0.1f, 0.1f, 1.0f));
}

Sandbox::~Sandbox()
{
}
