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

class Controller : public systems::ProcessInput {
private:
	std::vector<InputState> moveLeftCond{ LEFT_KEY_PRESS, LEFT_KEY_DOWN };
	std::vector<InputState> moveRightCond{ RIGHT_KEY_PRESS, RIGHT_KEY_DOWN };

public:

	Controller() {
		Signature nSignature;

		// Components utilized by this system
		ComponentManager::UpdateSignature<RigidBody2D>(nSignature);

		SetSignature(nSignature);
	}

	void Update (float deltaTime) override final {
		auto rigidbodies = GetComponent<RigidBody2D>();

		bool moveLeft = Input::CheckInput(moveLeftCond);
		bool moveRight = Input::CheckInput(moveRightCond);

		for (auto entity : GetEntities()) {
			if (!rigidbodies->HasEntity(entity)) {
				continue;
			}
			
			auto& rigidbody = rigidbodies->GetData(entity);
			rigidbody.velocity.x = 0;

			if (moveLeft) {
				rigidbody.velocity.x -= 400;
			}

			if (moveRight) {
				rigidbody.velocity.x += 400;
			}
		}
	}
};


TestLayer::TestLayer(const std::string& alias)
{
	SystemManager::RegisterSystem<Controller>();
	RealmID mRealm = GetRealm();

	ColliderID rectCollider = ColliderManager::GetColliderID("common.collider.rect");
	{
		Entity entity = RegisterEntity(new RectObject(360, 200, 500, 50), "platform1");
		auto rigidbody = EntityManager::GetComponent<RigidBody2D>(entity);
		rigidbody->realmID = mRealm;
		rigidbody->colliderID = rectCollider;
	}

	{
		Entity entity = RegisterEntity(new RectObject(1100, 500, 500, 50), "platform2");
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
		rigidbody->physicalProperties |= PhysicalProperty_Bouncy;

		SystemManager::AddEntity<Controller>(entity);
	}

	{
		Entity entity = RegisterEntity(new RectObject(700, 800, 50, 100, RED), "red_guy");
		auto rigidbody = EntityManager::GetComponent<RigidBody2D>(entity);
		rigidbody->realmID = mRealm;
		rigidbody->colliderID = rectCollider;
		rigidbody->use_gravity = true;
		rigidbody->physicalProperties |= PhysicalProperty_Bouncy;
	}

	{
		Entity entity = RegisterEntity(new RectObject(1200, 200, 100, 100, WHITE), "bullet");
		auto rigidbody = EntityManager::GetComponent<RigidBody2D>(entity);
		rigidbody->realmID = mRealm;
		rigidbody->colliderID = rectCollider;
		rigidbody->use_gravity = false;

		auto renderable = EntityManager::GetComponent<Renderable>(entity);
		renderable->texture = TextureManager::GetTextureID("bullet");
	}
}

TestLayer::~TestLayer()
{
}

void TestLayer::OnAttach()
{
}

void TestLayer::OnDetach()
{
	// TODO: Unbind GameObjects and GUIs
}

void TestLayer::OnProcessInput(const float deltaTime)
{
}

void TestLayer::OnFinalUpdate()
{
}

void TestLayer::OnRenderUpdate()
{
}


