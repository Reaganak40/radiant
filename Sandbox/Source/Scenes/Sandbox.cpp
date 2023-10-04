#include "Sandbox.h"
#include "Layers/Sandbox2D.h"

using namespace rdt;

Sandbox::Sandbox()
{
	RegisterToMessageBus("Sandbox");
	AddLayer(new Sandbox2D);
}

Sandbox::~Sandbox()
{
}
