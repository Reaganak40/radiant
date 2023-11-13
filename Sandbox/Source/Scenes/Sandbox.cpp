#include "Sandbox.h"
#include "Layers/TestLayer.h"

using namespace rdt;

Sandbox::Sandbox()
{
	TextureManager::LoadTextureFromPNG("bullet", "Resources/bullet.png");
	AddLayer(std::make_shared<TestLayer>());
}

Sandbox::~Sandbox()
{
}
