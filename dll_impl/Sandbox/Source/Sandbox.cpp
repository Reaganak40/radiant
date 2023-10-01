#include <Radiant.h>

using namespace rdt;

class Sandbox : public Application
{
public:
	Sandbox()
	{
		Renderer::CreateWindow("Sandbox", 1280, 720, false);
	}

	~Sandbox()
	{

	}

};

rdt::Application* rdt::CreateApplication()
{
	return new Sandbox;
}