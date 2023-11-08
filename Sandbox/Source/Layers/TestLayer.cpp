#include "TestLayer.h"

using namespace rdt;

class RectObject : public EntityDefinition, SpawnRect
{
private:
	Color m_color;
public:

	RectObject(double xPos, double yPos, double width, double height, Color nColor = BLACK)
		: SpawnRect(xPos, yPos, width, height)
	{
		m_color = nColor;
	}

	void OnCreate() override final
	{
		Register();
		Entity eID = GetID();

		Sprite sprite = SpawnRect::CreateSprite();
		sprite.model = ModelManager::GetModelID("common.model.rect");
		EntityManager::AddComponent<Sprite>(eID, sprite);

		Transform transform = SpawnRect::CreateTransform();
		EntityManager::AddComponent<Transform>(eID, transform);

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
	RealmID mRealm = CreateNewRealm();

	ColliderID rectCollider = ColliderManager::GetColliderID("common.collider.rect");
	{
		Entity entity = RegisterEntity(new RectObject(360, 200, 500, 50), "platform1");
		auto rigidbody = EntityManager::GetComponent<RigidBody2D>(entity);
		rigidbody->realmID = mRealm;
		rigidbody->colliderID = rectCollider;
	}

	{
		Entity entity = RegisterEntity(new RectObject(400, 800, 50, 100, BLUE), "player");
		auto rigidbody = EntityManager::GetComponent<RigidBody2D>(entity);
		rigidbody->realmID = mRealm;
		rigidbody->colliderID = rectCollider;
		rigidbody->use_gravity = true;
		rigidbody->max_velocity = Vec2d(300, 300);
	}
}

TestLayer::~TestLayer()
{
}

void TestLayer::OnAttach()
{
	Physics::ActivateRealm(GetRealms()[0]);

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


