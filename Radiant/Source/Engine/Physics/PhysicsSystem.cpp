#include "pch.h"
#include "PhysicsSystem.h"

namespace rdt {
	PhysicsSystem::PhysicsSystem()
	{
		Signature nSignature;

		// Components utilized by this system
		ComponentManager::UpdateSignature<RigidBody2D>(nSignature);
		ComponentManager::UpdateSignature<Sprite>(nSignature);

		SetSignature(nSignature);
	}

	PhysicsSystem::~PhysicsSystem()
	{
	}

	void PhysicsSystem::Update(float deltaTime)
	{
		auto& entities = System::GetEntities();
		auto rigidBodies = System::GetComponent<RigidBody2D>();
	}
}
