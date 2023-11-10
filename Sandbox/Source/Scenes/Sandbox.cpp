#include "Sandbox.h"
#include "Layers/Sandbox2D.h"
#include "Layers/TestLayer.h"

using namespace rdt;

Sandbox::Sandbox()
{
	RegisterToMessageBus("Sandbox");

	TextureManager::LoadTextureFromPNG("bullet", "Resources/bullet.png");

	AddLayer(new Sandbox2D);
	AddLayer(new TestLayer);
}

Sandbox::~Sandbox()
{
}
