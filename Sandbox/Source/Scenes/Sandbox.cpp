#include "Sandbox.h"
#include "Layers/TestLayer.h"

using namespace rdt;

Sandbox::Sandbox()
{
	TextureManager::LoadTextureFromPNG("bullet", "Resources/bullet.png");
	
	AddRealm();

	AddLayer(std::make_shared<TestLayer>());


}

Sandbox::~Sandbox()
{
}
