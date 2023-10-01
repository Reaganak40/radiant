#include <Radiant.h>

using namespace rdt;

class Sandbox : public Application
{
public:
	Sandbox()
	{
		Start("Sandbox", 1280, 720);
	}

	~Sandbox()
	{

	}

};

rdt::Application* rdt::CreateApplication()
{
	return new Sandbox;
}