#include "TestLayer.h"
#include "GameObjects/TestQuad.h"
#include "GameObjects/MyBeautifulObject.h"

using namespace rdt;

TestLayer::TestLayer(const std::string& alias)
{
	RegisterToMessageBus(alias);

	CreateNewRealm();

	TestQuad* quad3;
	RegisterGameObject(quad3 = new TestQuad("quad3", { 160, 500 }));
	quad3->RegisterToRealm(GetRealms()[0]);
	quad3->RegisterToLayer(GetID());

	RegisterGameObject(new MyBeautifulObject("Object1"));
}

TestLayer::~TestLayer()
{
}

void TestLayer::OnAttach()
{
	// TODO: Bind GameObjects and GUIs
	BindAll();
}

void TestLayer::OnDetach()
{
	// TODO: Unbind GameObjects and GUIs
}

void TestLayer::OnProcessInput(const float deltaTime)
{

	/* 
		Calls ProcessInput on all binded game objects
		and GUIs.
	*/
	Layer::OnProcessInput(deltaTime);
}

void TestLayer::OnFinalUpdate()
{

	/* Calls FinalUpdate on all binded game objects. */
	Layer::OnFinalUpdate();
}

void TestLayer::OnRender()
{

	/* 
		Calls OnRender on all binded game objects
		and GUIs.
	*/
	Layer::OnRender();
}


