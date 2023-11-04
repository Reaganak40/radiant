#include "TestLayer.h"
#include "GameObjects/TestQuad.h"
#include "GameObjects/MyBeautifulObject.h"

using namespace rdt;

class PinkBox : public EntityDefinition, SpawnRect {
private:
public:

	PinkBox(double xPos, double yPos, double width, double height)
		: SpawnRect(xPos, yPos, width, height)
	{
	}

	void OnCreate() override final
	{
		Register();
		Entity eID = GetID();

		Sprite sprite = SpawnRect::CreateSprite();
		EntityManager::AddComponent<Sprite>(eID, sprite);

		RigidBody2D rigidBody;
		EntityManager::AddComponent<RigidBody2D>(eID, rigidBody);

		static int i = 0;
		Renderable renderable;
		renderable.polygon_color = i % 2 == 0 ? PINK : BLUE;
		i++;

		EntityManager::AddComponent<Renderable>(eID, renderable);

		SystemManager::AddEntity<PhysicsSystem>(eID);
		SystemManager::AddEntity<RenderSystem>(eID);
	}
};

TestLayer::TestLayer(const std::string& alias)
{
	RegisterToMessageBus(alias);
	CreateNewRealm();

	TestQuad* quad3;
	RegisterGameObject(quad3 = new TestQuad("quad3", { 160, 500 }));
	quad3->RegisterToRealm(GetRealms()[0]);
	quad3->RegisterToLayer(GetID());

	RegisterGameObject(new MyBeautifulObject("Object1"));

	for (int i = 1; i < 17; i++) {
		RegisterEntity(new PinkBox(100 * i, 100, 50, 50), "pinkbox" + std::to_string(i));
	}
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


