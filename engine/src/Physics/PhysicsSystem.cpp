#include "pch.h"
#include "PhysicsSystem.h"
#include "Collider.h"
#include "Physics.h"
#include "ECS/ECS.h"

namespace rdt {
	PhysicsSystem::PhysicsSystem()
	{
		Signature nSignature;

		// Components utilized by this system
		ComponentManager::UpdateSignature<RigidBody2D>(nSignature);
		ComponentManager::UpdateSignature<Sprite>(nSignature);
		ComponentManager::UpdateSignature<Transform>(nSignature);

		SetSignature(nSignature);
	}

	PhysicsSystem::~PhysicsSystem()
	{
	}

	void PhysicsSystem::Update(float deltaTime)
	{
		auto rigidBodies = System::GetComponent<RigidBody2D>();
		auto transforms = System::GetComponent<Transform>();
		auto sprites = System::GetComponent<Sprite>();

		for (auto entity : GetEntities()) {
			if (!rigidBodies->HasEntity(entity) ||
				!transforms->HasEntity(entity) ||
				!sprites->HasEntity(entity)){
				continue;
			}


			auto& rigidBody = rigidBodies->GetData(entity);
			if (ColliderManager::ColliderExists(rigidBody.colliderID)) {
				Physics::AddEntityToRealm(rigidBody.realmID, entity);
			}
		}

		Physics::OnUpdate(deltaTime);
	}
}
