#include "MyBeautifulObject.h"

using namespace rdt;

MyBeautifulObject::MyBeautifulObject(const std::string& alias)
{
	RegisterToMessageBus(alias);
}

MyBeautifulObject::~MyBeautifulObject()
{
}

void MyBeautifulObject::OnBind()
{
	// TODO: Add Object to physical world and define physics properties
}

void MyBeautifulObject::OnRelease()
{
	// TODO: Remove Object from physical world
}

void MyBeautifulObject::OnMessage(rdt::Message msg)
{
	switch (msg.type) {
	default:
		break;
	}
}

void MyBeautifulObject::OnProcessInput(const float deltaTime)
{
}

void MyBeautifulObject::OnFinalUpdate()
{
}

void MyBeautifulObject::OnRender()
{
	// TODO: Call Render Pipeline
}
