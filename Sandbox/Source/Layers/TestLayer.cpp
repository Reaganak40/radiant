#include "TestLayer.h"
#include "GameObjects/TestQuad.h"

using namespace rdt;

class Platform : public EntityDefinition, SpawnRect
{
private:
public:

	Platform(double xPos, double yPos, double width, double height)
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

		Renderable renderable;
		renderable.fillColor = BLACK;

		EntityManager::AddComponent<Renderable>(eID, renderable);

		SystemManager::AddEntity<PhysicsSystem>(eID);
		SystemManager::AddEntity<RenderSystem>(eID);
	}
};

class Person : public EntityDefinition, SpawnRect
{
private:
	Color m_color;
public:

	Person(double xPos, double yPos, Color spriteColor)
		: SpawnRect(xPos, yPos, 50, 100)
	{
		m_color = spriteColor;
	}

	void OnCreate() override final
	{
		Register();
		Entity eID = GetID();

		Sprite sprite = SpawnRect::CreateSprite();
		EntityManager::AddComponent<Sprite>(eID, sprite);

		RigidBody2D rigidBody;
		EntityManager::AddComponent<RigidBody2D>(eID, rigidBody);

		Renderable renderable;
		renderable.fillColor = m_color;

		EntityManager::AddComponent<Renderable>(eID, renderable);

		SystemManager::AddEntity<PhysicsSystem>(eID);
		SystemManager::AddEntity<RenderSystem>(eID);
	}

};


TestLayer::TestLayer(const std::string& alias)
{
	RegisterToMessageBus(alias);
	CreateNewRealm();

	RegisterEntity(new Platform(360, 100, 500, 50), "platform1");
	RegisterEntity(new Person(360, 300, BLUE), "player");

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


