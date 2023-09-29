#include <Radiant.h>

class Sandbox : public rdt::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}

};

rdt::Application* rdt::CreateApplication()
{
	return new Sandbox;
}