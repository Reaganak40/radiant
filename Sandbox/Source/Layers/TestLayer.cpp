#include "TestLayer.h"
#include "GameObjects/TestQuad.h"

using namespace rdt;

TestLayer::TestLayer(const std::string& alias)
{
	RegisterToMessageBus(alias);

	CreateNewRealm();

	TestQuad* quad1;
	m_game_objects.push_back(quad1 = new TestQuad("quad3", { 160, 500 }));
	quad1->RegisterToRealm(m_realms[0]);
	quad1->RegisterToLayer(GetID());
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


